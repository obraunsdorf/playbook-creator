#ifndef VERSION_H
#define VERSION_H

#include <string>
#include <sstream>
#include <vector>

class PBCVersion {
 private:
    static int compareInt(int n, int m) {
        if(n == m) {
            return 0;
        } else if (n > m) {
            return 1;
        } else {
            return -1;
        }
    }

 public:
    static const unsigned int MAJOR = 0;
    static const unsigned int MINOR = 2;
    static const unsigned int REVISION = 23;
    static const unsigned int BUILD = 150523195;
    static int compare(const std::string& versionString) {
        std::vector<std::string> versionNumbers;
        std::stringstream ss(versionString);
        std::string item;
        while(std::getline(ss, item, '.')) {
            versionNumbers.push_back(item);
        }
        assert(versionNumbers.size() == 4);
        unsigned int otherMAJOR = std::stoul(versionNumbers[0]);
        unsigned int otherMINOR = std::stoul(versionNumbers[1]);
        unsigned int otherREVISION = std::stoul(versionNumbers[2]);
        unsigned int otherBUILD = std::stoul(versionNumbers[3]);
        int resultMAJOR = compareInt(MAJOR, otherMAJOR);
        if(resultMAJOR != 0) {
            return resultMAJOR;
        } else {
            int resultMINOR = compareInt(MINOR, otherMINOR);
            if(resultMINOR != 0) {
                return resultMINOR;
            } else {
                int resultREVISION = compareInt(REVISION, otherREVISION);
                if(resultREVISION != 0) {
                    return resultREVISION;
                } else {
                    return compareInt(BUILD, otherBUILD);
                }
            }
        }
    }

    static std::string getVersionString() {
        std::stringstream versionString;
        versionString << MAJOR << '.'
                      << MINOR << '.'
                      << REVISION << '.'
                      << BUILD;
        return versionString.str();
    }

    static std::string getSimpleVersionString() {
        std::stringstream versionString;
        versionString << MAJOR << '.' << MINOR;
        return versionString.str();
    }
};
#endif  // VERSION_H
