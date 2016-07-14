/** @file pbcVersion.h
    This file is part of Playbook Creator.

    Playbook Creator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Playbook Creator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2015 Oliver Braunsdorf

    @author Oliver Braunsdorf
    @brief This is the version file. It has been created automatically by version.py which is located in the base directory.
*/
#ifndef VERSION_H
#define VERSION_H

#include <string>
#include <sstream>
#include <vector>

/**
 * @brief The PBCVersion class is used to determine the version of Playbook Creator at runtime and compare it to the version of playbooks saved in .pbc files
 */
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
    static const unsigned int MINOR = 4;
    static const unsigned int REVISION = 50;
    static const unsigned int BUILD = 1603151245;

	/**
    * @brief compares the string of the current version to another version string
    * @param otherVersion the other versionx string
    * @return the result of compareVersions()
    */
    static int compareCurrentVersionTo(const std::string& otherVersion) {
        return compareVersions(getVersionString(), otherVersion);
    }

    /**
     * @brief compares two version strings
     * @param v1
     * @param v2
     * @return -1 if v1 < v2, 0 if v1 == v2 and 1 if v1 > v2
     */
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

    /**
     * @brief builds the full version string
     * @return the full version string
     */
    static std::string getVersionString() {
        std::stringstream versionString;
        versionString << MAJOR << '.'
                      << MINOR << '.'
                      << REVISION << '.'
                      << BUILD;
        return versionString.str();
    }

    /**
     * @brief builds a simple version string which only includes MAJOR and MINOR version numbers
     * @return a simple version string
     */
    static std::string getSimpleVersionString() {
        std::stringstream versionString;
        versionString << MAJOR << '.' << MINOR;
        return versionString.str();
    }
};
#endif  // VERSION_H
