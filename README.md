# JUCE DJ App

## Overview
This is the code for a new weekly livestream that I've been working on to show progress building a DJ app using C++ and JUCE.  I thought this would be a great way for us to learn and grow together.  Follow along on the [YouTube Channel](https://youtube.com/theaudioprogrammer)!

[Here's a rough roadmap](https://quartz-asparagus-70f.notion.site/DJ-App-in-C-JUCE-404e1459800f4b268ee70df3ee56cb40) of what I'm looking to achieve on this project.  Please feel free to make comments or suggestions!

## Compiling
I've finally taken the plunge and started using CMake.  Don't be scared!  If I can do it, you can too.  You're going to need some basic command line skills, and of course...CMake!  Download it [here](https://cmake.org/download/) and install it.

From the main repository folder, you will see `CMakeLists.txt`.  In the command line, change the directory (`cd`) to this folder.  Now you need to invoke CMake from the command line to create a Builds folder where a project in the IDE of your choice will reside.  For me, this is XCode, so I can do this with:

`cmake -B Builds -G Xcode`

Check out this [awesome tutorial](https://melatonin.dev/blog/how-to-use-cmake-with-juce/) from Sudara on further info on CMake and JUCE.  It takes a second to learn but it will greatly enhance your capabilities as a developer!

## Dependencies
**Note:** These are already pulled in using CMake's "Fetch Content" so you don't need to clone these manually yourself.  I've already taken care of it for you!

[Taglib](https://github.com/taglib/taglib) - Reading and writing audio file metadata

[XWax](https://github.com/xwax/xwax) - Digital Vinyl System (DVS) control

## License
This source code is licensed under the GNU General Public License (GPLv3) agreement. 
