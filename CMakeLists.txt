cmake_minimum_required(VERSION 3.19)
project(flashcards LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

qt_add_executable(flashcards
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    README.md
    resource.qrc
    app.rc
)

set_target_properties(flashcards PROPERTIES
   QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")

qt_add_translations(
    TARGETS flashcards
    TS_FILES flashcards_ja_JP.ts
)

target_link_libraries(flashcards
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS flashcards
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET flashcards
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
