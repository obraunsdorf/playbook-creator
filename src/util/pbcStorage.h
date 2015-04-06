#ifndef PBCSTORAGE_H
#define PBCSTORAGE_H

#include "pbcSingleton.h"
#include "models/pbcPlay.h"
#include <string>
#include <vector>
#include <botan/botan.h>

class PBCStorage : public PBCSingleton<PBCStorage> {
friend class PBCSingleton<PBCStorage>;
 private:
    const std::string _CIPHER = "AES-256/CBC";  // TODO(obr): update to current botan and use AES with GCM   // NOLINT
    const std::string _HASH = "SHA-256";
    const std::string _PBKDF = "PBKDF2(SHA-256)";
    const unsigned int _PBKDF_ITERATIONS = 10000;
    const unsigned int _SALT_SIZE = 16;  // in Bytes
    const unsigned int _KEY_SIZE = 32;  // in Bytes = 256 Bits
    const unsigned int _HASH_SIZE = 32;  // in Bytes = 256 Bits

    std::string _currentPlaybookFileName;
    boost::shared_ptr<Botan::SecureVector<Botan::byte>> _saltSP;
    boost::shared_ptr<Botan::OctetString> _keySP;

    void checkVersion(const std::string& version);
    void generateAndSetKey(const std::string& password);
    void setCryptoKey(Botan::OctetString key,
                      Botan::SecureVector<Botan::byte> salt);
    void encrypt(const std::string& input, std::ofstream& outFile);  // NOLINT
    void decrypt(const std::string& password,
                 std::ostream& ostream,  // NOLINT
                 std::ifstream& inFile); // NOLINT

 protected:
    PBCStorage();

 public:
    void init(const std::string& fileName);
    void savePlaybook(const std::string& password, const std::string& fileName);
    void automaticSavePlaybook();
    void writeToCurrentPlaybookFile();
    void loadPlaybook(const std::string& password, const std::string& fileName);
    void exportPlay(const std::string& fileName, PBCPlaySP play);
    void importPlay(const std::string& fileName, PBCPlaySP play);
    // void exportAsPDF(const std::string& fileName);
};

#endif  // PBCSTORAGE_H
