#include <Wt/WText>
#include <Wt/WStackedWidget>
#include <Wt/WApplication>
#include <Wt/Auth/AuthWidget>

#include "MainWidget.h"
#include "UserInterfaceWidget.h"
#include "ExtendedAuthWidget.h"

using namespace Wt;

MainWidget::MainWidget(WContainerWidget *parent): WContainerWidget(parent), session_(WApplication::instance()->appRoot() + "wt-philip-hue.db") {
    session_.login().changed().connect(this, &MainWidget::onAuthEvent);

    ExtendedAuthWidget *authWidget = new ExtendedAuthWidget(session_);

    authWidget->model()->addPasswordAuth(&Session::passwordAuth());
    authWidget->setRegistrationEnabled(true);

    WText *title = new WText(tr("wt-philip-hue.mainwidget.title"), this);
    addWidget(title);

    addWidget(authWidget);

    mainStack_ = new WStackedWidget();
    addWidget(mainStack_);

    WApplication::instance()->internalPathChanged()
            .connect(this, &MainWidget::handleInternalPath);

    authWidget->processEnvironment();
}

void MainWidget::onAuthEvent() {
    if (session_.login().loggedIn()) {
        handleInternalPath(WApplication::instance()->internalPath());
    } else {
        mainStack_->clear();
    }
}

void MainWidget::handleInternalPath(const std::string &internalPath) {
    if (session_.login().loggedIn()) {
        if (internalPath == "/ui")
            showUI();
        else
            WApplication::instance()->setInternalPath("/ui",  true);
    }
}

void MainWidget::showUI()
{
    if (!ui_) {
        delete ui_;
    }
    ui_ = new UserInterfaceWidget(session_, mainStack_);
    mainStack_->setCurrentWidget(ui_);
}