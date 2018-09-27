#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WServer>
#include <Wt/WBootstrapTheme>

#include "Session.h"
#include "MainWidget.h"

#include <Wt/Auth/AuthModel>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

using namespace Wt;

class HueApplication : public WApplication {
public:
    HueApplication(const WEnvironment& env);
};

HueApplication::HueApplication(const WEnvironment& env) : WApplication(env) {
    setTitle("WT - Philip Hue - Connector");
    messageResourceBundle().use(appRoot() + "strings");
    messageResourceBundle().use(appRoot() + "templates");
    setTheme(new Wt::WBootstrapTheme(this));
    useStyleSheet("wt-philip-hue.css");
    useStyleSheet("wt.css");

    new MainWidget(root());
}

WApplication *createApplication(const WEnvironment& env) {
    return new HueApplication(env);
}

int main(int argc, char **argv) {
    try {
        Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);

        server.addEntryPoint(Wt::Application, createApplication);

        Session::configureAuth();

        server.run();
    } catch (Wt::WServer::Exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}