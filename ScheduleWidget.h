#ifndef WT_PHILIP_HUE_SCHEDULEWIDGET_H
#define WT_PHILIP_HUE_SCHEDULEWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WTimeEdit>
#include <Wt/WDateEdit>
#include <Wt/Http/Client>
#include <Wt/Http/Message>
#include <Wt/WApplication>

#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>
#include <string>
#include "Schedule.h"

using namespace Wt;
using namespace std;
/*
 * Widget that display and allows interactions with Schedule functionality
 */
class ScheduleWidget : public WContainerWidget {
public:
    ScheduleWidget(const std::string& address, WContainerWidget *parent = nullptr);
    /*
     * performs Call to Emulator to update Schedule Attributes
     */
    void updateScheduleAttr();
    /*
     * performs Call to Emulator to create Schedule
     */
    void createSchedule();
    /*
     * performs Call to Emulator to delete Schedule
     */
    void deleteSchedule();

    Schedule* getSchedule();
    void setSchedule(Schedule * light);
    void loadSchedule();

    void showUpdateAttrButton();
    void hideUpdateAttrButton();
    void showDeleteButton();
    void hideDeleteButton();

    /*
     * Enter New Schedule Mode
     */
    void newScheduleMode();
    /*
     * Enter Edit Schedule Mode
     */
    void editScheduleMode();

private:
    WInPlaceEdit *NameField;            //Schedule Name Edit
    WInPlaceEdit *DescriptionField;     //Schedule Description Edit
    WDateEdit *DateField;               //Schedule Date Edit
    WTimeEdit *TimeField;               //Schedule Time Edit


    WSlider *CtField;                   //Schedule Color Temperature Edit
    WSlider *SatField;                  //Schedule Saturation Edit
    WButtonGroup *EffectField;          //Schedule Color Effect Edit
    WSlider *BriField;                  //Schedule Brightness Edit
    WSlider *HueField;                  //Schedule Hue Edit
    WSlider *TransitionTimeField;       //Schedule TransitionTime Edit
    WButtonGroup *OnOffField;           //Schedule ON/OFF Edit

    WPushButton *RegisterButton;        //Create new Schedule Button
    WPushButton *DeleteButton;          //Delete Schedule Button
    WPushButton *UpdateAttrButton;      //Update Schedule Attr Button

    Schedule *schedule;
    /*
     * helper method to serialize schedule attri
     */
    string serializeScheduleAttr();
    /*
     * Handle Schedule updates response
     */
    void handleHttpResponse(boost::system::error_code err, const Http::Message& response);
    /*
     * Handle Schedule Create response
     */
    void handleSchedulerCreateHttpResponse(boost::system::error_code err, const Http::Message& response);
    /*
     * Handle Schedule Delete response
     */
    void handleSchedulerDeleteHttpResponse(boost::system::error_code err, const Http::Message& response);
};

#endif //WT_PHILIP_HUE_SCHEDULEWIDGET_H
