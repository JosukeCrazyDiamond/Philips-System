#include "ExtendedAuthWidget.h"
#include "ExtendedRegistrationWidget.h"
#include "Session.h"

ExtendedAuthWidget::ExtendedAuthWidget(Session& session)
        : Wt::Auth::AuthWidget(Session::auth(), session.users(), session.login()), session_(session) {
}

Wt::WWidget *ExtendedAuthWidget::createRegistrationView(const Wt::Auth::Identity& id)
{
    ExtendedRegistrationWidget *extendedRegistrationWidget = new ExtendedRegistrationWidget(session_, this);
    Wt::Auth::RegistrationModel *model = createRegistrationModel();

    if (id.isValid())
        model->registerIdentified(id);

    extendedRegistrationWidget->setModel(model);
    return extendedRegistrationWidget;
}