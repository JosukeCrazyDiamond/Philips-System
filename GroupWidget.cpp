#include "GroupWidget.h"
#include "BridgeWidget.h"

#include <Wt/WText>
#include <Wt/WInPlaceEdit>
#include <Wt/WBreak>

GroupWidget::GroupWidget(WContainerWidget *parent) : WContainerWidget(parent) {
    new WText("Group Name: ", this);
    NameField = new WInPlaceEdit("Group Name", this);
    NameField->setButtonsEnabled(false);
    new WBreak(this);
    new WText("Group Lights: ", this);
    LightsField = new WInPlaceEdit("Light IDs", this);
    LightsField->setButtonsEnabled(false);
    UpdateAttrButton = new WPushButton("Update Group Attr", this);
    UpdateAttrButton->clicked().connect(this, &GroupWidget::updateGroupAttr);
    new WBreak(this);

    new WText("Brightness: ", this);
    BriField = new WSlider(Orientation::Horizontal, this);
    BriField->setMinimum(0);
    BriField->setMaximum(254);
    BriField->resize(100, 25);
    new WBreak(this);

    new WText("Hue: ", this);
    HueField = new WSlider(Orientation::Horizontal, this);
    HueField->setMinimum(0);
    HueField->setMaximum(65535);
    HueField->resize(100, 25);
    new WBreak(this);

    new WText("Saturation: ", this);
    SatField = new WSlider(Orientation::Horizontal, this);
    SatField->setMinimum(0);
    SatField->setMaximum(254);
    SatField->resize(100, 25);
    new WBreak(this);

    new WText("Color Temperature: ", this);
    CtField = new WSlider(Orientation::Horizontal, this);
    CtField->setMinimum(153);
    CtField->setMaximum(500);
    CtField->resize(100, 25);
    new WBreak(this);

    new WText("Color Mode: ", this);
    EffectField = new WButtonGroup(this);
    WRadioButton *button1 = new WRadioButton("None");
    WRadioButton *button2 = new WRadioButton("Color Loop");
    addWidget(button1);
    EffectField->addButton(button1, 0);
    addWidget(button2);
    EffectField->addButton(button2, 1);
    EffectField->setCheckedButton(EffectField->button(0));
    new WBreak(this);

    new WText("Color Transition Time: ", this);
    TransitionTimeField = new WSlider(Orientation::Horizontal, this);
    TransitionTimeField->setMinimum(0);
    TransitionTimeField->setMaximum(100);
    TransitionTimeField->resize(100, 25);
    new WBreak(this);

    UpdateStateButton = new WPushButton("Update Group State", this);
    UpdateStateButton->clicked().connect(this, &GroupWidget::updateGroupState);
    new WBreak(this);

    OnOffButton = new WPushButton("ON/OFF", this);
    OnOffButton->clicked().connect(this, &GroupWidget::turnGroupLightsOnOff);
    OnOffField = new WText("Default ON/OFF", this);

    CreateGroupButton = new WPushButton("Create this Group", this);
    CreateGroupButton->clicked().connect(this, &GroupWidget::createGroup);

    DeleteGroupButton = new WPushButton("Delete this Group", this);
    DeleteGroupButton->clicked().connect(this, &GroupWidget::deleteGroup);

    AddScheduleButton = new WPushButton("Create Group Schedule", this);
    AddScheduleButton->clicked().connect(this, &GroupWidget::initNewScheduleWidget);

    setStyleClass("groupWidget");
}

Group* GroupWidget::getGroup() { return group; }

void GroupWidget::setGroup(Group *group) {
    this->group = group;
    NameField->setText(group->getName());
    string lights;
    for(int i = 0; i < group->getLights().size() - 1; i++) {
        lights += to_string(group->getLights()[i]) + ",";
    }
    lights += to_string(group->getLights()[group->getLights().size() - 1]);
    LightsField->setText(lights);
    CtField->setValue(group->getLightState()->getCt());
    SatField->setValue(group->getLightState()->getSat());
    EffectField->setCheckedButton(EffectField->button(group->getLightState()->getEffect() == "none" ? 0 : 1));
    BriField->setValue(group->getLightState()->getBri());
    HueField->setValue(group->getLightState()->getHue());
    string onOff = (group->getLightState()->isOn()) ? "ON" : "OFF";
    OnOffField->setText("Light's: " + onOff);
}

void GroupWidget::createGroup() {
    Wt::Http::Message message;

    message.addHeader("Content-type","application/json");
    message.addBodyText(serializeGroupAttr());

    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(10 * 1024);
    client->done().connect(boost::bind(&GroupWidget::handleGroupCreateHttpResponse, this, _1, _2));
    if (client->post("http://"+ ((BridgeWidget*)parent())->getBridge()->ip
                    + ":" + ((BridgeWidget*)parent())->getBridge()->port
                    + "/api/"+ ((BridgeWidget*)parent())->getBridge()->username +"/groups", message)) {
        WApplication::instance()->deferRendering();
    } else {
        ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error in the %URL When createGroup");
    }
}

void GroupWidget::deleteGroup() {
    Wt::Http::Message message;

    message.addHeader("Content-type","application/json");

    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(10 * 1024);
    client->done().connect(boost::bind(&GroupWidget::handleGroupDeleteHttpResponse, this, _1, _2));
    if (client->deleteRequest("http://"+ ((BridgeWidget*)parent())->getBridge()->ip
                     + ":" + ((BridgeWidget*)parent())->getBridge()->port
                     + "/api/"+ ((BridgeWidget*)parent())->getBridge()->username +"/groups/" + to_string(group->getId()), message)) {
        WApplication::instance()->deferRendering();
    } else {
        ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error in the %URL When deleteGroup");
    }
}

void GroupWidget::initNewScheduleWidget() {
    ScheduleWidget* scheduleWidget = new ScheduleWidget("/api/"+ ((BridgeWidget*)parent())->getBridge()->username +"/groups/"+ to_string(group->getId()) + "/action", this);
    scheduleWidget->newScheduleMode();
    addWidget(scheduleWidget);
    addWidget(new WBreak(this));
}

void GroupWidget::updateGroupAttr() {
    if(group != nullptr) {
        Wt::Http::Message message;

        message.addHeader("Content-type","application/json");
        message.addBodyText(serializeGroupAttr());

        Http::Client *client = new Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(10 * 1024);
        client->done().connect(boost::bind(&GroupWidget::handleHttpResponse, this, _1, _2));
        if (client->put("http://"+ ((BridgeWidget*)parent())->getBridge()->ip
                        + ":" + ((BridgeWidget*)parent())->getBridge()->port
                        + "/api/"+ ((BridgeWidget*)parent())->getBridge()->username +"/groups/" + to_string(group->getId()), message)) {
            WApplication::instance()->deferRendering();
        } else {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error in the %URL When updateGroupAttr");
        }
    }
}

void GroupWidget::updateGroupState() {
    if(group != nullptr) {
        Wt::Http::Message message;

        message.addHeader("Content-type","application/json");
        message.addBodyText(serializeGroupState());

        Http::Client *client = new Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(10 * 1024);
        client->done().connect(boost::bind(&GroupWidget::handleHttpResponse, this, _1, _2));
        if (client->put("http://"+ ((BridgeWidget*)parent())->getBridge()->ip
                        + ":" + ((BridgeWidget*)parent())->getBridge()->port
                        + "/api/"+ ((BridgeWidget*)parent())->getBridge()->username +"/groups/" + to_string(group->getId()) + "/action", message)) {
            WApplication::instance()->deferRendering();
        } else {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error in the %URL When updateGroupState");
        }
    }
}

void GroupWidget::turnGroupLightsOnOff() {
    if(group != nullptr) {

        Wt::Http::Message message;

        message.addHeader("Content-type","application/json");
        message.addBodyText(serializeGroupLightsOnOff());

        Http::Client *client = new Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(10 * 1024);
        client->done().connect(boost::bind(&GroupWidget::handleGroupLightsOnOffHttpResponse, this, _1, _2));
        if (client->put("http://"+ ((BridgeWidget*)parent())->getBridge()->ip
                        + ":" + ((BridgeWidget*)parent())->getBridge()->port
                        + "/api/"+ ((BridgeWidget*)parent())->getBridge()->username +"/groups/" + to_string(group->getId()) + "/action", message)) {
            WApplication::instance()->deferRendering();
        }
        else {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error in the %URL When updateLightState");
        }
    }
}

string GroupWidget::serializeGroupAttr() {
    Json::Value vName(NameField->text().toUTF8());
    Json::Object groupAttr;
    groupAttr["name"] = vName;
    groupAttr["lights"] = parseLightIds();

    return Json::serialize(groupAttr);
}

string GroupWidget::serializeGroupState() {
    Json::Value vBri(BriField->value());
    Json::Value vHue(HueField->value());
    Json::Value vSat(SatField->value());
    Json::Value vCt(CtField->value());
    Json::Value vTransition(TransitionTimeField->value());
    Json::Value vEffect(EffectField->checkedId() == 0 ? "none" : "colorloop");
    Json::Object groupState;
    groupState["on"] = true;
    groupState["bri"] = vBri;
    groupState["hue"] = vHue;
    groupState["sat"] = vSat;
    groupState["ct"] = vCt;
    groupState["transitiontime"] = vTransition;
    groupState["effect"] = vEffect;

    return Json::serialize(groupState);
}

string GroupWidget::serializeGroupLightsOnOff() {
    Json::Object lightState;
    lightState["on"] = !group->getLightState()->isOn();
    return Json::serialize(lightState);
}

Json::Array GroupWidget::parseLightIds() {
    string lights = LightsField->text().toUTF8();

    Json::Array vLights;
    stringstream ss(lights);
    int i;
    while (ss >> i) {
        vLights.push_back(i);
        if (ss.peek() == ',' || ss.peek() == ' ')
            ss.ignore();
    }
    return vLights;
}


void GroupWidget::handleHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Array responseArray;
        Json::parse(response.body(), responseArray);
        Json::Object contentJsonObj = responseArray[0];
        Json::Object successJsonObj = contentJsonObj.get("success");
        if(successJsonObj.empty()) {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("Error Updating Group");
        }
    } else {
        ((UserInterfaceWidget*)(parent()->parent()))->showDialog("Error When handleHttpResponse");
    }
}

void GroupWidget::handleGroupCreateHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Array responseArray;
        Json::parse(response.body(), responseArray);
        Json::Object contentJsonObj = responseArray[0];
        Json::Object successJsonObj = contentJsonObj.get("success");
        if(successJsonObj.empty()) {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("Error Creating Group");
        } else {
            group = new Group();
            LightState* lightState = new LightState();
            group->setLightState(lightState);
            string idRaw = successJsonObj.get("id");
            istringstream ss(idRaw);
            string token;
            vector<string> vec;
            while(getline(ss, token, '/')) {
                vec.push_back(token);
            }
            string::size_type sz;
            group->setId(stoi(vec[2], &sz));
            this->editGroupMode();
        }
    } else {
        ((UserInterfaceWidget*)(parent()->parent()))->showDialog("Error When handleGroupCreateHttpResponse");
    }
}

void GroupWidget::handleGroupDeleteHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Array responseArray;
        Json::parse(response.body(), responseArray);
        Json::Object contentJsonObj = responseArray[0];
        Json::Value successJsonObj = contentJsonObj.get("success");
        if(successJsonObj.isNull()) {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("Error Deleting Group");
        } else {
            ((WContainerWidget*)parent())->removeWidget(this);
        }
    } else {
        ((UserInterfaceWidget*)(parent()->parent()))->showDialog("Error When handleGroupDeleteHttpResponse");
    }
}

void GroupWidget::handleGroupLightsOnOffHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Array responseArray;
        Json::parse(response.body(), responseArray);
        Json::Object contentJsonObj = responseArray[0];
        Json::Value successJsonObj = contentJsonObj.get("success");
        if(successJsonObj.isNull()) {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error Turning Light ON/OFF");
        }else {
            LightState* lightState = group->getLightState();
            lightState->setOn(!lightState->isOn());
            string onOff = (lightState->isOn()) ? "ON" : "OFF";
            OnOffField->setText("Light's: " + onOff);
        }
    } else {
        ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error When handleOnOffHttpResponse");
    }
}

void GroupWidget::showCreateButton() {
    CreateGroupButton->show();
}
void GroupWidget::hideCreateButton() {
    CreateGroupButton->hide();
}

void GroupWidget::newGroupMode() {
    UpdateAttrButton->hide();
    UpdateStateButton->hide();
    CreateGroupButton->show();
    DeleteGroupButton->hide();
    AddScheduleButton->hide();
    OnOffButton->hide();
    OnOffField->hide();
}
void GroupWidget::editGroupMode() {
    UpdateAttrButton->show();
    UpdateStateButton->show();
    CreateGroupButton->hide();
    DeleteGroupButton->show();
    AddScheduleButton->show();
    OnOffButton->show();
    OnOffField->show();
}