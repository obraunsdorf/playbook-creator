#define BOOST_TEST_MODULE PBCTests

#include "util/pbcStorage.h"
#include "util/pbcExceptions.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>


using namespace boost::filesystem;
using namespace boost::unit_test;

struct PBCTestConfig {
    static path test_base_dir;

    PBCTestConfig() {
        std::cout << "global setup" << std::endl;
        BOOST_REQUIRE ( framework::master_test_suite().argc == 3 );
        BOOST_TEST( framework::master_test_suite().argv[1] == "--test-base-dir" );
        test_base_dir = path(framework::master_test_suite().argv[2]);
        //std::cout << "test base directory: " << test_base_dir << std::endl;
    }

    ~PBCTestConfig() { std::cout << "global teardown\n"; }
};
path PBCTestConfig::test_base_dir = "test";     //default directory. but has to be overwritten, anyway.

BOOST_TEST_GLOBAL_FIXTURE(PBCTestConfig);



BOOST_AUTO_TEST_SUITE(VersionTests)

    void checkPlaybook() {
        BOOST_CHECK(PBCController::getInstance()->getPlaybook()->getPlay("testplay"));
        BOOST_CHECK(PBCController::getInstance()->getPlaybook()->getFormation("TestFormation"));
        BOOST_CHECK(PBCController::getInstance()->getPlaybook()->getRoute("testroute"));
        BOOST_CHECK(PBCController::getInstance()->getPlaybook()->getCategory("TestCategory"));
    }

    BOOST_AUTO_TEST_CASE(v0_18_0) {
    path global_test_directory =  PBCTestConfig::test_base_dir;
    path test_pbs =
            global_test_directory
                    .append("resources")
                    .append("StorageTests")
                    .append("VersionTestPlaybooks");
    path test_pb_path = path(test_pbs).append("v0_18_0.pbc");
    PBCStorage::getInstance()->loadActivePlaybook("test", test_pb_path.string());

    checkPlaybook();
    }

    BOOST_AUTO_TEST_CASE(v0_15_0) {
        path global_test_directory =  PBCTestConfig::test_base_dir;
        path test_pbs =
                global_test_directory
                        .append("resources")
                        .append("StorageTests")
                        .append("VersionTestPlaybooks");
        path test_pb_path = path(test_pbs).append("v0_15_0.pbc");
        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb_path.string());

        checkPlaybook();
    }

    BOOST_AUTO_TEST_CASE(v0_14_1) {
        path global_test_directory =  PBCTestConfig::test_base_dir;
        path test_pbs =
                global_test_directory
                        .append("resources")
                        .append("StorageTests")
                        .append("VersionTestPlaybooks");
        path test_pb_path = path(test_pbs).append("v0_14_1.pbc");
        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb_path.string());

        checkPlaybook();
    }


    BOOST_AUTO_TEST_CASE(v0_13_0) {
        path global_test_directory =  PBCTestConfig::test_base_dir;
        path test_pbs =
                global_test_directory
                        .append("resources")
                        .append("StorageTests")
                        .append("VersionTestPlaybooks");
        path test_pb_path = path(test_pbs).append("v0_13_0.pbc");
        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb_path.string());

        checkPlaybook();
    }

    BOOST_AUTO_TEST_CASE(v0_12_0) {
        path global_test_directory =  PBCTestConfig::test_base_dir;
        path test_pbs =
                global_test_directory
                        .append("resources")
                        .append("StorageTests")
                        .append("VersionTestPlaybooks");
        path test_pb_path = path(test_pbs).append("v0_12_0.pbc");
        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb_path.string());

        checkPlaybook();
    }
BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(StorageTests)
    BOOST_AUTO_TEST_CASE(store_and_load_test) {
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




    BOOST_AUTO_TEST_CASE(import_test_with_prefix_suffix_active_playbook) {
        path global_test_directory = PBCTestConfig::test_base_dir;
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
        BOOST_REQUIRE(playnames2.empty() == false);
        std::vector<std::string> categoryNames2 = PBCController::getInstance()->getPlaybook()->getCategoryNames();
        BOOST_REQUIRE(categoryNames2.empty() == false);
        std::vector<std::string> routeNames2 = PBCController::getInstance()->getPlaybook()->getRouteNames();
        BOOST_REQUIRE(routeNames2.empty() == false);
        std::vector<std::string> formationNames2 = PBCController::getInstance()->getPlaybook()->getFormationNames();
        BOOST_REQUIRE(formationNames2.empty() == false);

        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb1_path.string());
        std::vector<std::string> playnames1 = PBCController::getInstance()->getPlaybook()->getPlayNames();
        BOOST_REQUIRE(playnames1.empty() == false);
        std::vector<std::string> categoryNames1 = PBCController::getInstance()->getPlaybook()->getCategoryNames();
        BOOST_REQUIRE(categoryNames1.empty() == false);
        std::vector<std::string> routeNames1 = PBCController::getInstance()->getPlaybook()->getRouteNames();
        BOOST_REQUIRE(routeNames1.empty() == false);
        std::vector<std::string> formationNames1 = PBCController::getInstance()->getPlaybook()->getFormationNames();
        BOOST_REQUIRE(formationNames1.empty() == false);

        std::string prefix = "prefix_";
        std::string suffix = "_suffix";
        PBCStorage::getInstance()->importPlaybook(
                "test",
                test_pb2_path.string(),
                true,
                true,
                true,
                true,
                prefix,
                suffix);

        std::cout << "plays from playbook1" << std::endl;
        for (const std::string &playname : playnames1) {
            std::cout << "    " << playname << std::endl;
            PBCPlaySP play = PBCController::getInstance()->getPlaybook()->getPlay(playname);
            BOOST_CHECK(play != NULL);
        }

        std::cout << "plays from playbook2" << std::endl;
        for (const std::string &playname : playnames2) {
            std::cout << "    " << playname << std::endl;
            PBCPlaySP play = PBCController::getInstance()->getPlaybook()->getPlay(prefix + playname + suffix);
            BOOST_CHECK(play != NULL);
        }

        std::cout << "categories from playbook1" << std::endl;
        for (const std::string &categoryName : categoryNames1) {
            std::cout << "    " << categoryName << std::endl;
            PBCCategorySP category = PBCController::getInstance()->getPlaybook()->getCategory(categoryName);
            BOOST_CHECK(category != NULL);
        }

        std::cout << "categories from playbook2" << std::endl;
        for (const std::string &categoryName : categoryNames2) {
            std::cout << "    " << categoryName << std::endl;
            PBCCategorySP category = PBCController::getInstance()->getPlaybook()->getCategory(prefix + categoryName + suffix);
            BOOST_CHECK(category != NULL);
        }

        std::cout << "routes from playbook1" << std::endl;
        for (const std::string &routeName : routeNames1) {
            std::cout << "    " << routeName << std::endl;
            PBCRouteSP route = PBCController::getInstance()->getPlaybook()->getRoute(routeName);
            BOOST_CHECK(route != NULL);
        }

        std::cout << "routes from playbook2" << std::endl;
        for (const std::string &routeName : routeNames2) {
            std::cout << "    " << routeName << std::endl;
            PBCRouteSP route = PBCController::getInstance()->getPlaybook()->getRoute(prefix + routeName + suffix);
            BOOST_CHECK(route != NULL);
        }

        std::cout << "formations from playbook1" << std::endl;
        for (const std::string &formationName : formationNames1) {
            std::cout << "    " << formationName << std::endl;
            PBCFormationSP formation = PBCController::getInstance()->getPlaybook()->getFormation(formationName);
            BOOST_CHECK(formation != NULL);
        }

        std::cout << "formations from playbook2" << std::endl;
        for (const std::string &formationName : formationNames2) {
            std::cout << "    " << formationName << std::endl;
            PBCFormationSP formation = PBCController::getInstance()->getPlaybook()->getFormation(prefix + formationName + suffix);
            BOOST_CHECK(formation != NULL);
        }
    }


    BOOST_AUTO_TEST_CASE(import_test_with_prefix_suffix_stored_playbook) {
        path global_test_directory = PBCTestConfig::test_base_dir;
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
        BOOST_REQUIRE(playnames2.empty() == false);
        std::vector<std::string> categoryNames2 = PBCController::getInstance()->getPlaybook()->getCategoryNames();
        BOOST_REQUIRE(categoryNames2.empty() == false);
        std::vector<std::string> routeNames2 = PBCController::getInstance()->getPlaybook()->getRouteNames();
        BOOST_REQUIRE(routeNames2.empty() == false);
        std::vector<std::string> formationNames2 = PBCController::getInstance()->getPlaybook()->getFormationNames();
        BOOST_REQUIRE(formationNames2.empty() == false);

        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb1_path.string());
        std::vector<std::string> playnames1 = PBCController::getInstance()->getPlaybook()->getPlayNames();
        BOOST_REQUIRE(playnames1.empty() == false);
        std::vector<std::string> categoryNames1 = PBCController::getInstance()->getPlaybook()->getCategoryNames();
        BOOST_REQUIRE(categoryNames1.empty() == false);
        std::vector<std::string> routeNames1 = PBCController::getInstance()->getPlaybook()->getRouteNames();
        BOOST_REQUIRE(routeNames1.empty() == false);
        std::vector<std::string> formationNames1 = PBCController::getInstance()->getPlaybook()->getFormationNames();
        BOOST_REQUIRE(formationNames1.empty() == false);

        std::string prefix = "prefix_";
        std::string suffix = "_suffix";
        PBCStorage::getInstance()->importPlaybook(
                "test",
                test_pb2_path.string(),
                true,
                true,
                true,
                true,
                prefix,
                suffix);

        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb1_path.string());

        std::cout << "plays from playbook1" << std::endl;
        for (const std::string &playname : playnames1) {
            std::cout << "    " << playname << std::endl;
            PBCPlaySP play = PBCController::getInstance()->getPlaybook()->getPlay(playname);
            BOOST_CHECK(play != NULL);
        }

        std::cout << "plays from playbook2" << std::endl;
        for (const std::string &playname : playnames2) {
            std::cout << "    " << playname << std::endl;
            PBCPlaySP play = PBCController::getInstance()->getPlaybook()->getPlay(prefix + playname + suffix);
            BOOST_CHECK(play != NULL);
        }

        std::cout << "categories from playbook1" << std::endl;
        for (const std::string &categoryName : categoryNames1) {
            std::cout << "    " << categoryName << std::endl;
            PBCCategorySP category = PBCController::getInstance()->getPlaybook()->getCategory(categoryName);
            BOOST_CHECK(category != NULL);
        }

        std::cout << "categories from playbook2" << std::endl;
        for (const std::string &categoryName : categoryNames2) {
            std::cout << "    " << categoryName << std::endl;
            PBCCategorySP category = PBCController::getInstance()->getPlaybook()->getCategory(prefix + categoryName + suffix);
            BOOST_CHECK(category != NULL);
        }

        std::cout << "routes from playbook1" << std::endl;
        for (const std::string &routeName : routeNames1) {
            std::cout << "    " << routeName << std::endl;
            PBCRouteSP route = PBCController::getInstance()->getPlaybook()->getRoute(routeName);
            BOOST_CHECK(route != NULL);
        }

        std::cout << "routes from playbook2" << std::endl;
        for (const std::string &routeName : routeNames2) {
            std::cout << "    " << routeName << std::endl;
            PBCRouteSP route = PBCController::getInstance()->getPlaybook()->getRoute(prefix + routeName + suffix);
            BOOST_CHECK(route != NULL);
        }

        std::cout << "formations from playbook1" << std::endl;
        for (const std::string &formationName : formationNames1) {
            std::cout << "    " << formationName << std::endl;
            PBCFormationSP formation = PBCController::getInstance()->getPlaybook()->getFormation(formationName);
            BOOST_CHECK(formation != NULL);
        }

        std::cout << "formations from playbook2" << std::endl;
        for (const std::string &formationName : formationNames2) {
            std::cout << "    " << formationName << std::endl;
            PBCFormationSP formation = PBCController::getInstance()->getPlaybook()->getFormation(prefix + formationName + suffix);
            BOOST_CHECK(formation != NULL);
        }
    }




    BOOST_AUTO_TEST_CASE(import_test_routes) {
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
        BOOST_REQUIRE(playnames2.empty() == false);
        std::vector<std::string> categoryNames2 = PBCController::getInstance()->getPlaybook()->getCategoryNames();
        BOOST_REQUIRE(categoryNames2.empty() == false);
        std::vector<std::string> routeNames2 = PBCController::getInstance()->getPlaybook()->getRouteNames();
        BOOST_REQUIRE(routeNames2.empty() == false);
        std::vector<std::string> formationNames2 = PBCController::getInstance()->getPlaybook()->getFormationNames();
        BOOST_REQUIRE(formationNames2.empty() == false);

        PBCStorage::getInstance()->loadActivePlaybook("test", test_pb1_path.string());
        std::vector<std::string> playnames1 = PBCController::getInstance()->getPlaybook()->getPlayNames();
        BOOST_REQUIRE(playnames1.empty() == false);
        std::vector<std::string> categoryNames1 = PBCController::getInstance()->getPlaybook()->getCategoryNames();
        BOOST_REQUIRE(categoryNames1.empty() == false);
        std::vector<std::string> routeNames1 = PBCController::getInstance()->getPlaybook()->getRouteNames();
        BOOST_REQUIRE(routeNames1.empty() == false);
        std::vector<std::string> formationNames1 = PBCController::getInstance()->getPlaybook()->getFormationNames();
        BOOST_REQUIRE(formationNames1.empty() == false);

        PBCStorage::getInstance()->importPlaybook(
                "test",
                test_pb2_path.string(),
                false,
                false,
                true,
                false,
                "prefix_");

        std::cout << "plays from playbook1" << std::endl;
        for (const std::string &playname : playnames1) {
            PBCPlaySP play = PBCController::getInstance()->getPlaybook()->getPlay(playname);
            const auto& it = std::find(playnames2.begin(), playnames2.end(), playname);
            BOOST_CHECK(it == playnames2.end());
        }


        std::cout << "categories from playbook1" << std::endl;
        for (const std::string &categoryName : categoryNames1) {
            std::cout << "    " << categoryName << std::endl;
            PBCCategorySP category = PBCController::getInstance()->getPlaybook()->getCategory(categoryName);
            BOOST_CHECK(category != NULL);
        }


        std::cout << "routes from playbook1" << std::endl;
        for (const std::string &routeName : routeNames1) {
            std::cout << "    " << routeName << std::endl;
            PBCRouteSP route = PBCController::getInstance()->getPlaybook()->getRoute(routeName);
            BOOST_CHECK(route != NULL);
        }

        std::cout << "routes from playbook2" << std::endl;
        for (const std::string &routeName : routeNames2) {
            std::cout << "    " << routeName << std::endl;
            PBCRouteSP route = PBCController::getInstance()->getPlaybook()->getRoute("prefix_" + routeName);
            BOOST_CHECK(route != NULL);
        }

        std::cout << "formations from playbook1" << std::endl;
        for (const std::string &formationName : formationNames1) {
            std::cout << "    " << formationName << std::endl;
            PBCFormationSP formation = PBCController::getInstance()->getPlaybook()->getFormation(formationName);
            BOOST_CHECK(formation != NULL);
        }
    }

    BOOST_AUTO_TEST_CASE(overwrite_test) {
        PBCFormationSP formation = PBCController::getInstance()->getPlaybook()->formations().front();
        PBCPlaySP play(new PBCPlay("testplay1", "testcode1", formation->name()));
        PBCStorage::getInstance()->savePlaybook("test", "test.pbc");
        PBCController::getInstance()->getPlaybook()->addPlay(play);  // playbook is automatically saved here;
        PBCController::getInstance()->getPlaybook()->resetToNewEmptyPlaybook("active", 5);
        PBCStorage::getInstance()->savePlaybook("active", "active.pbc");
        BOOST_CHECK_THROW(
                PBCStorage::getInstance()->importPlaybook("test", "test.pbc", true, true, true, true),
                PBCImportException
        );
        PBCStorage::getInstance()->loadActivePlaybook("active", "active.pbc");
        const auto& playNames =  PBCController::getInstance()->getPlaybook()->getPlayNames();
        BOOST_CHECK_EQUAL(playNames.size(), 0); // the thrown exception should lead to a reset of the playbook

    }

    BOOST_AUTO_TEST_CASE(invalid_number_of_players_test) {
        PBCController::getInstance()->getPlaybook()->resetToNewEmptyPlaybook("7er", 7);
        PBCFormationSP formation = PBCController::getInstance()->getPlaybook()->formations().front();
        PBCPlaySP play(new PBCPlay("testplay1", "testcode1", formation->name()));
        PBCStorage::getInstance()->savePlaybook("test", "test.pbc");
        PBCController::getInstance()->getPlaybook()->addPlay(play);  // playbook is automatically saved here;
        PBCController::getInstance()->getPlaybook()->resetToNewEmptyPlaybook("5er", 5);
        BOOST_CHECK_THROW(
                PBCStorage::getInstance()->importPlaybook("test", "test.pbc", true, true, true, true),
                PBCImportException
        );
    }
BOOST_AUTO_TEST_SUITE_END()