from __future__ import print_function
import sys
import subprocess
import datetime

def main():
	versionFileName = "src/pbcVersion.h"
	BUILD = 150424

	
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

	versionFile.write("#ifndef VERSION_H\n")
	versionFile.write("#define VERSION_H\n")
	versionFile.write("\n")

	versionFile.write("#include <string>\n")
	versionFile.write("#include <sstream>\n")
	versionFile.write("#include <vector>\n")
	versionFile.write("\n")

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
	versionFile.write("    static int compare(const std::string& versionString) {\n")
	versionFile.write("        std::vector<std::string> versionNumbers;\n")
	versionFile.write("        std::stringstream ss(versionString);\n")
	versionFile.write("        std::string item;\n")
	versionFile.write("        while(std::getline(ss, item, '.')) {\n")
	versionFile.write("            versionNumbers.push_back(item);\n")
	versionFile.write("        }\n")
	versionFile.write("        assert(versionNumbers.size() == 4);\n")
	versionFile.write("        unsigned int otherMAJOR = std::stoul(versionNumbers[0]);\n")
	versionFile.write("        unsigned int otherMINOR = std::stoul(versionNumbers[1]);\n")
	versionFile.write("        unsigned int otherREVISION = std::stoul(versionNumbers[2]);\n")
	versionFile.write("        unsigned int otherBUILD = std::stoul(versionNumbers[3]);\n")
	versionFile.write("        int resultMAJOR = compareInt(MAJOR, otherMAJOR);\n")
	versionFile.write("        if(resultMAJOR != 0) {\n")
	versionFile.write("            return resultMAJOR;\n")
	versionFile.write("        } else {\n")
	versionFile.write("            int resultMINOR = compareInt(MINOR, otherMINOR);\n")
	versionFile.write("            if(resultMINOR != 0) {\n")
	versionFile.write("                return resultMINOR;\n")
	versionFile.write("            } else {\n")
	versionFile.write("                int resultREVISION = compareInt(REVISION, otherREVISION);\n")
	versionFile.write("                if(resultREVISION != 0) {\n")
	versionFile.write("                    return resultREVISION;\n")
	versionFile.write("                } else {\n")
	versionFile.write("                    return compareInt(BUILD, otherBUILD);\n")
	versionFile.write("                }\n")
	versionFile.write("            }\n")
	versionFile.write("        }\n")
	versionFile.write("    }\n")
	versionFile.write("\n")

	versionFile.write("    static std::string getVersionString() {\n")
	versionFile.write("        std::stringstream versionString;\n")
	versionFile.write("        versionString << MAJOR << '.'\n")
	versionFile.write("                      << MINOR << '.'\n")
	versionFile.write("                      << REVISION << '.'\n")
	versionFile.write("                      << BUILD;\n")
	versionFile.write("        return versionString.str();\n")
	versionFile.write("    }\n")
	versionFile.write("\n")

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