# Copyright (c) 2014-2015 Kartik Kumar (me@kartikkumar.com)
# Distributed under the MIT License.
# See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT

cmake_minimum_required(VERSION 2.6)

# Change this section to correspond with your project info.
project(TOG C CXX)
set(PROJECT_SUMMARY                            "Theory of Games -- sample application")
set(${CMAKE_PROJECT_NAME}_VENDOR_NAME          "Kartik Kumar")
set(${CMAKE_PROJECT_NAME}_VENDOR_CONTACT       "me@kartikkumar.com")
set(${CMAKE_PROJECT_NAME}_VERSION_MAJOR        0)
set(${CMAKE_PROJECT_NAME}_VERSION_MINOR        1)
set(${CMAKE_PROJECT_NAME}_VERSION_PATCH        0)
set(PROJECT_VERSION
  "${${CMAKE_PROJECT_NAME}_VERSION_MAJOR}.${${CMAKE_PROJECT_NAME}_VERSION_MINOR}.${${CMAKE_PROJECT_NAME}_VERSION_PATCH}")

set(PROJECT_PATH                               "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}     "${PROJECT_PATH}/cmake/Modules")
set(INCLUDE_PATH                               "${PROJECT_PATH}/include")
set(SRC_PATH                                   "${PROJECT_PATH}/src")
set(TEST_SRC_PATH                              "${PROJECT_PATH}/test")
if(NOT EXTERNAL_PATH)
  set(EXTERNAL_PATH                            "${PROJECT_PATH}/external")
endif(NOT EXTERNAL_PATH)
if(NOT DOXYGEN_PATH)
  set(DOXYGEN_PATH                             "${PROJECT_PATH}/doxydocs")
endif(NOT DOXYGEN_PATH)
if(NOT DOCS_PATH)
  set(DOCS_PATH                                "${PROJECT_PATH}/docs")
endif(NOT DOCS_PATH)
set(LIB_PATH                                   "${PROJECT_BINARY_DIR}/lib")
set(LIB_NAME                                   "tof_methods")
set(BIN_PATH                                   "${PROJECT_BINARY_DIR}/bin")
set(MAIN_NAME                                  "tog")
set(TEST_PATH                                  "${PROJECT_BINARY_DIR}/test")
set(TEST_NAME                                  "test_tog")

OPTION(BUILD_MAIN                              "Build main function"            ON)
OPTION(BUILD_DOXYGEN_DOCS                      "Build Doxygen docs"             OFF)
OPTION(BUILD_TESTS                             "Build tests"                    OFF)
OPTION(BUILD_DEPENDENCIES                      "Force build of dependencies"    OFF)

include(CMakeDependentOption)
CMAKE_DEPENDENT_OPTION(BUILD_COVERAGE_ANALYSIS "Build code coverage analysis"   OFF
                                               "BUILD_TESTS"                    OFF)

#set(SRC
#)

set(MAIN_SRC
  "${SRC_PATH}/tog.cpp"
)

set(TEST_SRC
  "${TEST_SRC_PATH}/testTOG.cpp"
)

# Set CMake build-type. If it not supplied by the user, the default built type is "Release".
if(((NOT CMAKE_BUILD_TYPE)
  AND (NOT BUILD_COVERAGE_ANALYSIS))
  OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
    message(STATUS "WARNING: building release version!")
    set(CMAKE_BUILD_TYPE Release)
elseif((CMAKE_BUILD_TYPE STREQUAL "Debug") OR (BUILD_COVERAGE_ANALYSIS))
    message(STATUS "WARNING: building debug version!")
endif(((NOT CMAKE_BUILD_TYPE)
  AND (NOT BUILD_COVERAGE_ANALYSIS))
  OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

# Set platform-specific compiler flags.
if(WIN32)
  if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /Ox /W3 /FC -D_SCL_SECURE_NO_WARNINGS")
    # We need to force MSVC to use static runtime:
    # (from http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace).
    foreach(flag_var
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      # Find all dynamic runtime (MD) references and replace with static (MT)
      if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      endif(${flag_var} MATCHES "/MD")
    endforeach(flag_var)
  else(MSVC)
    set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -Wold-style-cast -Wnon-virtual-dtor")
  endif(MSVC)
else(WIN32)
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -Wold-style-cast -Wnon-virtual-dtor")
endif(WIN32)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(AFTER "${INCLUDE_PATH}")

include(Dependencies.cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
#add_library(${LIB_NAME} ${SRC})
#if(NOT RAPIDJSON_FOUND)
#  add_dependencies(${LIB_NAME} rapidjson-lib)
#endif(NOT RAPIDJSON_FOUND)
#f(NOT KEPLERIANTOOLBOX_FOUND)
#  add_dependencies(${LIB_NAME} keplerian_toolbox-lib)
#endif(NOT KEPLERIANTOOLBOX_FOUND)

if(BUILD_MAIN)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})
  add_executable(${MAIN_NAME} ${MAIN_SRC})
  target_link_libraries(${MAIN_NAME}
#    ${LIB_NAME}
    ${KEPLERIANTOOLBOX_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
  )
endif(BUILD_MAIN)

if(BUILD_DOXYGEN_DOCS)
  find_package(Doxygen)

  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR
            "Doxygen is needed to build the documentation. Please make sure it is installed.")
  endif(NOT DOXYGEN_FOUND)

  configure_file(${PROJECT_PATH}/Doxyfile.in
                 ${PROJECT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)

  # Add a custom target to run Doxygen whenever the project is built.
  # If you do NOT want the documentation to be generated EVERY time you build the project
  # then leave out the 'ALL' keyword from the command below.
  add_custom_target(doxygen_docs ALL COMMAND ${DOXYGEN_EXECUTABLE}
                    ${PROJECT_BINARY_DIR}/Doxyfile
                    SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
endif(BUILD_DOXYGEN_DOCS)

if(BUILD_TESTS)
  enable_testing()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TEST_PATH})

  add_executable(${TEST_NAME} ${TEST_SRC})
  target_link_libraries(${TEST_NAME}
    ${LIB_NAME}
    ${KEPLERIANTOOLBOX_LIBRARY}
    )
  add_test(NAME ${TEST_NAME} COMMAND "${TEST_PATH}/${TEST_NAME}")

  if(BUILD_COVERAGE_ANALYSIS)
    include(CodeCoverage)
    set(COVERAGE_EXTRACT '${PROJECT_PATH}/include/*' '${PROJECT_PATH}/src/*')
    setup_target_for_coverage(coverage "${TEST_NAME}" coverage)
  endif(BUILD_COVERAGE_ANALYSIS)
endif(BUILD_TESTS)
