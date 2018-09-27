#ifndef WT_PHILIP_HUE_BRIDGEWIDGET_H
#define WT_PHILIP_HUE_BRIDGEWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/Http/Client>
#include <Wt/Http/Message>
#include <Wt/WApplication>
#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>
#include <Wt/Json/Object>

#include "Bridge.h"
#include "User.h"
#include "Session.h"
#include "LightWidget.h"
#include "GroupWidget.h"
#include "ScheduleWidget.h"
#include "UserInterfaceWidget.h"

using namespace Wt;
using namespace std;
/*
 * Widget that display and allows interactions with Bridge functionality
 * This Widget contains Light Widgets and Group Widgets
 */
class BridgeWidget : public WContainerWidget {
public:
    BridgeWidget(Session& session, WContainerWidget *parent = nullptr);

    Bridge* getBridge();
    void setBridge(Bridge * bridge);
    /*
     * Register a new Bridge
     */
    void registerBridge();
    /*
     * Update of existing Bridge info
     */
    void updateBridge();
    /*
     * Remove existing Bridge
     */
    void removeBridge();

    /*
     * Pull all Existing Lights belongs to this bridge
     */
    void retrieveLights();
    /*
     * Pull all Existing Groups belongs to this bridge
     */
    void retrieveGroups();
    /*
     * Pull all Existing Schedules belongs to this bridge
     */
    void retrieveSchedules();
    /*
     * Connect to This Bridge
     */
    void select();

    /*
     * Initialize a Empty Group Widget
     */
    void initNewGroupWidget();

    /*
     * Set Widget Mode to new Bridge
     */
    void newBridgeMode();
    /*
     * Set Widget Mode to existing Bridge
     */
    void editBridgeMode();

private:
    WInPlaceEdit *NameField;                    //Bridge Name Edit
    WInPlaceEdit *LocationField;                //Bridge Location Edit
    WInPlaceEdit *IPField;                      //Bridge IP Edit
    WInPlaceEdit *PortField;                    //Bridge Port Edit
    WInPlaceEdit *UserNameField;                //Bridge UserName Edit
    WPushButton *DeleteButton;                  //Bridge Delete Button
    WPushButton *UpdateButton;                  //Bridge Update Button
    WPushButton *RegisterButton;                //Bridge Register Button
    WPushButton *RefreshButton;                 //Bridge Refresh Button
    WPushButton *AddGroupButton;                //Bridge Add A Group Button

    Session& session_;                          //Stores user login session info
    Bridge * bridge;                            //underneath Bridge Object
    vector<LightWidget *> lightWidgets;         //Vector of LightWidgets belongs to this Bridge
    vector<GroupWidget *> groupWidgets;         //Vector of GroupWidgets belongs to this Bridge
    vector<ScheduleWidget *> scheduleWidgets;   //Vector of ScheduleWidgets belongs to this Bridge

    /*
     * Handles Testing Bridge connection
     */
    void handleBridgeTestHttpResponse(boost::system::error_code err, const Http::Message& response);
    /*
     * Handles Pull Lights Http Call
     */
    void handleRetrieveLightsHttpResponse(boost::system::error_code err, const Http::Message& response);
    /*
     * Handles Pull Groups Http Call
     */
    void handleRetrieveGroupsHttpResponse(boost::system::error_code err, const Http::Message& response);
    /*
     * Handles Pull Schedules Http Call
     */
    void handleRetrieveSchedulesHttpResponse(boost::system::error_code err, const Http::Message& response);

    /*
     * See if give id matches any Light Widgets contained in this Bridge Widget
     */
    bool matchIDofLight (LightWidget* lightWidget, int id);
    /*
     * See if give id matches any Group Widgets contained in this Bridge Widget
     */
    bool matchIDofGroup (GroupWidget* groupWidget, int id);
};

#endif //WT_PHILIP_HUE_BRIDGEWIDGET_H
