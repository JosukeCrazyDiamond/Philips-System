#ifndef WT_PHILIP_HUE_BRIDGE_H
#define WT_PHILIP_HUE_BRIDGE_H

#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>

#include <string>

namespace dbo = Wt::Dbo;
using namespace std;

class Bridge;
/**
 * Bridge Object which stores information used to connect Hue Emulator
 * persist in WTt:Dbo database
 */
class Bridge {
public:
    string name;
    string location;
    string ip;
    string port;
    string username;
    int account;
    int id;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, name, "name");            //Bridge Name
        dbo::field(a, location, "location");    //Bridge Location
        dbo::field(a, ip, "ip");                //Bridge IP, default emulator uses localhost
        dbo::field(a, port, "port");            //Bridge Port, default emulator uses 8000
        dbo::field(a, username, "username");    //Bridge Username, default emulator uses newdeveloper
        dbo::field(a, account, "account");      //User Account that this Bridge belongs to
        dbo::field(a, id, "id");                //index key in Database
    }
};

DBO_EXTERN_TEMPLATES(Bridge);

#endif //WT_PHILIP_HUE_BRIDGE_H
