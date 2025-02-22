cmake_minimum_required(VERSION 3.16)

project(Chat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Network Multimedia Concurrent)
find_package(miniocpp CONFIG REQUIRED)
qt_standard_project_setup(REQUIRES 6.5)

FILE(GLOB_RECURSE FILE_HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)
aux_source_directory(${PROJECT_SOURCE_DIR}/src FILE_SOURCES)

qt_add_executable(appChat
    main.cpp
    res.qrc
    FramelessWindow/QuickFramelessWindow.cpp
    FramelessWindow/QuickFramelessWindow.h
    FramelessWindow/private/QuickFramelessWindowPrivate.cpp
    FramelessWindow/private/QuickFramelessWindowPrivate.h
    ${FILE_HEADERS}
    ${FILE_SOURCES}
)

qt_add_qml_module(appChat
    URI Chat
    VERSION 1.0
    QML_FILES
        Main.qml
        MainWindow.qml
        NATitleBar.qml
        NAFeatureBar.qml
        MessageContainer.qml
        MessageListView.qml
        Global.qml
        ChatWindow.qml
        MessageItem.qml
        TextMessageItem.qml
        Login.qml
        Register.qml
        CustomComboBox.qml
        AddFriendContainer.qml
        MyFriendContainer.qml
        MyFriendListView.qml
        CustomMenu.qml
        SearchResult.qml
        NotificationContainer.qml
        UserInfoContainer.qml
        CustomProgressBar.qml
        FileMessageItem.qml
        PictureMessageItem.qml
        CreateGroupContainer.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appChat PROPERTIES
    #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appChat
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appChat
    PRIVATE Qt6::Quick Qt6::Network Qt6::Multimedia miniocpp::miniocpp Qt6::Concurrent
    dwmapi
)

include(GNUInstallDirs)
install(TARGETS appChat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
