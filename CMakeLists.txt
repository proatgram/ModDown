# Includes
include(FindDoxygen)
include(FindPkgConfig)
find_package(PkgConfig REQUIRED)

## Sets the project variables ##
cmake_minimum_required(VERSION 3.16)

EXECUTE_PROCESS (COMMAND getconf LONG_BIT COMMAND tr -d '\n' OUTPUT_VARIABLE ARCH)
EXECUTE_PROCESS (COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE PCT)
if (${PCT} STREQUAL "x86_64")
	set (ARCHITECTURE "amd64")
elseif(${PCT} STREQUAL "i386")
	set (ARCHITECTURE "i386")
elseif(${PCT} STREQUAL "arm")
	set (ARCHITECTURE "arm${ARCH}")
else()
	message(FATAL_ERROR "Unknown Architecture type.")
endif()
message(STATUS "Detected architecture: ${ARCHITECTURE}")

project (ModDown VERSION "2.01")
set (CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ECLIPSE_VERSION 4.23.0)
# CPack variables

set (CPACK_GENERATOR "TGZ" "DEB" "RPM")
set (CMAKE_PROJECT_HOMEPAGE_URLE "https://github.com/proatgram/ModDown")
set (CPACK_PACKAGE_DESCRIPTION "A tool to download all the mods in a ModPack from CurseForge.")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CPACK_PACKAGE_DESCRIPTION})
set (CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set (CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set (CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${ARCHITECTURE}")
set (CPACK_PACKAGE_CONTACT "tgh6407@gmail.com")
set (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set (CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
set (CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set (CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${ARCHITECTURE})
set (CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set (CPACK_SOURCE_IGNORE_FILES "build/;/build/;//.git;.git;deps/;")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# Finds libraries needed for program
find_library(CURL curl libcurl)
find_library(ZIP zip libzip)
find_library(PTHREAD pthread libpthread)

pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(nlohmann_json REQUIRED nlohmann_json>=3.10.5)
pkg_search_module(indicators REQUIRED indicators>=2.2.0)

## Configures targets ##
file (GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c")
file (GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/include/*.hpp" "${CMAKE_SOURCE_DIR}/include/*.inl")

add_executable (ModDown ${SOURCES} ${HEADERS})
target_link_libraries(ModDown ${CURL} ${ZIP} ${PTHREAD} ${GLIB_LDFLAGS})
target_include_directories(ModDown PUBLIC ${CMAKE_SOURCE_DIR}/include PRIVATE ${GLIB_INCLUDE_DIRS})
target_compile_options(ModDown PUBLIC "-std=c++17" "-g" )
include(GNUInstallDirs)
## Adds installation instructions ##
install (TARGETS ModDown RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT applications)
include(CPack)
