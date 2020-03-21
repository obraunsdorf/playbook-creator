#define BOOST_TEST_MODULE PBCTests
#include "util/pbcStorage.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(StorageTests)
    BOOST_AUTO_TEST_CASE(import_test) {

        using namespace boost::filesystem;
        path test_pbs =
                path("test")
                .append("resources")
                .append("StorageTests");
        path test_pb1_path_orig = path(test_pbs).append("TestPlaybook1.pbc");
        path test_pb2_path_orig = path(test_pbs).append("TestPlaybook2.pbc");
        path test_pb1_path = path(test_pbs).append("tmp1.pbc");
        path test_pb2_path = path(test_pbs).append("tmp2.pbc");

        copy_file(test_pb1_path_orig, test_pb1_path, copy_option::overwrite_if_exists);
        copy_file(test_pb2_path_orig, test_pb2_path, copy_option::overwrite_if_exists);

        std::cout << "loading playbook" << std::endl;
        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb2_path.string());
        std::cout << "done loading playbook" << std::endl;
        std::vector<std::string> playnames2 = PBCController::getInstance()->getPlaybook()->getPlayNames();
        BOOST_CHECK(playnames2.empty() == false);

        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb1_path.string());
        std::vector<std::string> playnames1 = PBCController::getInstance()->getPlaybook()->getPlayNames();
        BOOST_CHECK(playnames1.empty() == false);

        PBCStorage::getInstance()->importPlaybook("test", test_pb2_path.string());
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
