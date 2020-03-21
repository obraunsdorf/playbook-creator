#define BOOST_TEST_MODULE PBCTests
#include "util/pbcStorage.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(StorageTests)
    BOOST_AUTO_TEST_CASE(import_test) {
        boost::filesystem::copy_file("test/resources/StorageTests/TestPlaybook1.pbc","test/tmp1.pbc", boost::filesystem::copy_option::overwrite_if_exists);
        boost::filesystem::copy_file("test/resources/StorageTests/TestPlaybook2.pbc","test/tmp2.pbc", boost::filesystem::copy_option::overwrite_if_exists);

        std::cout << "loading playbook" << std::endl;
        PBCStorage::getInstance()->loadActivePlaybook("test", "test/tmp2.pbc");
        std::cout << "done loading playbook" << std::endl;
        std::vector<std::string> playnames2 = PBCController::getInstance()->getPlaybook()->getPlayNames();
        BOOST_CHECK(playnames2.empty() == false);

        PBCStorage::getInstance()->loadActivePlaybook("test", "test/tmp1.pbc");
        std::vector<std::string> playnames1 = PBCController::getInstance()->getPlaybook()->getPlayNames();
        BOOST_CHECK(playnames1.empty() == false);

        PBCStorage::getInstance()->importPlaybook("test", "test/tmp2.pbc");
        for (const std::string& playname : playnames1) {
            std::cout << playname << std::endl;
            PBCPlaySP play = PBCController::getInstance()->getPlaybook()->getPlay(playname);
            BOOST_CHECK(play != NULL);
        }
        for (const std::string& playname : playnames2) {
            std::cout << playname << std::endl;
            PBCPlaySP play = PBCController::getInstance()->getPlaybook()->getPlay(playname);
            BOOST_CHECK(play != NULL);
        }
    }
BOOST_AUTO_TEST_SUITE_END()
