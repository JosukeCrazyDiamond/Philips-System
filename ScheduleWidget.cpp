#include "ScheduleWidget.h"
#include "BridgeWidget.h"

#include <Wt/WText>
#include <Wt/WInPlaceEdit>
#include <Wt/WBreak>

ScheduleWidget::ScheduleWidget(const std::string& address, WContainerWidget *parent) : WContainerWidget(parent) {
    schedule = new Schedule(address);

    new WText("Light Name: ", this);
    NameField = new WInPlaceEdit("Light Name", this);
    NameField->setButtonsEnabled(false);
    new WBreak(this);
    DescriptionField = new WInPlaceEdit("Schedule Description", this);
    DescriptionField->setButtonsEnabled(false);
    new WBreak(this);

    new WText("Date: ", this);
    DateField = new WDateEdit(this);
    DateField->setFormat("yyyy-MM-d");
    new WText("Time: ", this);
    TimeField = new WTimeEdit(this);
    TimeField->setFormat("hh:mm:ss");
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
    TransitionTimeField->setMaximum(100);
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

    new WText("ON/OFF: ", this);
    OnOffField = new WButtonGroup(this);
    WRadioButton *button3 = new WRadioButton("ON");
    WRadioButton *button4 = new WRadioButton("OFF");
    addWidget(button3);
    OnOffField->addButton(button3, 0);
    addWidget(button4);
    OnOffField->addButton(button4, 1);
    OnOffField->setCheckedButton(OnOffField->button(0));
    new WBreak(this);

    UpdateAttrButton = new WPushButton("Update Light Attr", this);
    UpdateAttrButton->clicked().connect(this, &ScheduleWidget::updateScheduleAttr);

    RegisterButton = new WPushButton("Create", this);
    RegisterButton->clicked().connect(this, &ScheduleWidget::createSchedule);

    DeleteButton = new WPushButton("Delete", this);
    DeleteButton->clicked().connect(this, &ScheduleWidget::deleteSchedule);

    setStyleClass("scheduleWidget");
}

Schedule* ScheduleWidget::getSchedule() { return schedule; }

void ScheduleWidget::setSchedule(Schedule * schedule) {
    this->schedule = schedule;
    NameField->setText(schedule->getName());
    DescriptionField->setText(schedule->getDescription());
    CtField->setValue(schedule->getLightState()->getCt());
    SatField->setValue(schedule->getLightState()->getSat());
    EffectField->setCheckedButton(EffectField->button(schedule->getLightState()->getEffect() == "none" ? 0 : 1));
    BriField->setValue(schedule->getLightState()->getBri());
    HueField->setValue(schedule->getLightState()->getHue());
    OnOffField->setCheckedButton(OnOffField->button(schedule->getLightState()->isOn() ? 0 : 1));
}

void ScheduleWidget::loadSchedule() {
    NameField->setText(schedule->getName());
    DescriptionField->setText(schedule->getDescription());
    DateField->setDate(schedule->getTime().date());
    TimeField->setTime(schedule->getTime().time());
    CtField->setValue(schedule->getLightState()->getCt());
    SatField->setValue(schedule->getLightState()->getSat());
    EffectField->setCheckedButton(EffectField->button(schedule->getLightState()->getEffect() == "none" ? 0 : 1));
    BriField->setValue(schedule->getLightState()->getBri());
    HueField->setValue(schedule->getLightState()->getHue());
    OnOffField->setCheckedButton(OnOffField->button(schedule->getLightState()->isOn() ? 0 : 1));
}

void ScheduleWidget::createSchedule() {
    Wt::Http::Message message;

    message.addHeader("Content-type","application/json");
    message.addBodyText(serializeScheduleAttr());

    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(10 * 1024);
    client->done().connect(boost::bind(&ScheduleWidget::handleSchedulerCreateHttpResponse, this, _1, _2));
    if (client->post("http://"+ ((BridgeWidget*)((WContainerWidget*)parent()->parent()))->getBridge()->ip
                     + ":" + ((BridgeWidget*)((WContainerWidget*)parent()->parent()))->getBridge()->port
                     + "/api/"+ ((BridgeWidget*)((WContainerWidget*)parent()->parent()))->getBridge()->username +"/schedules/", message)) {
        WApplication::instance()->deferRendering();
    } else {
        ((UserInterfaceWidget*)(parent()->parent()->parent()))->showDialog("error in the %URL When createSchedule");
    }
}

void ScheduleWidget::deleteSchedule() {
    Wt::Http::Message message;

    message.addHeader("Content-type","application/json");

    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(10 * 1024);
    client->done().connect(boost::bind(&ScheduleWidget::handleSchedulerDeleteHttpResponse, this, _1, _2));
    if (client->deleteRequest("http://"+ ((BridgeWidget*)((WContainerWidget*)parent()->parent()))->getBridge()->ip
                              + ":" + ((BridgeWidget*)((WContainerWidget*)parent()->parent()))->getBridge()->port
                              + "/api/"+ ((BridgeWidget*)((WContainerWidget*)parent()->parent()))->getBridge()->username +"/schedules/" + to_string(schedule->getId()), message)) {
        WApplication::instance()->deferRendering();
    } else {
        ((UserInterfaceWidget*)(parent()->parent()->parent()))->showDialog("error in the %URL When deleteSchedule");
    }
}

void ScheduleWidget::updateScheduleAttr() {
    if(schedule != nullptr) {

        Wt::Http::Message message;

        message.addHeader("Content-type","application/json");
        message.addBodyText(serializeScheduleAttr());

        Http::Client *client = new Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(10 * 1024);
        client->done().connect(boost::bind(&ScheduleWidget::handleHttpResponse, this, _1, _2));
        if (client->put("http://"+ ((BridgeWidget*)((WContainerWidget*)parent()->parent()))->getBridge()->ip
                        + ":" + ((BridgeWidget*)((WContainerWidget*)parent()->parent()))->getBridge()->port
                        + "/api/"+ ((BridgeWidget*)((WContainerWidget*)parent()->parent()))->getBridge()->username +"/schedules/" + to_string(schedule->getId()), message)) {
            WApplication::instance()->deferRendering();
        }
        else {
            ((UserInterfaceWidget*)(parent()->parent()->parent()))->showDialog("error in the %URL When updateScheduleAttr");
        }
    }
}

std::string ScheduleWidget::serializeScheduleAttr() {
    Json::Value vName(NameField->text().toUTF8());
    Json::Value vDescription(DescriptionField->text().toUTF8());
    WDateTime *dateTime = new WDateTime();
    dateTime->setDate(DateField->date());
    dateTime->setTime(TimeField->time());
    Json::Value vTime(dateTime->toString("yyyy-MM-d'T'hh:mm:ss"));

    Json::Value vOn(OnOffField->checkedId() == 0);
    Json::Value vBri(BriField->value());
    Json::Value vHue(HueField->value());
    Json::Value vCt(CtField->value());
    Json::Value vSat(SatField->value());
    Json::Value vTransition(TransitionTimeField->value());
    Json::Value vEffect(EffectField->checkedId() == 0 ? "none" : "colorloop");

    Json::Object scheduleAttr;
    scheduleAttr["name"] = vName;
    scheduleAttr["description"] = vDescription;
    scheduleAttr["time"] = vTime;

    Json::Value vAddress(schedule->getAddress());
    Json::Value vMethod("PUT");
    Json::Object scheduleState;
    scheduleState["on"] = vOn;
    scheduleState["bri"] = vBri;
    scheduleState["hue"] = vHue;
    scheduleState["ct"] = vCt;
    scheduleState["sat"] = vSat;
    scheduleState["transitiontime"] = vTransition;
    scheduleState["effect"] = vEffect;

    Json::Object scheduleCommand;
    scheduleCommand["address"] = vAddress;
    scheduleCommand["method"] = vMethod;
    scheduleCommand["body"] = scheduleState;

    scheduleAttr["command"] = scheduleCommand;

    return Json::serialize(scheduleAttr);
}

void ScheduleWidget::handleHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Array responseArray;
        Json::parse(response.body(), responseArray);
        Json::Object contentJsonObj = responseArray[0];
        if(!contentJsonObj.get("success")) {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("Error Updating Schedule");
        }
    } else {
        ((UserInterfaceWidget*)(parent()->parent()->parent()))->showDialog("error When handleHttpResponse");
    }

}

void ScheduleWidget::handleSchedulerCreateHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Array createReturnJsonObj;
        Json::parse(response.body(), createReturnJsonObj);
        Json::Object contentJsonObj = createReturnJsonObj[0];
        Json::Object successJsonObj = contentJsonObj.get("success");
        if(successJsonObj.empty()) {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("Error Creating Schedule");
        }else {
            string::size_type sz;
            schedule->setId(stoi(successJsonObj.get("id"), &sz));
            this->editScheduleMode();
        }
    } else {
        ((UserInterfaceWidget*)(parent()->parent()->parent()))->showDialog("error When handleSchedulerCreateHttpResponse");
    }
}

void ScheduleWidget::handleSchedulerDeleteHttpResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        Json::Object responseArray;
        Json::parse(response.body(), responseArray);
        Json::Value successJsonObj = responseArray.get("success");
        if(successJsonObj.isNull()) {
            ((UserInterfaceWidget*)(parent()->parent()))->showDialog("Error Deleting Group");
        } else {
            ((WContainerWidget*)parent())->removeWidget(this);
        }
    } else {
        ((UserInterfaceWidget*)(parent()->parent()->parent()))->showDialog("error When handleSchedulerDeleteHttpResponse");
    }
}

void ScheduleWidget::showUpdateAttrButton() {
    UpdateAttrButton->show();
}
void ScheduleWidget::hideUpdateAttrButton() {
    UpdateAttrButton->hide();
}
void ScheduleWidget::showDeleteButton() {
    DeleteButton->show();
}
void ScheduleWidget::hideDeleteButton() {
    DeleteButton->hide();
}

void ScheduleWidget::newScheduleMode() {
    RegisterButton->show();
    DeleteButton->hide();
    UpdateAttrButton->hide();
}
void ScheduleWidget::editScheduleMode() {
    RegisterButton->hide();
    DeleteButton->show();
    UpdateAttrButton->show();
}