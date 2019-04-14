# Install script for directory: /home/gaoyi/workspace/StaticNFrame/Server/StaticNFrame/NFComm

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "DynamicDebug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFCore/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFMongoPlugin/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFActorPlugin/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFKernelPlugin/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFMath/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFNetPlugin/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFPluginModule/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFMysqlPlugin/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFXPlugin/NFLuaScriptPlugin/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFSqlitePlugin/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFMonitorPlugin/cmake_install.cmake")
  include("/home/gaoyi/workspace/StaticNFrame/Product/build_dynamic_debug/Server/StaticNFrame/NFComm/NFNoSqlPlugin/cmake_install.cmake")

endif()

