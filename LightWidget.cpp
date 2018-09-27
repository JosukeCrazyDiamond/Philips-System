#include "LightWidget.h"
#include "BridgeWidget.h"

#include <Wt/WText>
#include <Wt/WInPlaceEdit>
#include <Wt/WBreak>

LightWidget::LightWidget(WContainerWidget *parent) : WContainerWidget(parent) {
    new WText("Light ID: ", this);
    IdField = new WText("Light ID", this);
    new WBreak(this);

    new WText("Light Name: ", this);
    NameField = new WInPlaceEdit("Light Name", this);
    NameField->setButtonsEnabled(false);
    UpdateAttrButton = new WPushButton("Change Name", this);
    UpdateAttrButton->clicked().connect(this, &LightWidget::updateLightAttr);
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

    new WText("Color Transition Time: ", this);
    TransitionTimeField = new WSlider(Orientation::Horizontal, this);
    TransitionTimeField->setMinimum(0);
    TransitionTimeField->setMaximum(10000);
    TransitionTimeField->resize(100, 25);
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

    OnOffButton = new WPushButton("ON/OFF", this);
    OnOffButton->clicked().connect(this, &LightWidget::turnLightOnOff);

    OnOffField = new WText("Default ON/OFF", this);

    UpdateStateButton = new WPushButton("Update Light State", this);
    UpdateStateButton->clicked().connect(this, &LightWidget::updateLightState);

    AddScheduleButton = new WPushButton("Create Light Schedule", this);
    AddScheduleButton->clicked().connect(this, &LightWidget::initNewScheduleWidget);

    setStyleClass("lightWidget");
}

Light* LightWidget::getLight() { return light; }

void LightWidget::setLight(Light * light) {
    this->light = light;
    IdField->setText(to_string(light->getId()));
    NameField->setText(light->getName());
    CtField->setValue(light->getLightState()->getCt());
    SatField->setValue(light->getLightState()->getSat());
    EffectField->setCheckedButton(EffectField->button(light->getLightState()->getEffect() == "none" ? 0 : 1));
    BriField->setValue(light->getLightState()->getBri());
    HueField->setValue(light->getLightState()->getHue());
    string onOff = (light->getLightState()->isOn()) ? "ON" : "OFF";
    OnOffField->setText("Light's: " + onOff);
}

void LightWidget::initNewScheduleWidget() {
    ScheduleWidget* scheduleWidget = new ScheduleWidget("/api/"+ ((BridgeWidget*)parent())->getBridge()->username +"/lights/"+ to_string(light->getId()) + "/state", this);
    scheduleWidget->newScheduleMode();
    addWidget(scheduleWidget);
    addWidget(new WBreak(this));
}

void LightWidget::updateLightAttr() {
    if(light != nullptr) {

        Wt::Http::Message message;

        message.addHeader("Content-type","application/json");
        message.addBodyText(serializeLightAttr());

        Http::Client *client = new Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(10 * 1024);
        client->done().connect(boost::bind(&LightWidget::handleHttpResponse, this, _1, _2));
        if (client->put("http://"+ ((BridgeWidget*)parent())->getBridge()->ip
                        + ":" + ((BridgeWidget*)parent())->getBridge()->port
                        + "/api/"+ ((BridgeWidget*)parent())->getBridge()->username +"/lights/" + to_string(light->getId()), message)) {
            WApplication::instance()->deferRendering();
        }
        else {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error in the %URL When updateLightAttr");
        }
    }
}

void LightWidget::updateLightState() {
    if(light != nullptr) {

        Wt::Http::Message message;

        message.addHeader("Content-type","application/json");
        message.addBodyText(serializeLightState());

        Http::Client *client = new Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(10 * 1024);
        client->done().connect(boost::bind(&LightWidget::handleHttpResponse, this, _1, _2));
        if (client->put("http://"+ ((BridgeWidget*)parent())->getBridge()->ip
                        + ":" + ((BridgeWidget*)parent())->getBridge()->port
                        + "/api/"+ ((BridgeWidget*)parent())->getBridge()->username +"/lights/" + to_string(light->getId()) + "/state", message)) {
            WApplication::instance()->deferRendering();
        }
        else {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error in the %URL When updateLightState");
        }
    }
}

void LightWidget::turnLightOnOff() {
    if(light != nullptr) {

        Wt::Http::Message message;

        message.addHeader("Content-type","application/json");
        message.addBodyText(serializeLightOnOff());

        Http::Client *client = new Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(10 * 1024);
        client->done().connect(boost::bind(&LightWidget::handleOnOffHttpResponse, this, _1, _2));
        if (client->put("http://"+ ((BridgeWidget*)parent())->getBridge()->ip
                        + ":" + ((BridgeWidget*)parent())->getBridge()->port
                        + "/api/"+ ((BridgeWidget*)parent())->getBridge()->username +"/lights/" + to_string(light->getId()) + "/state", message)) {
            WApplication::instance()->deferRendering();
        }
        else {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error in the %URL When updateLightState");
        }
    }
}

string LightWidget::serializeLightAttr() {
    Json::Value vName(NameField->text().toUTF8());
    Json::Object lightAttr;
    lightAttr["name"] = vName;

    return Json::serialize(lightAttr);
}

string LightWidget::serializeLightState() {
    Json::Value vBri(BriField->value());
    Json::Value vHue(HueField->value());
    Json::Value vCt(CtField->value());
    Json::Value vSat(SatField->value());
    Json::Value vTransition(TransitionTimeField->value());
    Json::Value vEffect(EffectField->checkedId() == 0 ? "none" : "colorloop");
    Json::Object lightState;
    lightState["on"] = true;
    lightState["bri"] = vBri;
    lightState["hue"] = vHue;
    lightState["ct"] = vCt;
    lightState["sat"] = vSat;
    lightState["transitiontime"] = vTransition;
    lightState["effect"] = vEffect;

    return Json::serialize(lightState);
}

string LightWidget::serializeLightOnOff() {
    Json::Object lightState;
    lightState["on"] = !light->getLightState()->isOn();
    return Json::serialize(lightState);
}

void LightWidget::handleHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Array responseArray;
        Json::parse(response.body(), responseArray);
        Json::Object contentJsonObj = responseArray[0];
        if(!contentJsonObj.get("success")) {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error Updating Light");
        }
    } else {
        ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error When handleHttpResponse");
    }
}

void LightWidget::handleOnOffHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Array responseArray;
        Json::parse(response.body(), responseArray);
        Json::Object contentJsonObj = responseArray[0];
        Json::Value successJsonObj = contentJsonObj.get("success");
        if(successJsonObj.isNull()) {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error Turning Light ON/OFF");
        }else {
            LightState* lightState = light->getLightState();
            lightState->setOn(!lightState->isOn());
            string onOff = (lightState->isOn()) ? "ON" : "OFF";
            OnOffField->setText("Light's: " + onOff);
        }
    } else {
        ((UserInterfaceWidget*)(parent()->parent()))->showDialog("error When handleOnOffHttpResponse");
    }
}