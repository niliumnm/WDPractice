cmake_minimum_required(VERSION 3.5)

project(WDPractice VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE PROJECT_SOURCES
    "WDDB/*.cpp"
    "WDDB/*.h"
    "Midi/*.h"
    "Midi/*.cpp"
    "GUI/*.cpp"
    "GUI/*.h"
)

link_directories("D:\\Code\\WDPractice\\library\\libremidi")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WDPractice
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(WDPractice
        ${PROJECT_SOURCES}
    )
endif()

# 指定.h头文件目录


target_link_libraries(WDPractice PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(WDPractice PRIVATE libremidi)
target_link_libraries(WDPractice PRIVATE winmm)

# 指定.dll链接库文件目录
include_directories("D:\\Code\\WDPractice\\include")

target_include_directories(WDPractice PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"  # 使可以直接识别#include"WDDB/wddn.h" 而无需相对路径
    # 如果有多个目录，可以继续添加
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.WDPractice)
endif()
set_target_properties(WDPractice PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS WDPractice
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WDPractice)
endif()
