#include "Light.h"

Light::Light(){};
Light::~Light(){};

int Light::getId() { return id; }
void Light::setId(int id) {
    this->id = id;
}

string Light::getName() { return name; }
void Light::setName(const string& name) {
    this->name = name;
}

LightState* Light::getLightState() { return lightState; }
void Light::setLightState(LightState* lightState) {
    this->lightState = lightState;
}