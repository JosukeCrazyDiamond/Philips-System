#include "Schedule.h"

Schedule::Schedule(const std::string& address) : address(address){
    setTarget();
};
Schedule::~Schedule(){};

int Schedule::getId() { return id; }
void Schedule::setId(int id) {
    this->id = id;
}

WDateTime Schedule::getTime() { return time; }
void Schedule::setTime(const std::string& timeStr) {
    time = WDateTime::fromString(timeStr, "yyyy-MM-d'T'hh:mm:ss");
    cout<< time.toString() << endl;
}

std::string Schedule::getDescription() {return description; }
void Schedule::setDescription(const std::string& description) {
    this->description = description;
}

string Schedule::getName() { return name; }
void Schedule::setName(const string& name) {
    this->name = name;
}

std::string Schedule::getAddress() { return address; }
void Schedule::setAddress(const std::string& address) {
    this->address = address;
}

std::string Schedule::getTargetType() { return targetType; }
int Schedule::getTargetId() { return targetId; }
void Schedule::setTarget() {
    istringstream ss(address);
    string token;
    vector<string> vec;
    while(getline(ss, token, '/')) {
        vec.push_back(token);
    }
    this->targetType = vec[3];
    string::size_type sz;
    this->targetId = stoi(vec[4], &sz);
}

std::string Schedule::getMethod() { return method; }
void Schedule::setMethod(const std::string& method) {
    this->method = method;
}

LightState* Schedule::getLightState() { return lightState; }
void Schedule::setLightState(LightState* lightState) {
    this->lightState = lightState;
}