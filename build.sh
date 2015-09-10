#!/bin/bash

baseDir=$(pwd)"/"

toolDir=$baseDir"tools/"
buildDir=$baseDir"build/"
srcDir=$baseDir"src/"
binDir=$baseDir"bin/"

MAJOR_VERSION_NUMBER=0
MINOR_VERSION_NUMBER=4

echo "base directory: " $baseDir
echo "build directory: " $buildDir

set -e
(cd $baseDir && python version.py $MAJOR_VERSION_NUMBER $MINOR_VERSION_NUMBER)
(cd $baseDir && mkdir -p $buildDir)
(cd $baseDir && cppcheck $srcDir --error-exitcode=2 --suppress=unusedFunction --inline-suppr --template='{file}:{line}: ({severity}) {message} [{id}]' --enable=all -q)
(cd $toolDir && python checkStyle.py)
(cd $buildDir && qmake $srcDir"PlaybookCreator.pro" -r -spec linux-g++-64 CONFIG+=debug)
(cd $buildDir && make)
(cd $baseDir && mkdir -p $binDir)
(mv $buildDir"PlaybookCreator" $binDir)
(cd $baseDir && doxygen)