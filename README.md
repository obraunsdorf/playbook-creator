Playbook Creator: A free, open-source editor for American (Flag) Football Playbooks
====================================================================================
This is a free editor for offensive playbooks in American Football. 
It is primarily dedicated to the flag football community as most of the teams (at least in Germany) cannot afford a commercial tool.

Playbook Creator is mainly developed and tested on Linux, but you can download and run it on all major operating systems.
  * MacOS X [![Build Status](https://travis-ci.org/obraunsdorf/playbook-creator.svg?branch=master)](https://travis-ci.org/obraunsdorf/playbook-creator)
  * Windows 10 [![Build status](https://ci.appveyor.com/api/projects/status/c0n8v494adn2kfmh?svg=true)](https://ci.appveyor.com/project/obraunsdorf/playbook-creator) 
  * Linux (Ubuntu 18.04) [![CircleCI](https://circleci.com/gh/obraunsdorf/playbook-creator.svg?style=svg)](https://circleci.com/gh/obraunsdorf/playbook-creator)


## How to install
  * **On Windows / MacOS X :**   
  Just download the latest release of Playbook Creator for your OS at https://github.com/obraunsdorf/playbook-creator/releases
  * **On Linux :**
    1. Install Qt:
        `apt-get install qt5-default`
    2. Download Playbook Creator for Linux at https://github.com/obraunsdorf/playbook-creator/releases

## Documentation
Most of the user interface of Playbook Creator should be self-explainable. There is the main menu which can be used to, e.g.
 * create/open/safe a new playbook
 * create/open/safe/delete a play
 * export the playbook as pdf, arranged for using as a wrist coach

Moreover you can **right-click** on a player to
 * apply a route / create a new route
 * apply a motion
 * set her color
 
 You can also **double-click** on a player to quickly **apply a route**.  
 **Press 'Ctrl' and move your mouse** during route creation **to draw curved routes**.
 
Note: A (quite outdated) manual in PDF format is available [here](manual/pbcManual.pdf). The last page contains a step-by-step guideline to create a new playbook from scratch. I'll try to update the documentation in a new fancy manner as soon as I can.


## How to build
For anyone who wants to build the application from source on Linux (and make changes), you can run

    apt-get install build-essential pkg-config git cmake qt5-default libbotan-2-dev libboost-serialization-dev

    git clone https://github.com/obraunsdorf/playbook-creator.git
    cd playbook-creator
    cmake .
    make

Building on MacOS X is done similarly using homebrew. Please refer to [the TravisCI script](.travis.yml).
For Building on Windows you can set up a [Appveyor](https://www.appveyor.com/)-like environment with Qt5 and Boost and refer to [the Appveyor CI script](appveyor.yml) accordingly.


## How to contribute
Feel free to contribute on any aspects of Playbook Creator: Front-end design, back-end code, crypto, documentation, website, etc. If you don't know where to start, please have a look at <https://github.com/obraunsdorf/playbook-creator/issues?q=is%3Aopen+is%3Aissue+label%3A%22help+wanted%22>

If you have any concrete idea, bug fixes, or even new code, you can either send me an email at <oliver.braunsdorf@gmx.de> or you can create an issue at <https://github.com/obraunsdorf/playbook-creator/issues>

If you are used to the github workflow, I am happy about every fork and pull request ;)
