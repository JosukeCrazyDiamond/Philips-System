cmake_minimum_required(VERSION 3.8)
project(wt_philip_hue)

set(SOURCE_FILES main.cpp Session.cpp User.cpp UserInterfaceWidget.cpp MainWidget.cpp ExtendedRegistrationWidget.h UserDetailsModel.h UserDetailsModel.cpp ExtendedRegistrationWidget.cpp ExtendedAuthWidget.h ExtendedAuthWidget.cpp Bridge.h Bridge.cpp BridgeWidget.h BridgeWidget.cpp LightWidget.h LightWidget.cpp Light.h Light.cpp Group.h Group.cpp GroupWidget.h GroupWidget.cpp Schedule.h Schedule.cpp LightState.h LightState.cpp ScheduleWidget.h ScheduleWidget.cpp)

INCLUDE_DIRECTORIES(/usr/local/include)
LINK_DIRECTORIES(/usr/local/lib)
add_executable(wt_philip_hue ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(wt_philip_hue wthttp wt wtdbo wtdbosqlite3 boost_system-mt)
