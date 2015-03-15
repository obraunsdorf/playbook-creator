#ifndef PBCSTORAGE_H
#define PBCSTORAGE_H

#include "pbcSingleton.h"
#include "models/pbcPlay.h"
#include <string>

class PBCStorage : public PBCSingleton<PBCStorage> {
friend class PBCSingleton<PBCStorage>;
 private:
    void checkVersion(const std::string& version);
 protected:
    PBCStorage();

 public:
    void savePlaybook(const std::string& fileName);
    void loadPlaybook(const std::string& fileName);
    void exportPlay(const std::string& fileName, PBCPlaySP play);
    void importPlay(const std::string& fileName, PBCPlaySP play);
    // void exportAsPDF(const std::string& fileName);
};

#endif  // PBCSTORAGE_H
