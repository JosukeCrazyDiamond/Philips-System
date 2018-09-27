#ifndef WT_PHILIP_HUE_LIGHTWIDGET_H
#define WT_PHILIP_HUE_LIGHTWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WSlider>
#include <Wt/WRadioButton>
#include <Wt/WButtonGroup>
#include <Wt/Http/Client>
#include <Wt/Http/Message>
#include <Wt/WApplication>
#include <Wt/Json/Array>
#include <Wt/Json/Object>

#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>
#include <string>
#include "Light.h"

using namespace Wt;
using namespace std;
/*
 * Widget that display and allows interactions with Light functionality
 */
class LightWidget : public WContainerWidget {
public:
    LightWidget(WContainerWidget *parent = nullptr);
    /*
     * performs Call to Emulator to update Light Attributes
     */
    void updateLightAttr();
    /*
     * performs Call to Emulator to update Light State
     */
    void updateLightState();
    /*
     * performs Call to Emulator to update Light Attributes: ON/OFF
     */
    void turnLightOnOff();

    Light* getLight();
    void setLight(Light * light);
    /*
     * Initialize a new ScheduleWidget
     */
    void initNewScheduleWidget();

private:
    WText *IdField;                 //Light ID
    WInPlaceEdit *NameField;        //Light Name Edit

    WSlider *CtField;               //Light Color Temperature Edit
    WSlider *SatField;              //Light Saturation Edit
    WButtonGroup *EffectField;      //Light Effect Edit
    WSlider *BriField;              //Light Brightness Edit
    WSlider *HueField;              //Light Hue Edit
    WSlider *TransitionTimeField;   //Light TransitionTime Edit

    WText *OnOffField;              //Light ON/OFF Edit
    WPushButton *OnOffButton;

    WPushButton *UpdateAttrButton;  //Update Light Attribute Button
    WPushButton *UpdateStateButton; //Update Light State Button
    WPushButton *AddScheduleButton; //Add a new ScheduleWidget Button

    Light *light;
    //Helper methods for serialize Light info
    string serializeLightAttr();
    string serializeLightState();
    string serializeLightOnOff();

    /*
     * Handle Light updates response
     */
    void handleHttpResponse(boost::system::error_code err, const Http::Message& response);
    /*
     * Handle Light ON/OFF response
     */
    void handleOnOffHttpResponse(boost::system::error_code err, const Http::Message& response);
};

#endif //WT_PHILIP_HUE_LIGHTWIDGET_H
