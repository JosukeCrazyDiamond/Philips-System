#ifndef WT_PHILIP_HUE_GROUP_H
#define WT_PHILIP_HUE_GROUP_H

#include <string>
#include <vector>
#include <algorithm>

#include "LightState.h"

using namespace std;
/**
 * Group Object which stores information about each group
 * pulled from Emulator
 */
class Group {
public:
    Group();
    ~Group();

    int getId();
    void setId(int id);

    string getName();
    void setName(const string& name);

    LightState* getLightState();
    void setLightState(LightState* lightState);

    vector<int> getLights();
    void addLight(int lightId);
    void removeLight(int lightId);

private:
    int id;                 //Group ID
    string name;            //Group Name

    LightState* lightState; //Group LightState

    vector<int> lights;     //Vector of Light Ids that this group controls
};
#endif //WT_PHILIP_HUE_GROUP_H
