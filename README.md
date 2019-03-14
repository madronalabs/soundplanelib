# Soundplanelib	

Loew-level support and and example application for the Madrona Labs Soundplane.

Copyright (c) 2019 Madrona Labs LLC. http://www.madronalabs.com

Distributed under the MIT license: http://madrona-labs.mit-license.org/

## Building

### OS X

To compile this project, the Xcode Command Line Tools and CMake are required.
Xcode Command Line Tools can be downloaded and installed from the "Downloads"
tab in Xcode's settings. An easy way of installing CMake is to do it via
Homebrew: With Homebrew installed, type `brew install cmake` in a terminal.

In order to prepare the build, the following commands can be used. 

    $ mkdir build
    $ cd build
    $ cmake -GXcode ..


At this point, there is an Xcode project `soundplanelib.xcodeproj` ready for coding
and compiling in `build/`. If you just want to build the example application and run it
from the comfort of the terminal, these commands can be used

    $ xcodebuild -project soundplanelib.xcodeproj -target helloSoundplane -configuration Debug
    $ open examples/Debug/helloSoundplane

### Linux

## NOTE: UNTESTED FOR SOUNDPLANELIB

On Linux, the libusb library is used for USB communications. On Ubuntu, the following 
command should install it:

    $ sudo apt-get install cmake libusb-1.0.0-dev 

In order to fetch source dependencies and build, the following commands can be
used (in a terminal)

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

On Linux, the Soundplane application typically uses udev to access Soundplane
via USB. On most stock Linux installations, the default configuration is to block
non-superuser access to the Soundplane. In order to allow the Soundplane
application to access the device, add a udev rule that allows it:

    $ sudo cp Data/59-soundplane.rules /etc/udev/rules.d/
    $ sudo udevadm control --reload-rules

The app can now be run with

    $ examples/helloSoundplane

If desired, it is possible to build a Debian package with the command

    $ make Soundplane_deb
