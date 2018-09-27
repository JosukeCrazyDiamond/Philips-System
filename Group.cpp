#include "Group.h"

Group::Group(){};
Group::~Group(){};

int Group::getId() { return id; }
void Group::setId(int id) {
    this->id = id;
}

std::string Group::getName() { return name; }
void Group::setName(const std::string& name) {
    this->name = name;
}

LightState* Group::getLightState() { return lightState; }
void Group::setLightState(LightState* lightState) {
    this->lightState = lightState;
}

vector<int> Group::getLights() { return lights; }

void Group::addLight(int lightId) {
    vector<int>::iterator index = find(lights.begin(), lights.end(), lightId);
    if (index == lights.end())
        lights.push_back(lightId);
}

void Group::removeLight(int lightId) {
    vector<int>::iterator index = find(lights.begin(), lights.end(), lightId);
    if (index != lights.end())
        lights.erase(index);
}