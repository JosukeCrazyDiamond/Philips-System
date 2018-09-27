#include "UserInterfaceWidget.h"

#include "Session.h"
#include <Wt/WText>
#include <Wt/WBreak>
#include "Bridge.h"

using namespace Wt;

UserInterfaceWidget::UserInterfaceWidget(Session& session, Wt::WContainerWidget *parent)
        : WContainerWidget(parent), session_(session) {
    refresh();
}

void UserInterfaceWidget::initNewBridgeWidget() {
    BridgeWidget* bridgeWidget = new BridgeWidget(session_, this);
    bridgeWidget->newBridgeMode();
    addWidget(bridgeWidget);
    addWidget(new WBreak(this));
}

void UserInterfaceWidget::showDialog(const string& message) {
    dialog_ = new WDialog("Information");
    dialog_->setStyleClass("infoDialog");
    dialog_->contents()->addWidget(new WText(message));
    dialog_->contents()->addWidget(new WBreak());

    Wt::WPushButton *ok = new WPushButton("Ok");
    dialog_->contents()->addWidget(ok);
    ok->clicked().connect(dialog_, &WDialog::accept);

    dialog_->finished().connect(this, &UserInterfaceWidget::dialogDone);
    this->addWidget(dialog_);
    dialog_->show();
}
void UserInterfaceWidget::dialogDone() {
    this->removeChild(dialog_);
}

void UserInterfaceWidget::refresh() {
    bridgeWidgets.clear();
    this->clear();
    WGoogleMap* map = new WGoogleMap(WGoogleMap::ApiVersion::Version3, this);
    map->setMapTypeControl(WGoogleMap::MapTypeControl::DefaultControl);
    map->enableScrollWheelZoom();
    map->panTo(WGoogleMap::Coordinate(43.0095597,-81.2714285));
    std::vector<Bridge *> bridges = session_.bridges();
    for (Bridge * bridge : bridges) {
        BridgeWidget *bridgeWidget = new BridgeWidget(session_, this);
        bridgeWidget->setBridge(bridge);
        bridgeWidget->editBridgeMode();
        bridgeWidgets.push_back(bridgeWidget);
    }
    setContentAlignment(AlignCenter);
    AddBridgeButton = new WPushButton("Add New Bridge", this);
    AddBridgeButton->clicked().connect(this,&UserInterfaceWidget::initNewBridgeWidget);
}