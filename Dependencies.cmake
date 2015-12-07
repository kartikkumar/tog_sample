# Copyright (c) 2014-2015 Kartik Kumar (me@kartikkumar.com)
# Distributed under the MIT License.
# See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT

# Include script to build external library with CMake.
include(ExternalProject)

if(NOT BUILD_DEPENDENCIES)
  find_package(rapidjson)
endif(NOT BUILD_DEPENDENCIES)

if(NOT RAPIDJSON_FOUND)
  message(STATUS "RapidJSON will be downloaded when ${CMAKE_PROJECT_NAME} is built")
  ExternalProject_Add(rapidjson-lib
    PREFIX ${EXTERNAL_PATH}/RapidJson
    #--Download step--------------
    URL https://github.com/miloyip/rapidjson/archive/master.zip
    TIMEOUT 30
    #--Update/Patch step----------
    #--Configure step-------------
    CONFIGURE_COMMAND ""
    #--Build step-----------------
    BUILD_COMMAND ""
    #--Install step---------------
    INSTALL_COMMAND ""
    #--Output logging-------------
    LOG_DOWNLOAD ON
  )
  ExternalProject_Get_Property(rapidjson-lib source_dir)
  set(RAPIDJSON_INCLUDE_DIRS ${source_dir}/include
    CACHE INTERNAL "Path to include folder for RapidJSON")
endif(NOT RAPIDJSON_FOUND)

if(NOT APPLE)
  include_directories(SYSTEM AFTER "${RAPIDJSON_INCLUDE_DIRS}")
else(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem \"${RAPIDJSON_INCLUDE_DIRS}\"")
endif(NOT APPLE)

# -------------------------------

if(NOT BUILD_DEPENDENCIES)
  find_package(KeplerianToolbox)
endif(NOT BUILD_DEPENDENCIES)

if(NOT KEPLERIANTOOLBOX_FOUND)
  message(STATUS "KeplerianToolbox will be downloaded when ${CMAKE_PROJECT_NAME} is built")
  ExternalProject_Add(keplerian_toolbox-lib
    PREFIX ${EXTERNAL_PATH}/KeplerianToolbox
    #--Download step--------------
    URL https://github.com/esa/pykep/archive/master.zip
    TIMEOUT 120
    #--Update/Patch step----------
    #--Configure step-------------
    #--Build step-----------------
    BUILD_IN_SOURCE 1
    #--Install step---------------
    INSTALL_COMMAND ""
    #--Output logging-------------
    LOG_DOWNLOAD ON
  )
  ExternalProject_Get_Property(keplerian_toolbox-lib source_dir)
  set(KEPLERIANTOOLBOX_INCLUDE_DIRS ${source_dir}/src
    CACHE INTERNAL "Path to include folder for KeplerianToolbox")
  set(KEPLERIANTOOLBOX_LIBRARY_DIR ${source_dir}/src
    CACHE INTERNAL "Path to include folder for KeplerianToolbox")
  set(KEPLERIANTOOLBOX_LIBRARY "keplerian_toolbox_static")
endif(NOT KEPLERIANTOOLBOX_FOUND)

if(NOT APPLE)
  include_directories(SYSTEM AFTER "${KEPLERIANTOOLBOX_INCLUDE_DIRS}")
else(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem \"${KEPLERIANTOOLBOX_INCLUDE_DIRS}\"")
endif(NOT APPLE)
link_directories(${KEPLERIANTOOLBOX_LIBRARY_DIR})

# -------------------------------

if(BUILD_TESTS)
  if(NOT BUILD_DEPENDENCIES)
    find_package(CATCH)
  endif(NOT BUILD_DEPENDENCIES)

  if(NOT CATCH_FOUND)
    message(STATUS "Catch will be downloaded when ${CMAKE_PROJECT_NAME} is built")
    ExternalProject_Add(catch
      PREFIX ${EXTERNAL_PATH}/Catch
      #--Download step--------------
      URL https://github.com/kartikkumar/Catch/archive/master.zip
      TIMEOUT 30
      #--Update/Patch step----------
      UPDATE_COMMAND ""
      PATCH_COMMAND ""
      #--Configure step-------------
      CONFIGURE_COMMAND ""
      #--Build step-----------------
      BUILD_COMMAND ""
      #--Install step---------------
      INSTALL_COMMAND ""
      #--Output logging-------------
      LOG_DOWNLOAD ON
    )
    ExternalProject_Get_Property(catch source_dir)
    set(CATCH_INCLUDE_DIRS ${source_dir}/include CACHE INTERNAL "Path to include folder for Catch")
  endif(NOT CATCH_FOUND)

  if(NOT APPLE)
    include_directories(SYSTEM AFTER "${CATCH_INCLUDE_DIRS}")
  else(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem \"${CATCH_INCLUDE_DIRS}\"")
  endif(NOT APPLE)

  if(BUILD_TESTS_WITH_EIGEN)
    if(NOT BUILD_DEPENDENCIES)
      find_package(Eigen3)
    endif(NOT BUILD_DEPENDENCIES)

    if(NOT EIGEN3_FOUND)
      message(STATUS "Eigen will be downloaded when ${CMAKE_PROJECT_NAME} is built")
      ExternalProject_Add(eigen-lib
        PREFIX ${EXTERNAL_PATH}/Eigen
        #--Download step--------------
        URL http://bitbucket.org/eigen/eigen/get/3.2.2.tar.gz
        TIMEOUT 30
        #--Update/Patch step----------
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        #--Configure step-------------
        CONFIGURE_COMMAND ""
        #--Build step-----------------
        BUILD_COMMAND ""
        #--Install step---------------
        INSTALL_COMMAND ""
        #--Output logging-------------
        LOG_DOWNLOAD ON
      )
      ExternalProject_Get_Property(eigen-lib source_dir)
      set(EIGEN3_INCLUDE_DIR ${source_dir} CACHE INTERNAL "Path to include folder for Eigen")
    endif(NOT EIGEN3_FOUND)

    if(NOT APPLE)
      include_directories(SYSTEM AFTER "${EIGEN3_INCLUDE_DIR}")
    else(APPLE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem \"${EIGEN3_INCLUDE_DIR}\"")
    endif(NOT APPLE)
  endif(BUILD_TESTS_WITH_EIGEN)
endif(BUILD_TESTS)
