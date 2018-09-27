#ifndef WT_PHILIP_HUE_EXTENDEDREGISTRATIONWIDGET_H
#define WT_PHILIP_HUE_EXTENDEDREGISTRATIONWIDGET_H

#include <Wt/Auth/RegistrationWidget>

class Session;
class UserDetailsModel;

class ExtendedRegistrationWidget : public Wt::Auth::RegistrationWidget {
public:
    ExtendedRegistrationWidget(Session& session, Wt::Auth::AuthWidget *authWidget = 0);

    /* specialize to create user details fields */
    virtual Wt::WWidget *createFormWidget(Wt::WFormModel::Field field);

protected:
    /* specialize to also validate the user details */
    virtual bool validate();

    /* specialize to register user details */
    virtual void registerUserDetails(Wt::Auth::User& user);

private:
    Session& session_;

    UserDetailsModel *detailsModel_;
};

#endif //WT_PHILIP_HUE_EXTENDEDREGISTRATIONWIDGET_H
