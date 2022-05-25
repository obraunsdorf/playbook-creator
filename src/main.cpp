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
#include "../updater/pbc_updater_bindings.h"
#include <botan/version.h>
#include <boost/version.hpp>
#include <QApplication>
#include <QMessageBox>
#include <iostream>

/**
 * @brief the main function
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return Returns the return value of the Qt application execution.
 */
int main(int argc, char *argv[]) {
    std::cout << "Playbook Creator Version: "
              << PBCVersion::getVersionString()
              << std::endl;
    std::cout << "built with Qt version: " << QT_VERSION_STR << std::endl;
    std::cout << "built with boost version: " << BOOST_LIB_VERSION << std::endl;
    std::cout << "built with botan verison: "
              << BOTAN_VERSION_MAJOR << "."
              << BOTAN_VERSION_MINOR << "."
              << BOTAN_VERSION_PATCH << std::endl;

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    a.setApplicationName("Playbook Creator");

    MainDialog w;
    try {
        QMessageBox::StandardButton shall_search_update =
                    QMessageBox::question(NULL,
                                          "PBC Update Checker",
                                          "Do you want to look for updates and news?\n"
                                          "Recommended only, if you have stable internet connection."  // NOLINT
                                          "This should only take a few seconds. Please press OK and wait...",
                                          QMessageBox::Ok | QMessageBox::Cancel); //NOLINT

        if(shall_search_update == QMessageBox::Ok) {
            std::cout << "Looking for updates. This should only take a few seconds. Please wait..." << std::endl;
            const uint32_t BUFF_LEN = 500;
            uint8_t buffer[BUFF_LEN] = {0};
            CBuffer desc_result_buf = CBuffer{buffer, BUFF_LEN};
            UpdateCheckingStatus ucs = updates_available(PBC_VERSION_MAJOR, PBC_VERSION_MINOR, PBC_VERSION_PATCH, &desc_result_buf); // TODO: numeric conversions might be problematic here!
            switch (ucs.tag) {
                case UpdateCheckingStatus::Tag::UpdatesAvailable:
                    {
                        unsigned major = ucs.updates_available._0;
                        uint64_t minor = ucs.updates_available._1;
                        uint64_t patch = ucs.updates_available._2;
                        std::string desc( reinterpret_cast<char const*>(buffer), BUFF_LEN ) ;
                        std::string latest_version = "v" + std::to_string(major) +  "." + std::to_string(minor) + "." + std::to_string(patch);
                        std::string msg = "A new version of PlaybookCreator is available!"
                                        "<br><br>Description of release " + latest_version + ":\n" + desc +
                                        "<br><br>Please visit <a href='https://github.com/obraunsdorf/playbook-creator/releases'>https://github.com/obraunsdorf/playbook-creator/releases</a> "
                                        "to update to the current version!";

                        QMessageBox info;
                        info.setWindowTitle("PBC Update Checker");
                        info.setTextFormat(Qt::RichText);
                        info.setText(QString::fromStdString(msg));
                        info.exec();
                    }
                    break;

                case UpdateCheckingStatus::Tag::Error:
                    {
                        QMessageBox::information(&w, "PBC Update Checker", "An error occured during checking for updates"
                                                                        " (maybe bad internet connection?). Please visit "
                                                                        "https://github.com/obraunsdorf/playbook-creator/releases "
                                                                        "and see if a new version of PBC has been released",
                                                QMessageBox::Ok);
                    }
                    break;

                case UpdateCheckingStatus::Tag::UpToDate:
                    std::cout << "No updates available" << std::endl;
                    break;

            }
        }
        w.show();
        a.exec();
    } catch(std::exception &e) {
        QString errorString = "Terminating playbook creator. Please open an bug report on "
                              "Github (https://github.com/obraunsdorf/playbook-creator/issues) "
                              "containing the following error log.\n\n";
        errorString += e.what();
        QMessageBox::critical(QApplication::activeWindow(), "", errorString);
        return 1;
    }
    return 0;
}
