#!/bin/bash

### only edit here #################################
baseDir="/home/oliver/workspace/playbook-creator/" #
####################################################


toolDir=$baseDir"tools/"
buildDir=$baseDir"build-PlaybookCreator-Desktop-Debug/"
srcDir=$baseDir"src/"

MAJOR_VERSION_NUMBER=0
MINOR_VERSION_NUMBER=2

echo "build directory: " $buildDir

set -e
(cd $baseDir && python version.py $MAJOR_VERSION_NUMBER $MINOR_VERSION_NUMBER)
(cd $baseDir && mkdir -p $buildDir)
(cd $baseDir && cppcheck $srcDir --error-exitcode=2 --suppress=unusedFunction --inline-suppr --template='{file}:{line}: ({severity}) {message} [{id}]' --enable=all -q)
(cd $toolDir && python checkStyle.py)
(cd $buildDir && qmake $srcDir"PlaybookCreator.pro" -r -spec linux-g++-64 CONFIG+=debug)
(cd $buildDir && make)