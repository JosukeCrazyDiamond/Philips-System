#ifndef WT_PHILIP_HUE_LIGHTSTATE_H
#define WT_PHILIP_HUE_LIGHTSTATE_H

#include <string>

using namespace std;

/*
 * LightState Object that can be reused by Light, Group and Schedule
 */
class LightState {
public:
    LightState();
    ~LightState();

    int getCt();
    void setCt(int ct);

    int getSat();
    void setSat(int sat);

    string getEffect();
    void setEffect(const string& effect);

    int getBri();
    void setBri(int bri);

    int getHue();
    void setHue(int hue);

    bool isOn();
    void setOn(bool on);

    int getTransitionTime();
    void setTransitionTime(int transitionTime);
private:
    int ct;             //Color temperature
    int sat;            //Saturation
    string effect;      //Color Effect
    int bri;            //Brightness
    int hue;            //Hue
    bool on;            //ON/OFF
    int transitionTime; //Transition Time
};
#endif //WT_PHILIP_HUE_LIGHTSTATE_H
