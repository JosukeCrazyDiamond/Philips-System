#ifndef WT_PHILIP_HUE_USERINTERFACEWIDGET_H
#define WT_PHILIP_HUE_USERINTERFACEWIDGET_H

#include <vector>

#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WText>
#include <Wt/WGoogleMap>

#include "User.h"
#include "BridgeWidget.h"
#include "Bridge.h"

namespace dbo = Wt::Dbo;
using namespace std;
using namespace Wt;

class Session;
class BridgeWidget;
/*
 * user interface widget which contains all bridges belongs to current user
 */
class UserInterfaceWidget: public Wt::WContainerWidget {
public:
    UserInterfaceWidget(Session& session, Wt::WContainerWidget *parent = nullptr);
    /*
     * initialize a new BridgeWidget
     */
    void initNewBridgeWidget();
    /*
     * display error message dialog
     */
    void showDialog(const string& message);
    void dialogDone();
    /*
     * refresh user interface page
     */
    void refresh();

private:
    WText *title_;                          //interface title
    WPushButton *AddBridgeButton;           //add new bridge button
    vector<BridgeWidget *> bridgeWidgets;   //list of bridges belogns to this user

    WDialog* dialog_;                       //error message dialog

    Session& session_;                      //user login session
};

#endif //WT_PHILIP_HUE_USERINTERFACEWIDGET_H
