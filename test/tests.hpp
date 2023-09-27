
using namespace boost::filesystem;

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