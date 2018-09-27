#include "ExtendedRegistrationWidget.h"
#include "UserDetailsModel.h"

#include <Wt/WLineEdit>

ExtendedRegistrationWidget::ExtendedRegistrationWidget(Session& session, Wt::Auth::AuthWidget *authWidget)
        : Wt::Auth::RegistrationWidget(authWidget), session_(session) {
    setTemplateText(tr("extended.template.registration"));
    detailsModel_ = new UserDetailsModel(session_, this);

    updateView(detailsModel_);
}

Wt::WWidget *ExtendedRegistrationWidget::createFormWidget(Wt::WFormModel::Field field) {
    if (field == UserDetailsModel::FirstNameField) {
        return new Wt::WLineEdit();
    } else if (field == UserDetailsModel::LastNameField) {
        return new Wt::WLineEdit();
    } else {
        return Wt::Auth::RegistrationWidget::createFormWidget(field);
    }
}

bool ExtendedRegistrationWidget::validate() {
    bool result = Wt::Auth::RegistrationWidget::validate();

    updateModel(detailsModel_);
    if (!detailsModel_->validate())
        result = false;
    updateView(detailsModel_);

    return result;
}

void ExtendedRegistrationWidget::registerUserDetails(Wt::Auth::User& user) {
    detailsModel_->save(user);
}