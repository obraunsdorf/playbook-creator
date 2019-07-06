/** @file main.cpp
    This file is part of Playbook Creator.

    Playbook Creator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Playbook Creator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2015 Oliver Braunsdorf

    @author Oliver Braunsdorf
    @brief This is the main file.
*/

#include "dialogs/mainDialog.h"
#include "util/pbcExceptions.h"
#include "pbcVersion.h"
#include <boost/version.hpp>
#include <botan/botan.h>
#include <QApplication>
#include <QMessageBox>
#include <iostream>

class PBCqApplication final : public QApplication {
 public:
    PBCqApplication(int& argc, char** argv) : QApplication(argc, argv) {}
    bool notify(QObject *receiver, QEvent *event) override {
        try {
            return QApplication::notify(receiver, event);
        } catch(std::exception &e) {
            QString errorString = QString::fromStdString(e.what());
            errorString += "\nTerminating playbook creator. Please open an bug report on "
                           "Github (https://github.com/obraunsdorf/playbook-creator/issues) "
                           "containing the following error log.";
            QMessageBox::critical(activeWindow(), "", errorString);
            QApplication::exit(1);
            return false;
        }
    }
};

/**
 * @brief the main function
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return Returns the return value of the Qt application execution.
 */
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

    PBCqApplication a(argc, argv);
    MainDialog w;
    w.show();
    return a.exec();
}
