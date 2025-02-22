cmake_minimum_required(VERSION 3.16)

project(Server LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Network)
find_package(miniocpp CONFIG REQUIRED)
add_executable(Server
  main.cpp
  miniomanager.h miniomanager.cpp
  clientcontainer.h clientcontainer.cpp
  clientwork.h clientwork.cpp
  userdatabasemanager.h userdatabasemanager.cpp
  globalproperties.h globalproperties.cpp
)
target_link_libraries(Server Qt${QT_VERSION_MAJOR}::Core Qt6::Sql Qt6::Network miniocpp::miniocpp)

include(GNUInstallDirs)
install(TARGETS Server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
