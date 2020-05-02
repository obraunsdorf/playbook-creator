Playbook Creator: A free, open-source editor for American (Flag) Football Playbooks
====================================================================================
This is a free editor for offensive playbooks in American Football. 
It is primarily dedicated to the flag football community as most of the teams (at least in Germany) cannot afford a commercial tool.

Playbook Creator is mainly developed and tested on Linux, but you can download and run it on all major operating systems.
  * MacOS X [![Build Status](https://travis-ci.org/obraunsdorf/playbook-creator.svg?branch=master)](https://travis-ci.org/obraunsdorf/playbook-creator)
  * Windows 10 [![Build status](https://ci.appveyor.com/api/projects/status/c0n8v494adn2kfmh/branch/master?svg=true)](https://ci.appveyor.com/project/obraunsdorf/playbook-creator) 
  * Linux (Ubuntu 18.04) [![CircleCI](https://circleci.com/gh/obraunsdorf/playbook-creator.svg?style=svg)](https://circleci.com/gh/obraunsdorf/playbook-creator)

Mailing list:
  * send email (without registration): <pbc-users@freelists.org>
  * subscribe for new messages/announcements: <http://www.freelists.org/list/pbc-users>


**I'm looking for developers who can help out on PBC. Any help is welcome. Please see the section `How to contribute to PBC` at the bottom.**


## How to install PBC
  * **On Windows**   
  Download the latest release of Playbook Creator for Windows at https://github.com/obraunsdorf/playbook-creator/releases.  
  This is a setup file. Please follow the instructions of the setup to complete the installation.
  * **On MacOS X**  
  Download the latest release of Playbook Creator for MacOSX at https://github.com/obraunsdorf/playbook-creator/releases.  
  This is a DMG-file. Just open it and drag&drop the PlaybookCreator app to your Applications folder.  
  => Open PlaybookCreator by right-clicking on it and select "Open".  
  If you try to open it via double-click MacOS will complain that it cannot open the app because it is from an unidentified developer. This is because I am not a registered as a developer on Apple. I can think about registering there if that is important to someone.
  * **On Linux**
    1. Install Qt: `apt-get install qt5-default`
    2. Download Playbook Creator for Linux at https://github.com/obraunsdorf/playbook-creator/releases
    3. Make it executable: `chmod +x path/to/PlaybookCreator`
    4. Run it from the console: `path/to/PlaybookCreator`

## How to use PBC
Most of the user interface of Playbook Creator should be self-explainable. There is the main menu which can be used to, e.g.
 * create/open/safe a new playbook
 * create/open/safe/delete a play
 * export the playbook as pdf, arranged for using as a wrist coach

When creating a new playbook, you can select the number of players (5, 7, 9 or 11). PBC is mostly tested for 5on5 playbooks.

The next subsections shall be a little guide on how to create new plays and export a wrist coach from your playbook.

### Creating a New Formation
In the main menu click `Play -> New play`. Enter whatever you want for play name / code name (these are discussed in the next section) and click OK. Move the players around using Drag&Drop. You can also set a color for a player by right-clicking on it and select `Set color`. If the formation is aligned as you wish, click `Play -> Save formation as` in the main menu.

### Creating a New Play
In the main menu click `Play -> New play`. Enter a name for your play. The play name will be displayed in the bottom of the graphic.  
You can optionally choose a code name. If you set a code name then this one will be displayed in the bottom of the graphic instead of the original play name.  
We used this because our play names were super obvious. Therefore we had to use code names on our wrist coaches so that we could shout them out to make an audible at the LOS or call the play in a No Huddle Offense  

Choose the formation you like and click OK

Now in the main screen, you can **right-click** on a player to
 * apply a route (from the route selection menu)
 * apply a motion
 * set her/his color

You can also **double-click** on a player to quickly apply a route.  
Move your mouse and click to set a route point. While moving your mouse, you can also **Press 'Ctrl'** to draw a curved route.  
**Double-click** again to set the end of the route.

If you designed your play don't forget to save it. In the main menu click `Play -> Save play`

### Using Categories
After saving a play, you can assign it to custom categories (e.g. "run plays", "4th down plays", etc.). Click `Play -> Edit play's categories` in the main menu. There you can create new categories and assign the play to them.  
This is especially helpful if you later want to open plays of specific categories. The *Open play* dialog lets you apply category filters.

### Creating a New Route
The quickly applied routes will not be added to your playbook. If you want to create new routes that shall be available in your route selection menu in the future, please do the following steps.

  1. Right-click on a player
  2. Select `Routes -> create (named) route`
  3. Enter the route name (code name is optional here and will be displayed in the route selection menu) and click OK
  4. Now proceed moving your mouse and set route points as usual.
  5. Double click to set the end of the route and save the route. 
 

### Creating a Wrist Coach
If you created some plays, click `Playbook -> Create Wrist Coach (PDF)` in the main menu.
A dialog will show up where you can see your plays on the left side. 
  1. Select every play you want to have on your wrist coach and use the button with the right arrow place them on the right list.
  2. Use the buttons with the up and down arrow to change the ordering of the plays on the right site according to your planned wrist coach ordering
  3. Enter the columns and rows your wrist coach should have
  4. Enter the dimensions of one page that will fit into your physical wrist coach (in millimeters). The exported PDF will have exactly this size.
  5. Most of the physical wrist coaches overlap the play sheets in some areas. You can select margins were nothing should be printed. The margins will be marked with a red dashed line 
  6. Click OK and select the file name where you want to save your PDF.
  7. Enjoy ;)

The created PDFs look will look like this one.

<a href="manual/images/WristCoach.png"><img width="600" src="manual/images/WristCoach.png"></a>

If you quickly want to print your playbook. You can skip entering dimensions and margins. The page dimensions will be calculated automatically using your specified column and row count.


### Deleting Plays etc.
If you want to delete some particular plays, categories, formations or routes from the playbook, simply click `Delete` in the main menu and choose accordingly.  
Deleting a formation or route will not have an impact on existing plays. The players will keep their routes and their positions/alignments.  
Deleting a category will not delete the plays within that category. It will only un-assign all plays from that category.


### Renaming Plays
To rename a play you have to save it under a different name and then delete the old one. In particular:
  1. Open the play you want to rename
  2. Click `Play -> Save Play as` and choose a different play name (code name can stay the same as before).
  3. Afterwards click `Delete -> Plays` and choose the old one to delete it.

This is a little bit inconvenient. There will be a feature to easily rename plays, soon -- or sooner if you help programming PBC ;-)  (see issue [#25](https://github.com/obraunsdorf/playbook-creator/issues/25))


### Importing from another Playbook
You can import another playbook. This is useful, e.g., when your teammates had some ideas for new plays and want to send them to you to import it in the team's playbook.  
Just click `Playbook -> Import Playbook` in the main menu.

This feature is not fully complete yet. You can see the current progress in issue [#10](https://github.com/obraunsdorf/playbook-creator/issues/10).  
At the moment, when importing another playbook, it automatically imports all plays, categories, and formations from the other playbook. To make it easily visible which plays have been imported, their name is prefixed with 'imported_'. You can rename those plays after importing or you can delete them if you don't like them (see the sections above).

Routes are not imported. Importing of routes is easily possible but I disabled it until we have a GUI dialog for import settings. If you urgently need to import routes, please make a comment to issue [#10](https://github.com/obraunsdorf/playbook-creator/issues/10) or write an email to the mailing list.


### Further Help and Discussion
If you have any questions or want to discuss about Playbook Creator, you can use the issue tracking system of Github https://github.com/obraunsdorf/playbook-creator/issues.  
Alternatively you can write an email to the following mailing list <pbc-users@freelists.org>.  
You can also subscribe to the mailing list here, to receive an email when new versions of PBC are released: http://www.freelists.org/list/pbc-users.

Note: A (quite outdated) manual in PDF format is available [here](manual/pbcManual.pdf). The last page contains a step-by-step guideline to create a new playbook from scratch. I'll try to update the documentation in a new fancy manner as soon as I can.

## How to build PBC
For anyone who wants to build the application from source on Linux (and make changes), you can run

    apt-get install build-essential pkg-config curl git cmake libbotan-2-dev qt5-default libboost-serialization-dev libboost-test-dev libboost-filesystem-dev libssl-dev
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

    git clone https://github.com/obraunsdorf/playbook-creator.git
    cd playbook-creator
    mkdir build
    cd build
    cmake ..
    make

To run unit tests, you can run

    make -j4 tests && ASAN_OPTIONS=detect_leaks=0 bin/tests --log_level=all -- --test-base-dir "test"



Building on MacOS X is done similarly using homebrew. Please refer to [the TravisCI script](.travis.yml).
For Building on Windows you can set up a [Appveyor](https://www.appveyor.com/)-like environment with Qt5 and Boost and refer to [the Appveyor CI script](appveyor.yml) accordingly.


## How to contribute to PBC
Feel free to contribute on any aspects of Playbook Creator: Front-end design, back-end code, crypto, documentation, website, etc. If you don't know where to start, please have a look at <https://github.com/obraunsdorf/playbook-creator/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22>. Of course, you can also write me if you need some introduction to the code base or any other guidance.

If you have any concrete idea, bug fixes, or even new code, you can either send me an email at <oliver.braunsdorf@gmx.de> or you can create an issue at <https://github.com/obraunsdorf/playbook-creator/issues>

If you are used to the github workflow, I am happy about every fork and pull request ;)
