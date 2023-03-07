# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMake docs.

cmake_minimum_required(VERSION 3.25)

# The top-level CMakeLists.txt file for a project must contain a literal, direct call to the
# `project()` command. `project()` sets up some helpful variables that describe source/binary
# directories, and the current project version. This is a standard CMake command.

project(juceDjApp VERSION 0.0.1)

# Set our C++ version to 17
set(CMAKE_CXX_STANDARD 17)

# By default we don't want Xcode schemes to be made for modules, etc
set(CMAKE_XCODE_GENERATE_SCHEME OFF)

# Go get JUCE using CMake's "FetchContent"
set(FETCHCONTENT_QUIET OFF)

include (FetchContent)

option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

FetchContent_Declare (
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG origin/master
    GIT_SHALLOW ON
    FIND_PACKAGE_ARGS 7.0.3 GLOBAL)

FetchContent_Declare (
    taglib
    GIT_REPOSITORY https://github.com/taglib/taglib.git
    GIT_TAG origin/master
    GIT_SHALLOW ON)

FetchContent_MakeAvailable (JUCE)
FetchContent_MakeAvailable (taglib)

if(taglib_POPULATED)
    get_property(dirs DIRECTORY "${taglib_SOURCE_DIR}/taglib" PROPERTY INCLUDE_DIRECTORIES)
    target_include_directories(tag PUBLIC ${dirs} "${taglib_SOURCE_DIR}/taglib")
endif()

juce_add_gui_app(juceDjApp)


# We want all source code in the source directory
file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h")

# No, we don't want our source buried in extra nested folders
set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")

# The Xcode source tree should look like the source tree
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "" FILES ${SourceFiles})


# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

target_sources("${PROJECT_NAME}" PRIVATE ${SourceFiles})

target_link_libraries(juceDjApp PRIVATE
    juce::juce_analytics
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_core
    juce::juce_data_structures
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    tag)

