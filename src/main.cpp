#include "dialogs/mainDialog.h"
#include <QApplication>
#include <boost/version.hpp>
#include <botan/botan.h>
#include "pbcVersion.h"

int main(int argc, char *argv[]) {
    Botan::LibraryInitializer botanInit;

    std::cout << "Playbook Creator Version: "
              << PBCVersion::getVersionString()
              << std::endl;
    std::cout << "built with Qt version: " << QT_VERSION_STR << std::endl;
    std::cout << "built with boost version: " << BOOST_LIB_VERSION << std::endl;
    std::cout << "built with botan verison: "
              << BOTAN_VERSION_MAJOR << "."
              << BOTAN_VERSION_MINOR << "."
              << BOTAN_VERSION_PATCH << std::endl;

    QApplication a(argc, argv);
    MainDialog w;
    w.show();
    return a.exec();
}
