#ifndef WT_PHILIP_HUE_MAINWIDGET_H
#define WT_PHILIP_HUE_MAINWIDGET_H

#include <Wt/WContainerWidget>

#include "Session.h"

namespace Wt {
    class WStackedWidget;
    class WAnchor;
}

class UserInterfaceWidget;
class Session;

/*
 * This is Main Widget of Application
 */
class MainWidget : public Wt::WContainerWidget
{
public:
    MainWidget(Wt::WContainerWidget *parent = 0);

    /*
     * navigate to other page
     */
    void handleInternalPath(const std::string &internalPath);

private:
    Wt::WStackedWidget *mainStack_;
    UserInterfaceWidget *ui_;

    Session session_;
    /*
     * on login
     */
    void onAuthEvent();
    /*
     * display UserInterface after login
     */
    void showUI();
};

#endif //WT_PHILIP_HUE_MAINWIDGET_H
