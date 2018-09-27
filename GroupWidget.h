#ifndef WT_PHILIP_HUE_GROUPWIDGET_H
#define WT_PHILIP_HUE_GROUPWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WSlider>
#include <Wt/WRadioButton>
#include <Wt/WButtonGroup>
#include <Wt/Http/Client>
#include <Wt/Http/Message>
#include <Wt/WApplication>

#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "Group.h"

using namespace Wt;
using namespace std;
/*
 * Widget that display and allows interactions with Group functionality
 */
class GroupWidget : public WContainerWidget {
public:
    GroupWidget(WContainerWidget *parent = nullptr);

    /*
     * performs Call to Emulator to generate new Group
     */
    void createGroup();
    /*
     * performs Call to Emulator to delete an existing Group
     */
    void deleteGroup();
    /*
     * performs Call to Emulator to initialize a new Schedule Widget
     */
    void initNewScheduleWidget();
    /*
     * performs Call to Emulator to update existing Group attributes
     */
    void updateGroupAttr();
    /*
     * performs Call to Emulator to update existing Group lightstates
     */
    void updateGroupState();
    /*
     * performs Call to Emulator to update existing Group lightstates : ON/OFF
     */
    void turnGroupLightsOnOff();

    void showCreateButton();
    void hideCreateButton();

    /*
     * Set to New Group Mode
     */
    void newGroupMode();
    /*
     * Set to Existing Group Mode
     */
    void editGroupMode();

    Group* getGroup();
    void setGroup(Group * group);

private:
    WInPlaceEdit *NameField;        //Group Name Edit
    WInPlaceEdit *LightsField;      //Group Lights Edit

    WSlider *BriField;              //Group LightState Brightness Edit
    WSlider *HueField;              //Group LightState Hue Edit
    WSlider *SatField;              //Group LightState Saturation Edit
    WSlider *CtField;               //Group LightState Color Temperature Edit
    WButtonGroup *EffectField;      //Group LightState Color Effect Edit
    WSlider *TransitionTimeField;   //Group LightState Transition Time Edit

    WText *OnOffField;              //Group LightState ON/OFF Edit
    WPushButton *OnOffButton;

    WPushButton *UpdateAttrButton;  //Update Group Attr Button
    WPushButton *UpdateStateButton; //Update Group LightState Button
    WPushButton *CreateGroupButton; //Create Group Button
    WPushButton *DeleteGroupButton; //Delete Group Button
    WPushButton *AddScheduleButton; //Add a new Schedule for this Group Button

    Group *group;

    /*
     * helper methods to serialize group info
     */
    string serializeGroupAttr();
    string serializeGroupState();
    string serializeGroupLightsOnOff();
    Json::Array parseLightIds();

    /*
     * Handles Group Update Response
     */
    void handleHttpResponse(boost::system::error_code err, const Http::Message& response);
    /*
     * Handles Group Create Response
     */
    void handleGroupCreateHttpResponse(boost::system::error_code err, const Http::Message& response);
    /*
     * Handles Group Delete Response
     */
    void handleGroupDeleteHttpResponse(boost::system::error_code err, const Http::Message& response);
    /*
     * Handles Group Light ON/OFF Response
     */
    void handleGroupLightsOnOffHttpResponse(boost::system::error_code err, const Http::Message& response);
};

#endif //WT_PHILIP_HUE_GROUPWIDGET_H
