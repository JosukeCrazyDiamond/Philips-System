#ifndef WT_PHILIP_HUE_LIGHT_H
#define WT_PHILIP_HUE_LIGHT_H

#include <string>

#include "LightState.h"

using namespace std;
/**
 * Light Object which stores information about each light
 * pulled from Emulator
 */
class Light {
public:
    Light();
    ~Light();

    int getId();
    void setId(int id);

    string getName();
    void setName(const string& name);

    LightState* getLightState();
    void setLightState(LightState* lightState);

private:
    int id;                 //Light ID
    string name;            //Light Name

    LightState* lightState; //Light State
};
#endif //WT_PHILIP_HUE_LIGHT_H
