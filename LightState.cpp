#include "LightState.h"

LightState::LightState() : ct(153), sat(0), effect("none"), bri(0), hue(0), on(true), transitionTime(0){};
LightState::~LightState(){};

int LightState::getCt() { return ct; }
void LightState::setCt(int ct) {
    this->ct = ct;
}

int LightState::getSat() { return sat; }
void LightState::setSat(int sat) {
    this->sat = sat;
}

string LightState::getEffect() { return effect; }
void LightState::setEffect(const string&effect) {
    this->effect = effect;
}

int LightState::getBri() { return bri; }
void LightState::setBri(int bri) {
    this->bri = bri;
}

int LightState::getHue() { return hue; }
void LightState::setHue(int hue) {
    this->hue = hue;
}

bool LightState::isOn() { return on; }
void LightState::setOn(bool on) {
    this->on = on;
}

int LightState::getTransitionTime() { return transitionTime; }
void LightState::setTransitionTime(int transitionTime) {
    this->transitionTime = transitionTime;
}