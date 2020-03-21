#define BOOST_TEST_MODULE PBCTests

#include "util/pbcStorage.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>


using namespace boost::filesystem;
using namespace boost::unit_test;

struct PBCTestConfig {
    static path test_base_dir;

    PBCTestConfig() {
        std::cout << "global setup" << std::endl;
        BOOST_TEST_REQUIRE( framework::master_test_suite().argc == 3 );
        BOOST_TEST( framework::master_test_suite().argv[1] == "--test-base-dir" );
        test_base_dir = path(framework::master_test_suite().argv[2]);
        //std::cout << "test base directory: " << test_base_dir << std::endl;
    }

    ~PBCTestConfig() { std::cout << "global teardown\n"; }
};
path PBCTestConfig::test_base_dir = "test";     //default directory. but has to be overwritten, anyway.
BOOST_TEST_GLOBAL_FIXTURE(PBCTestConfig);


BOOST_AUTO_TEST_SUITE(StorageTests)
    BOOST_AUTO_TEST_CASE(store_and_load) {
        PBCFormationSP formation = PBCController::getInstance()->getPlaybook()->formations().front();
        PBCPlaySP play(new PBCPlay("testplay1", "testcode1", formation->name()));
        BOOST_CHECK(play->name() == "testplay1");
        PBCStorage::getInstance()->savePlaybook("test", "test.pbc");
        PBCController::getInstance()->getPlaybook()->addPlay(play);  // playbook is automatically saved here;
        PBCStorage::getInstance()->loadActivePlaybook("test", "test.pbc");
        PBCPlaySP loaded_play = PBCController::getInstance()->getPlaybook()->getPlay("testplay1");
        BOOST_CHECK_EQUAL(loaded_play->codeName(), "testcode1");
    }

    BOOST_AUTO_TEST_CASE(file_copy_test) {
        path global_test_directory =  PBCTestConfig::test_base_dir;
        path test_pbs =
            global_test_directory
                .append("resources")
                .append("StorageTests");
        path test_pb1_path_orig = path(test_pbs).append("TestPlaybook1.pbc");
        path test_pb2_path_orig = path(test_pbs).append("TestPlaybook2.pbc");
        path test_pb1_path = path(test_pbs).append("tmp1.pbc");
        path test_pb2_path = path(test_pbs).append("tmp2.pbc");

        copy_file(test_pb1_path_orig, test_pb1_path, copy_option::overwrite_if_exists);
        copy_file(test_pb2_path_orig, test_pb2_path, copy_option::overwrite_if_exists);

        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb2_path.string());
    }

    BOOST_AUTO_TEST_CASE(import_test) {
        path global_test_directory =  PBCTestConfig::test_base_dir;
        path test_pbs =
                global_test_directory
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
        for (const std::string &playname : playnames1) {
            std::cout << playname << std::endl;
            PBCPlaySP play = PBCController::getInstance()->getPlaybook()->getPlay(playname);
            BOOST_CHECK(play != NULL);
        }
        for (const std::string &playname : playnames2) {
            std::cout << playname << std::endl;
            PBCPlaySP play = PBCController::getInstance()->getPlaybook()->getPlay(playname);
            BOOST_CHECK(play != NULL);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
