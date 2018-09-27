#ifndef WT_PHILIP_HUE_USERDETAILSMODEL_H
#define WT_PHILIP_HUE_USERDETAILSMODEL_H

#include <Wt/WFormModel>

class Session;
/*
 * user login form detail
 */
class UserDetailsModel : public Wt::WFormModel
{
public:
    static const Field FirstNameField;  //first name field
    static const Field LastNameField;   //lastname field
    static const Field LastLoginField;  //lastlogin time

    UserDetailsModel(Session& session, Wt::WObject *parent = 0);

    void save(const Wt::Auth::User& user);

private:
    Session& session_;
};

#endif //WT_PHILIP_HUE_USERDETAILSMODEL_H
