#ifndef WT_PHILIP_HUE_SESSION_H
#define WT_PHILIP_HUE_SESSION_H

#include <Wt/Auth/Login>

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/backend/Sqlite3>

#include <vector>

#include "User.h"
#include "Bridge.h"

namespace dbo = Wt::Dbo;

typedef Wt::Auth::Dbo::UserDatabase<AuthInfo> UserDatabase;
typedef dbo::collection< dbo::ptr<Bridge> > Bridges;
/*
 * User Session Class which stores user login info
 */
class Session{
public:
    static void configureAuth();

    Session(const std::string& sqliteDb);
    ~Session();

    /*
     * Get login user
     */
    dbo::ptr<User> getUser() const;
    /*
     * register new user
     */
    dbo::ptr<User> registerUser(const Wt::Auth::User& authUser) const;
    /*
     * store new bridge info into database
     */
    dbo::ptr<Bridge> createBridge(Bridge* bridge);
    /*
     * update existing bridge info into database
     */
    dbo::ptr<Bridge> updateBridge(Bridge* bridge);
    /*
     * delete existing bridge info from database
     */
    void deleteBridge(int id);
    /*
     * get bridge info from database
     */
    dbo::ptr<Bridge> bridge(int id);
    /*
     * get all bridges info from database for current login user
     */
    std::vector<Bridge *> bridges();

    Wt::Auth::AbstractUserDatabase& users();
    Wt::Auth::Login& login() { return login_; }

    static const Wt::Auth::AuthService& auth();
    static const Wt::Auth::AbstractPasswordService& passwordAuth();

private:
    dbo::backend::Sqlite3 connection_;
    mutable dbo::Session session_;
    UserDatabase *users_;
    Wt::Auth::Login login_;
    std::vector<Bridge *> bridges_;
};

#endif //WT_PHILIP_HUE_SESSION_H
