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

This Pyton script checks the coding style of the Playbook Creator source files.
Every source file that has changed since the last build, is given to the
"cpplint.py" script, which checks Google's code conventions. To detect that a 
file has changed, this script will calculate an md5 hash of the source file
and compare it to a previous hash from the last build that is stored in "hashRegister.txt"
"""
import os.path
import subprocess
import sys
import hashlib

def hashFileContent(afile, hasher, blocksize=65536):
    buf = afile.read(blocksize)
    while len(buf) > 0:
        hasher.update(buf)
        buf = afile.read(blocksize)
    return hasher.hexdigest()

def readPreviousHashes(filename):
	table = {}
	if(os.path.isfile(filename)):
		f = open(filename)
		for line in f.readlines():
			splitted = line.strip().split(":")
			hashedFile = splitted[0]
			hashString = splitted[1]
			table[hashedFile] = hashString

		f.close()
	return table


def writeNewHashes(hashRegister, prevHash):
	hashFile = open(hashRegister, "w+")
	for filename, hashString in prevHash.iteritems():
		hashFile.write(filename + ":" + hashString + "\n")


def main():
	root = "../src"
	filterOut = ["whitespace/parens", "build/include_order", "build/header_guard"]
	filterIn = []
	hashRegister = "hashRegister.txt"


	for i,val in enumerate(filterIn):
	    filterIn[i] = "+" + val

	for i,val in enumerate(filterOut):
		filterOut[i] = "-" + val

	filterText = ""
	if(len(filterOut) > 0 or len(filterIn) > 0):
		filterInOut = filterIn + filterOut
		filterText = "--filter=" + ",".join(filterInOut)


	prevHash = readPreviousHashes(hashRegister)
	hasStyleErrors = False

	for path, subdirs, files in os.walk(root):
	    for name in files:
	    	extension = name.split(".")[1]
	    	if(extension == "cpp" or extension == "h"):
	    		filename =  os.path.join(path, name)
	    		hashString = hashFileContent(open(filename, "rb"), hashlib.md5())
			if(filename not in prevHash or prevHash[filename] != hashString):
				try:
					output = subprocess.check_output(["python", "cpplint.py", filterText, filename])
					prevHash[filename] = hashString
				except:
					hasStyleErrors = True

	writeNewHashes(hashRegister, prevHash)
	if(hasStyleErrors == True):
		sys.exit(1)



if __name__ == '__main__':
	main()