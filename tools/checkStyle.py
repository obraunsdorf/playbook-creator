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
	filterOut = ["legal", "whitespace/parens", "build/include_order", "build/header_guard"]
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