#include "util/pbcStorage.h"

#include <iostream>
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    // Create a temporary file
    const std::string testPlaybookPasswd = "test";

    const std::string tmp_filename = "/tmp/pbc_fuzzer_tmp";
    std::ofstream file(tmp_filename, std::ios::binary);

    // Write the data into the file
    if (file)
    {
        file.write(reinterpret_cast<const char *>(Data), Size);
        file.close();
    }

    // Read the file again
    std::ifstream inputFile(tmp_filename, std::ios::binary);
    if (inputFile)
    {
        // Read the contents of the file
        std::string content((std::istreambuf_iterator<char>(inputFile)),
                            std::istreambuf_iterator<char>());

        try {
            PBCStorage::getInstance()->inner_loadPlaybook(testPlaybookPasswd, inputFile, PBCController::getInstance()->getPlaybook());       
        } catch(PBCUnexpectedError& e) {
            std::cout << "PBC error: " << std::endl;  // TODD(obr): message to user
        }
        inputFile.close();
    }

    // Remove the temporary file
    std::remove(tmp_filename.c_str());
    
    return 0;
}