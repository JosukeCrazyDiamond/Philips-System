#include "BridgeWidget.h"

#include <Wt/WLineEdit>
#include <Wt/WInPlaceEdit>
#include <Wt/WBreak>

using namespace Wt;

BridgeWidget::BridgeWidget(Session& session, Wt::WContainerWidget *parent)
        : WContainerWidget(parent), session_(session) {
    new WText("Bridge Name: ", this);
    NameField = new WInPlaceEdit("Bridge Name", this);
    NameField->setButtonsEnabled(false);
    new WBreak(this);

    new WText("Bridge Location: ", this);
    LocationField = new WInPlaceEdit("Bridge Location", this);
    LocationField->setButtonsEnabled(false);
    new WBreak(this);

    new WText("Bridge IP: ", this);
    IPField = new WInPlaceEdit("Bridge IP", this);
    IPField->setButtonsEnabled(false);
    new WBreak(this);

    new WText("Bridge Port: ", this);
    PortField = new WInPlaceEdit("Bridge Port", this);
    PortField->setButtonsEnabled(false);
    new WBreak(this);

    new WText("Bridge UserName: ", this);
    UserNameField = new WInPlaceEdit("Bridge UserName", this);
    UserNameField->setButtonsEnabled(false);
    new WBreak(this);

    DeleteButton = new WPushButton("Delete", this);
    DeleteButton->clicked().connect(this, &BridgeWidget::removeBridge);

    UpdateButton = new WPushButton("Update", this);
    UpdateButton->clicked().connect(this, &BridgeWidget::updateBridge);

    RegisterButton = new WPushButton("Create", this);
    RegisterButton->clicked().connect(this, &BridgeWidget::registerBridge);

    RefreshButton = new WPushButton("Connect To This Bridge", this);
    RefreshButton->clicked().connect(this, &BridgeWidget::select);

    AddGroupButton = new WPushButton("Add New Group", this);
    AddGroupButton->clicked().connect(this, &BridgeWidget::initNewGroupWidget);

    setStyleClass("bridgeWidget");
}

Bridge* BridgeWidget::getBridge() {
    return bridge;
}

void BridgeWidget::setBridge(Bridge * bridge) {
    this->bridge = bridge;
    NameField->setText(bridge->name);
    LocationField->setText(bridge->location);
    IPField->setText(bridge->ip);
    PortField->setText(bridge->port);
    UserNameField->setText(bridge->username);
}

void BridgeWidget::initNewGroupWidget() {
    GroupWidget* groupWidget = new GroupWidget(this);
    groupWidget->newGroupMode();
    addWidget(groupWidget);
    addWidget(new WBreak(this));
}

void BridgeWidget::registerBridge() {
    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(10 * 1024);
    client->done().connect(boost::bind(&BridgeWidget::handleBridgeTestHttpResponse, this, _1, _2));
    if (client->get("http://"+ IPField->text().toUTF8() +":"+ PortField->text().toUTF8() + "/api/"+ UserNameField->text().toUTF8()))
        WApplication::instance()->deferRendering();
    else {
        ((UserInterfaceWidget*)parent())->showDialog("error in the %URL When register Bridge");
    }
}

void BridgeWidget::updateBridge() {
    if(bridge != nullptr) {
        bridge->name = NameField->text().toUTF8();
        bridge->location = LocationField->text().toUTF8();
        bridge->ip = IPField->text().toUTF8();
        bridge->port = PortField->text().toUTF8();
        bridge->username = UserNameField->text().toUTF8();
        session_.updateBridge(bridge);
    }
}

void BridgeWidget::removeBridge() {
    session_.deleteBridge(bridge->id);
    ((WContainerWidget*)parent())->removeWidget(this);
}

void BridgeWidget::select() {
    if(bridge != nullptr) {
        for(LightWidget* lightWidget : lightWidgets) {
            removeChild(lightWidget);
        }
        for(GroupWidget* groupWidget : groupWidgets) {
            removeChild(groupWidget);
        }
        for(ScheduleWidget* scheduleWidget : scheduleWidgets) {
            removeChild(scheduleWidget);
        }
        retrieveLights();
    }
}

void BridgeWidget::retrieveLights() {
    if(bridge != nullptr) {
        lightWidgets.clear();
        Http::Client *client = new Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(10 * 1024);
        client->done().connect(boost::bind(&BridgeWidget::handleRetrieveLightsHttpResponse, this, _1, _2));
        if (client->get("http://"+ IPField->text().toUTF8() +":"+ PortField->text().toUTF8() + "/api/" + UserNameField->text().toUTF8() + "/lights/"))
            WApplication::instance()->deferRendering();
        else {
            ((UserInterfaceWidget*)parent())->showDialog("error in the %URL when retrieveLights");
        }
    }
}

void BridgeWidget::retrieveGroups() {
    if(bridge != nullptr) {
        groupWidgets.clear();
        Http::Client *client = new Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(10 * 1024);
        client->done().connect(boost::bind(&BridgeWidget::handleRetrieveGroupsHttpResponse, this, _1, _2));
        if (client->get("http://"+ IPField->text().toUTF8() +":"+ PortField->text().toUTF8() + "/api/" + UserNameField->text().toUTF8() + "/groups/"))
            WApplication::instance()->deferRendering();
        else {
            ((UserInterfaceWidget*)parent())->showDialog("error in the %URL when retrieveGroups");
        }
    }
}

void BridgeWidget::retrieveSchedules() {
    if(bridge != nullptr) {
        scheduleWidgets.clear();
        Http::Client *client = new Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(10 * 1024);
        client->done().connect(boost::bind(&BridgeWidget::handleRetrieveSchedulesHttpResponse, this, _1, _2));
        if (client->get("http://"+ IPField->text().toUTF8() +":"+ PortField->text().toUTF8() + "/api/" + UserNameField->text().toUTF8() + "/schedules/"))
            WApplication::instance()->deferRendering();
        else {
            ((UserInterfaceWidget*)parent())->showDialog("error in the %URL when retrieveSchedules");
        }
    }
}

void BridgeWidget::handleBridgeTestHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        bridge = new Bridge();
        bridge->name = NameField->text().toUTF8();
        bridge->location = LocationField->text().toUTF8();
        bridge->ip = IPField->text().toUTF8();
        bridge->port = PortField->text().toUTF8();
        bridge->username = UserNameField->text().toUTF8();
        session_.createBridge(bridge);
        ((UserInterfaceWidget*)parent())->refresh();
    }
}

void BridgeWidget::handleRetrieveLightsHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Object lightsJsonObj;
        Json::parse(response.body(), lightsJsonObj);
        std::set<string>::iterator it;
        set<string> ids = lightsJsonObj.names();
        for (it = ids.begin(); it != ids.end(); it++) {
            string id = *it;
            string::size_type sz;
            Json::Object lightJsonObj = lightsJsonObj.get(id);
            Light* light = new Light();
            light->setId(stoi(id, &sz));
            light->setName(lightJsonObj.get("name"));

            Json::Object lightStateJsonObj = lightJsonObj.get("state");
            LightState* state = new LightState();
            state->setBri(lightStateJsonObj.get("bri"));
            state->setCt(lightStateJsonObj.get("ct"));
            state->setEffect(lightStateJsonObj.get("effect"));
            state->setHue(lightStateJsonObj.get("hue"));
            state->setOn(lightStateJsonObj.get("on"));
            state->setSat(lightStateJsonObj.get("sat"));
            light->setLightState(state);

            LightWidget* lightWidget = new LightWidget(this);
            lightWidget->setLight(light);
            lightWidgets.push_back(lightWidget);
        }
        retrieveGroups();
    } else {
        ((UserInterfaceWidget*)parent())->showDialog("error processing RetrieveLightsHttpResponse");
    }
}

void BridgeWidget::handleRetrieveGroupsHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Object groupsJsonObj;
        Json::parse(response.body(), groupsJsonObj);
        std::set<string>::iterator it;
        set<string> ids = groupsJsonObj.names();
        for (it = ids.begin(); it != ids.end(); ++it) {
            string id = *it;
            string::size_type sz;
            Json::Object groupJsonObj = groupsJsonObj.get(id);
            Group* group = new Group();
            group->setId(stoi(id, &sz));
            group->setName(groupJsonObj.get("name"));
            Json::Array lightsJsonObj = groupJsonObj.get("lights");
            for(int j = 0; j < lightsJsonObj.size(); j++) {
                group->addLight(stoi(lightsJsonObj[j], &sz));
            }

            Json::Object lightStateJsonObj = groupJsonObj.get("action");
            LightState* state = new LightState();
            state->setBri(lightStateJsonObj.get("bri"));
            state->setCt(lightStateJsonObj.get("ct"));
            state->setEffect(lightStateJsonObj.get("effect"));
            state->setHue(lightStateJsonObj.get("hue"));
            state->setOn(lightStateJsonObj.get("on"));
            state->setSat(lightStateJsonObj.get("sat"));
            group->setLightState(state);

            GroupWidget* groupWidget = new GroupWidget(this);
            groupWidget->editGroupMode();
            groupWidget->setGroup(group);
            groupWidgets.push_back(groupWidget);
        }
        retrieveSchedules();
    } else {
        ((UserInterfaceWidget*)parent())->showDialog("error processing RetrieveGroupsHttpResponse");
    }
}

void BridgeWidget::handleRetrieveSchedulesHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Object schedulesJsonObj;
        Json::parse(response.body(), schedulesJsonObj);
        std::set<string>::iterator it;
        set<string> ids = schedulesJsonObj.names();
        for (it = ids.begin(); it != ids.end(); ++it) {
            string id = *it;
            string::size_type sz;
            Json::Object scheduleJsonObj = schedulesJsonObj.get(id);

            Json::Object scheduleCommandJsonObj = scheduleJsonObj.get("command");
            Json::Object lightStateJsonObj = scheduleCommandJsonObj.get("body");
            LightState* state = new LightState();
            Json::Value vBri = lightStateJsonObj.get("bri");
            if(!vBri.isNull()) {
                state->setBri(vBri);
            }
            Json::Value vCt = lightStateJsonObj.get("ct");
            if(!vCt.isNull()) {
                state->setCt(vCt);
            }
            Json::Value vEffect = lightStateJsonObj.get("effect");
            if(!vEffect.isNull()) {
                state->setEffect(vEffect);
            }
            Json::Value vHue = lightStateJsonObj.get("hue");
            if(!vHue.isNull()) {
                state->setHue(vHue);
            }
            Json::Value vOn = lightStateJsonObj.get("on");
            if(!vOn.isNull()) {
                state->setOn(vOn);
            }
            Json::Value vSat = lightStateJsonObj.get("sat");
            if(!vSat.isNull()) {
                state->setSat(vSat);
            }
            ScheduleWidget* scheduleWidget = new ScheduleWidget(scheduleCommandJsonObj.get("address"));
            scheduleWidget->editScheduleMode();
            Schedule* schedule = scheduleWidget->getSchedule();
            schedule->setId(stoi(id, &sz));
            schedule->setName(scheduleJsonObj.get("name"));
            schedule->setDescription(scheduleJsonObj.get("description"));
            schedule->setTime(scheduleJsonObj.get("time"));
            schedule->setMethod(scheduleCommandJsonObj.get("method"));
            schedule->setLightState(state);
            scheduleWidget->loadSchedule();
            scheduleWidgets.push_back(scheduleWidget);

            if(schedule->getTargetType() == "lights") {
                vector<LightWidget*>::iterator iterator1 = std::find_if (lightWidgets.begin(), lightWidgets.end(), boost::bind(&BridgeWidget::matchIDofLight, this, _1, schedule->getTargetId()));
                if(iterator1 != lightWidgets.end()) {
                    LightWidget* lightWidget = *iterator1;
                    lightWidget->addWidget(scheduleWidget);
                }
            } else if(schedule->getTargetType() == "groups") {
                vector<GroupWidget*>::iterator iterator1 = std::find_if (groupWidgets.begin(), groupWidgets.end(), boost::bind(&BridgeWidget::matchIDofGroup, this, _1, schedule->getTargetId()));
                if(iterator1 != groupWidgets.end()) {
                    GroupWidget* groupWidget = *iterator1;
                    groupWidget->addWidget(scheduleWidget);
                }
            } else{
                ((UserInterfaceWidget*)parent())->showDialog("error processing handleRetrieveSchedulesHttpResponse");
            }
        }
    } else {
        ((UserInterfaceWidget*)parent())->showDialog("error processing handleRetrieveSchedulesHttpResponse");
    }
}

bool BridgeWidget::matchIDofLight (LightWidget* lightWidget, int id) {
    return lightWidget->getLight()->getId() == id;
}

bool BridgeWidget::matchIDofGroup (GroupWidget* groupWidget, int id) {
    return groupWidget->getGroup()->getId() == id;
}

void BridgeWidget::newBridgeMode() {
    DeleteButton->show();
    UpdateButton->hide();
    RegisterButton->show();
    RefreshButton->hide();
    AddGroupButton->hide();
}
void BridgeWidget::editBridgeMode() {
    DeleteButton->show();
    UpdateButton->show();
    RegisterButton->hide();
    RefreshButton->show();
    AddGroupButton->show();
}