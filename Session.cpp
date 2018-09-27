#include <Wt/Auth/Identity>
#include "Session.h"

#include "Wt/Auth/AuthService"
#include "Wt/Auth/HashFunction"
#include "Wt/Auth/PasswordService"
#include "Wt/Auth/PasswordStrengthValidator"
#include "Wt/Auth/PasswordVerifier"
#include "Wt/Auth/Dbo/AuthInfo"
#include "Wt/Auth/Dbo/UserDatabase"

using namespace Wt;
namespace dbo = Wt::Dbo;

namespace  {
    Auth::AuthService myAuthService;
    Auth::PasswordService myPasswordService(myAuthService);
}

void Session::configureAuth() {
    myAuthService.setAuthTokensEnabled(true, "logincookie");
    myAuthService.setEmailVerificationEnabled(true);

    Auth::PasswordVerifier *verifier = new Auth::PasswordVerifier();
    verifier->addHashFunction(new Auth::BCryptHashFunction(7));

    myPasswordService.setVerifier(verifier);
    myPasswordService.setStrengthValidator(new Auth::PasswordStrengthValidator());
    myPasswordService.setAttemptThrottlingEnabled(true);
}

Session::Session(const std::string& sqliteDb) : connection_(sqliteDb) {
    connection_.setProperty("show-queries", "true");

    session_.setConnection(connection_);

    session_.mapClass<User>("user");
    session_.mapClass<Bridge>("bridge");
    session_.mapClass<AuthInfo>("auth_info");
    session_.mapClass<AuthInfo::AuthIdentityType>("auth_identity");
    session_.mapClass<AuthInfo::AuthTokenType>("auth_token");

    users_ = new UserDatabase(session_);

    dbo::Transaction transaction(session_);
    try {
        session_.createTables();
        std::cerr << "Created database." << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Using existing database";
    }
    transaction.commit();
}

Session::~Session() {
    delete users_;
    bridges_.clear();
}

dbo::ptr<User> Session::getUser() const {
    if (login_.loggedIn()) {
        dbo::ptr<AuthInfo> authInfo = users_->find(login_.user());
        dbo::ptr<User> user = authInfo->user();

        if (!user) {
            user = session_.add(new User());
            authInfo.modify()->setUser(user);
        }

        return user;
    } else
        return dbo::ptr<User>();
}

dbo::ptr<User> Session::registerUser(const Wt::Auth::User& authUser) const {
    dbo::ptr<AuthInfo> authInfo = users_->find(authUser);

    dbo::ptr<User> user = authInfo->user();

    if (!user) {
        user = session_.add(new User());
        authInfo.modify()->setUser(user);
    }

    return user;
}

dbo::ptr<Bridge> Session::createBridge(Bridge* bridge) {
    dbo::Transaction transaction(session_);

    dbo::ptr<User> user = Session::getUser();
    bridge->account = user.id();
    dbo::ptr<Bridge> bridgePtr = session_.add(bridge);

    transaction.commit();

    return bridgePtr;
}

dbo::ptr<Bridge> Session::updateBridge(Bridge* bridge) {
    dbo::Transaction transaction(session_);

    dbo::ptr<Bridge> bridgeInDb = Session::bridge(bridge->id);
    bridgeInDb.modify()->location = bridge->location;
    bridgeInDb.modify()->ip = bridge->ip;
    bridgeInDb.modify()->port = bridge->port;
    bridgeInDb.modify()->username = bridge->username;

    transaction.commit();

    return bridgeInDb;
}

void Session::deleteBridge(int id) {
    dbo::Transaction transaction(session_);

    dbo::ptr<User> user = Session::getUser();
    dbo::ptr<Bridge> bridgeInDb = Session::bridge(id);
    bridgeInDb.remove();
    transaction.commit();
}

dbo::ptr<Bridge> Session::bridge(int id) {
    dbo::Transaction transaction(session_);

    dbo::ptr<User> user = Session::getUser();

    dbo::ptr<Bridge> bridge = session_.find<Bridge>().where("id = ? and account = ?").bind(id).bind(user.id());

    transaction.commit();

    return bridge;
}

std::vector<Bridge *> Session::bridges() {
    bridges_.clear();
    dbo::Transaction transaction(session_);
    dbo::ptr<User> user = Session::getUser();

    Bridges bridgesDb = session_.find<Bridge>().where("account = ?").bind(user.id());
    for (const dbo::ptr<Bridge> &bridge : bridgesDb) {
        Bridge *tempBridge = new Bridge();
        tempBridge->name = bridge->name;
        tempBridge->location = bridge->location;
        tempBridge->ip = bridge->ip;
        tempBridge->port = bridge->port;
        tempBridge->username = bridge->username;
        tempBridge->account = bridge->account;
        tempBridge->id = bridge->id;
        bridges_.push_back(tempBridge);
    }
    return bridges_;
}

Auth::AbstractUserDatabase& Session::users() {
    return *users_;
}

const Auth::AuthService& Session::auth() {
    return myAuthService;
}

const Auth::AbstractPasswordService& Session::passwordAuth() {
    return myPasswordService;
}