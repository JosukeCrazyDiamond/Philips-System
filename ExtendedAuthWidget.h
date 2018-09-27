#ifndef WT_PHILIP_HUE_EXTENDEDAUTHWIDGET_H
#define WT_PHILIP_HUE_EXTENDEDAUTHWIDGET_H

#include <Wt/Auth/AuthWidget>

class Session;

class ExtendedAuthWidget : public Wt::Auth::AuthWidget {
public:
    ExtendedAuthWidget(Session& session);

    /* We will use a custom registration view */
    virtual Wt::WWidget *createRegistrationView(const Wt::Auth::Identity& id);

private:
    Session& session_;
};

#endif //WT_PHILIP_HUE_EXTENDEDAUTHWIDGET_H
