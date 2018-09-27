#ifndef WT_PHILIP_HUE_USER_H
#define WT_PHILIP_HUE_USER_H

#include <Wt/WDateTime>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Auth/Dbo/AuthInfo>

#include <string>

namespace dbo = Wt::Dbo;

class User;
typedef Wt::Auth::Dbo::AuthInfo<User> AuthInfo;
/**
 * User Object which stores information of registered user
 * persist in WTt:Dbo database
 */
class User {
public:
    std::string firstname;      //firstName
    std::string lastname;       //lastname
    std::string email;          //email
    std::string password;       //password
    Wt::WDateTime lastLogin;    //last login time
    dbo::weak_ptr<AuthInfo> authInfo;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, firstname, "firstname");
        dbo::field(a, lastname, "lastname");
        dbo::field(a, email, "email");
        dbo::field(a, password, "password");
        dbo::field(a, lastLogin, "lastLogin");

        Wt::Dbo::hasOne(a, authInfo, "user");
    }
};

DBO_EXTERN_TEMPLATES(User);

#endif //WT_PHILIP_HUE_USER_H
