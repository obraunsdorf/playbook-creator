"""
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

This Pyton script is responsible for creating creating the full version
number of the Playbook Creator application. It is invoked by the build 
system. It takes MAJOR and MINOR version numbers as commandline parameters.
The REVISION number is taken from the svn revision.
The BUILD number concatenates year, month, day and minute of the day the
application is being built.
"""

from __future__ import print_function
import sys
import subprocess
import datetime

def main():
	versionFileName = "src/pbcVersion.h"
	
	# read major and minor version number from command line
	if(len(sys.argv) != 3):
		print("You have to specify 2 integer parameters. MAJOR version number and MINOR version number. Your parameters were " + str(sys.argv), file=sys.stderr)
		sys.exit(1)

	if(str.isdigit(sys.argv[1]) == False or str.isdigit(sys.argv[2]) == False):
		print("MAJOR and MINOR version numbers have to be integers. Your parameters were " + str(sys.argv), file=sys.stderr)
		sys.exit(1)

	MAJOR = int(sys.argv[1])
	MINOR = int(sys.argv[2])
	REVISION = 0



	# determine svn revision number
	revisionString = subprocess.check_output(["svnversion"])
	revisionString = revisionString.strip()
	colonPos = revisionString.find(":")
	mPos = revisionString.find("M")
	sPos = revisionString.find("S")
	pPos = revisionString.find("P")
	if(colonPos != -1):
		if(mPos != -1):
			REVISION = int(revisionString[colonPos+1:mPos])
		elif(sPos != -1):
			REVISION = int(revisionString[colonPos+1:sPos])
		else:
			REVISION = int(revisionString[colonPos+1:])
	elif(mPos != -1):
		REVISION = int(revisionString[0 : mPos])
	elif(sPos != -1):
		REVISION = int(revisionString[0 : sPos])
	elif(pPos != -1):
		REVISION = int(revisionString[0 : pPos])
	else:
		REVISION = int(revisionString)


	# determine build time and calculate build number
	now = datetime.datetime.now()
	minuteOfDay = now.hour * 60 + now.minute
	buildTimeString = now.strftime("%y%m%d" + str(minuteOfDay))
	assert str.isdigit(buildTimeString) == True
	BUILD = int(buildTimeString)


	# writer version header file
	versionFile = open(versionFileName, "w+")

	versionFile.write("/** @file pbcVersion.h\n")
	versionFile.write("    This file is part of Playbook Creator.\n\n")

	versionFile.write("    Playbook Creator is free software: you can redistribute it and/or modify\n")
	versionFile.write("    it under the terms of the GNU General Public License as published by\n")
	versionFile.write("    the Free Software Foundation, either version 3 of the License, or\n")
	versionFile.write("    (at your option) any later version.\n\n")

	versionFile.write("    Playbook Creator is distributed in the hope that it will be useful,\n")
	versionFile.write("    but WITHOUT ANY WARRANTY; without even the implied warranty of\n")
	versionFile.write("    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n")
	versionFile.write("    GNU General Public License for more details.\n\n")

	versionFile.write("    You should have received a copy of the GNU General Public License\n")
	versionFile.write("    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.\n\n")
	
	versionFile.write("    Copyright 2015 Oliver Braunsdorf\n\n")

	versionFile.write("    @author Oliver Braunsdorf\n")
	versionFile.write("    @brief This is the version file. It has been created automatically by version.py which is located in the base directory.\n")
	versionFile.write("*/\n")

	versionFile.write("#ifndef VERSION_H\n")
	versionFile.write("#define VERSION_H\n")
	versionFile.write("\n")

	versionFile.write("#include <string>\n")
	versionFile.write("#include <sstream>\n")
	versionFile.write("#include <vector>\n")
	versionFile.write("\n")

	versionFile.write("/**\n")
	versionFile.write(" * @brief The PBCVersion class is used to determine the version of Playbook Creator at runtime and compare it to the version of playbooks saved in .pbc files\n")
	versionFile.write(" */\n")
	versionFile.write("class PBCVersion {\n")
	versionFile.write(" private:\n")
	versionFile.write("    static int compareInt(int n, int m) {\n")
	versionFile.write("        if(n == m) {\n")
	versionFile.write("            return 0;\n")
	versionFile.write("        } else if (n > m) {\n")
	versionFile.write("            return 1;\n")
	versionFile.write("        } else {\n")
	versionFile.write("            return -1;\n")
	versionFile.write("        }\n")
	versionFile.write("    }\n")
	versionFile.write("\n")

	versionFile.write(" public:\n")
	versionFile.write("    static const unsigned int MAJOR = " + str(MAJOR) + ";\n")
	versionFile.write("    static const unsigned int MINOR = " + str(MINOR) + ";\n")
	versionFile.write("    static const unsigned int REVISION = " + str(REVISION) + ";\n")
	versionFile.write("    static const unsigned int BUILD = " + str(BUILD) + ";\n")
	versionFile.write("\n")

	versionFile.write("	/**\n")
	versionFile.write("    * @brief compares the string of the current version to another version string\n")
	versionFile.write("    * @param otherVersion the other versionx string\n")
	versionFile.write("    * @return the result of compareVersions()\n")
	versionFile.write("    */\n")
	versionFile.write("    static int compareCurrentVersionTo(const std::string& otherVersion) {\n")
	versionFile.write("        return compareVersions(getVersionString(), otherVersion);\n")
	versionFile.write("    }\n")
	versionFile.write("\n")

	
	versionFile.write("    /**\n")
	versionFile.write("     * @brief compares two version strings\n")
	versionFile.write("     * @param v1\n")
	versionFile.write("     * @param v2\n")
	versionFile.write("     * @return -1 if v1 < v2, 0 if v1 == v2 and 1 if v1 > v2\n")
	versionFile.write("     */\n")
	versionFile.write("    static int compareVersions(const std::string& v1, const std::string& v2) {\n")
	versionFile.write("        std::vector<std::string> versionNumbers1;\n")
	versionFile.write("        std::stringstream ss1(v1);\n")
	versionFile.write("        std::string item1;\n")
	versionFile.write("        while(std::getline(ss1, item1, '.')) {\n")
	versionFile.write("            versionNumbers1.push_back(item1);\n")
	versionFile.write("        }\n")
	versionFile.write("        assert(versionNumbers1.size() == 4);\n")
	versionFile.write("        unsigned int major1 = std::stoul(versionNumbers1[0]);\n")
	versionFile.write("        unsigned int minor1 = std::stoul(versionNumbers1[1]);\n")
	versionFile.write("        unsigned int revision1 = std::stoul(versionNumbers1[2]);\n")
	versionFile.write("        unsigned int build1 = std::stoul(versionNumbers1[3]);\n")
	versionFile.write("\n")

	versionFile.write("        std::vector<std::string> versionNumbers2;\n")
	versionFile.write("        std::stringstream ss2(v2);\n")
	versionFile.write("        std::string item2;\n")
	versionFile.write("        while(std::getline(ss2, item2, '.')) {\n")
	versionFile.write("            versionNumbers2.push_back(item2);\n")
	versionFile.write("        }\n")
	versionFile.write("        assert(versionNumbers2.size() == 4);\n")
	versionFile.write("        unsigned int major2 = std::stoul(versionNumbers2[0]);\n")
	versionFile.write("        unsigned int minor2 = std::stoul(versionNumbers2[1]);\n")
	versionFile.write("        unsigned int revision2 = std::stoul(versionNumbers2[2]);\n")
	versionFile.write("        unsigned int build2 = std::stoul(versionNumbers2[3]);\n")
	versionFile.write("\n")
	versionFile.write("\n")

	versionFile.write("        int resultMajor = compareInt(major1, major2);\n")
	versionFile.write("        if(resultMajor != 0) {\n")
	versionFile.write("            return resultMajor;\n")
	versionFile.write("        } else {\n")
	versionFile.write("            int resultMinor = compareInt(minor1, minor2);\n")
	versionFile.write("            if(resultMinor != 0) {\n")
	versionFile.write("                return resultMinor;\n")
	versionFile.write("            } else {\n")
	versionFile.write("                int resultRevision = compareInt(revision1, revision2);\n")
	versionFile.write("                if(resultRevision != 0) {\n")
	versionFile.write("                    return resultRevision;\n")
	versionFile.write("                } else {\n")
	versionFile.write("                    return compareInt(build1, build2);\n")
	versionFile.write("                }\n")
	versionFile.write("            }\n")
	versionFile.write("        }\n")
	versionFile.write("    }\n")
	versionFile.write("\n")

	versionFile.write("    /**\n")
	versionFile.write("     * @brief builds the full version string\n")
	versionFile.write("     * @return the full version string\n")
	versionFile.write("     */\n")
	versionFile.write("    static std::string getVersionString() {\n")
	versionFile.write("        std::stringstream versionString;\n")
	versionFile.write("        versionString << MAJOR << '.'\n")
	versionFile.write("                      << MINOR << '.'\n")
	versionFile.write("                      << REVISION << '.'\n")
	versionFile.write("                      << BUILD;\n")
	versionFile.write("        return versionString.str();\n")
	versionFile.write("    }\n")
	versionFile.write("\n")

	versionFile.write("    /**\n")
	versionFile.write("     * @brief builds a simple version string which only includes MAJOR and MINOR version numbers\n")
	versionFile.write("     * @return a simple version string\n")
	versionFile.write("     */\n")
	versionFile.write("    static std::string getSimpleVersionString() {\n")
	versionFile.write("        std::stringstream versionString;\n")
	versionFile.write("        versionString << MAJOR << '.' << MINOR;\n")
	versionFile.write("        return versionString.str();\n")
	versionFile.write("    }\n")
	versionFile.write("};\n")
	versionFile.write("#endif  // VERSION_H\n")	

	versionFile.close()


if __name__ == '__main__':
	main()