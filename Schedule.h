#ifndef WT_PHILIP_HUE_SCHEDULE_H
#define WT_PHILIP_HUE_SCHEDULE_H

#include <string>
#include <Wt/WDateTime>
#include <Wt/Json/Object>
#include <iostream>
#include <sstream>

#include "LightState.h"

using namespace std;
using namespace Wt;
/**
 * Schedule Object which stores information about each Schedule
 * pulled from Emulator
 */
class Schedule {
public:
    Schedule(const std::string& address);
    ~Schedule();

    int getId();
    void setId(int id);

    WDateTime getTime();
    void setTime(const std::string& timeStr);

    std::string getDescription();
    void setDescription(const std::string& description);

    std::string getName();
    void setName(const std::string& name);

    std::string getAddress();
    void setAddress(const std::string& address);

    std::string getTargetType();
    int getTargetId();
    void setTarget();

    std::string getMethod();
    void setMethod(const std::string& method);

    LightState* getLightState();
    void setLightState(LightState* lightState);



private:
    int id;                 //Schedule ID
    WDateTime time;         //Schedule Time
    string description;     //Schedule Description
    string name;            //Schedule Name
    string address;         //Schedule address
    string method;          //Schedule method

    string targetType;      //Whether schedule Lights/Groups
    int targetId;           //Light/Group ID

    LightState* lightState; //Light State
};
#endif //WT_PHILIP_HUE_SCHEDULE_H
