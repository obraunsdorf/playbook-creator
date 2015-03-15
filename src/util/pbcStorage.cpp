#include "pbcStorage.h"
#include "models/pbcPlaybook.h"
#include "util/pbcConfig.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <string>

void PBCStorage::checkVersion(const std::string &version) {
    int result = PBCConfig::getInstance()->version().compare(version);
    assert(result <= 0);
}

PBCStorage::PBCStorage() {
}

void PBCStorage::savePlaybook(const std::string &fileName) {
    std::string extension = fileName.substr(fileName.size() - 4);
    assert(extension == ".pbc");
    std::stringbuf buff;
    std::ostream ostream(&buff);
    ostream << "Playbook-Creator" << "\n";
    ostream << "playbook" << "\n";
    ostream << PBCConfig::getInstance()->version() << "\n";
    boost::archive::text_oarchive archive(ostream);
    archive << *PBCPlaybook::getInstance();
    std::ofstream ofstream(fileName);
    ofstream << buff.str();
    ofstream.close();
}

void PBCStorage::loadPlaybook(const std::string &fileName) {
    std::string extension = fileName.substr(fileName.size() - 4);
    assert(extension == ".pbc");
    std::ifstream ifstream(fileName);

    int kBuffSize = 17;
    char buffer[kBuffSize];
    ifstream.getline(buffer, kBuffSize);
    std::string pbcString(buffer);
    assert(pbcString == "Playbook-Creator");

    ifstream.getline(buffer, kBuffSize);
    std::string pbString(buffer);
    assert(pbString == "playbook");

    ifstream.getline(buffer, kBuffSize);
    std::string version(buffer);
    checkVersion(version);

    boost::archive::text_iarchive archive(ifstream);
    archive >> *PBCPlaybook::getInstance();
}
