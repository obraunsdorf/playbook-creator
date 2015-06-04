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
    static const unsigned int MINOR = 3;
    static const unsigned int REVISION = 27;
    static const unsigned int BUILD = 1506011024;

    static int compareCurrentVersionTo(const std::string& otherVersion) {
        return compareVersions(getVersionString(), otherVersion);
    }

    static int compareVersions(const std::string& v1, const std::string& v2) {
        std::vector<std::string> versionNumbers1;
        std::stringstream ss1(v1);
        std::string item1;
        while(std::getline(ss1, item1, '.')) {
            versionNumbers1.push_back(item1);
        }
        assert(versionNumbers1.size() == 4);
        unsigned int major1 = std::stoul(versionNumbers1[0]);
        unsigned int minor1 = std::stoul(versionNumbers1[1]);
        unsigned int revision1 = std::stoul(versionNumbers1[2]);
        unsigned int build1 = std::stoul(versionNumbers1[3]);

        std::vector<std::string> versionNumbers2;
        std::stringstream ss2(v2);
        std::string item2;
        while(std::getline(ss2, item2, '.')) {
            versionNumbers2.push_back(item2);
        }
        assert(versionNumbers2.size() == 4);
        unsigned int major2 = std::stoul(versionNumbers2[0]);
        unsigned int minor2 = std::stoul(versionNumbers2[1]);
        unsigned int revision2 = std::stoul(versionNumbers2[2]);
        unsigned int build2 = std::stoul(versionNumbers2[3]);


        int resultMajor = compareInt(major1, major2);
        if(resultMajor != 0) {
            return resultMajor;
        } else {
            int resultMinor = compareInt(minor1, minor2);
            if(resultMinor != 0) {
                return resultMinor;
            } else {
                int resultRevision = compareInt(revision1, revision2);
                if(resultRevision != 0) {
                    return resultRevision;
                } else {
                    return compareInt(build1, build2);
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
