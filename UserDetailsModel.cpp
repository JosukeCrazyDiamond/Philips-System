#include "UserDetailsModel.h"
#include "User.h"
#include "Session.h"

const Wt::WFormModel::Field UserDetailsModel::FirstNameField = "first-name";
const Wt::WFormModel::Field UserDetailsModel::LastNameField = "last-name";

UserDetailsModel::UserDetailsModel(Session& session, Wt::WObject *parent)
        : Wt::WFormModel(parent), session_(session) {
    addField(FirstNameField, Wt::WString::tr("first-name-info"));
    addField(LastNameField, Wt::WString::tr("last-name-info"));
}

void UserDetailsModel::save(const Wt::Auth::User& authUser) {
    Wt::Dbo::ptr<User> user = session_.registerUser(authUser);
    user.modify()->firstname = valueText(FirstNameField).toUTF8();
    user.modify()->lastname = valueText(LastNameField).toUTF8();
}