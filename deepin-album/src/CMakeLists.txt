cmake_minimum_required(VERSION 3.1)

set(APP_BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin/)
set(BIN_NAME ${CMAKE_PROJECT_NAME})
set(LIB_NAME dtkqml-${CMAKE_PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)
#list (APPEND RESOURCES ${CMAKE_PROJECT_NAME}.qrc)

# Set sources
set(SRC
    launcherplugin.cpp
    )

# 源文件
file(GLOB_RECURSE SRCS
    ./src/*.h
    ./src/*.cpp
    )

file(GLOB_RECURSE QMLSRC
    ./*.qml
    )

# Find the Qt5Quick library
find_package(Qt5Quick CONFIG REQUIRED)
find_package(Qt5QuickCompiler REQUIRED)
find_package(Qt5DBus CONFIG REQUIRED)
find_package(Qt5Concurrent CONFIG REQUIRED)
find_package(Qt5Svg CONFIG REQUIRED)
find_package(Qt5PrintSupport CONFIG REQUIRED)
find_package(Qt5LinguistTools CONFIG REQUIRED)
find_package(Qt5Sql CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)

qtquick_compiler_add_resources(RCC_SOURCES ${RESOURCES})

pkg_check_modules(3rd_lib REQUIRED
    dtkwidget  gio-qt udisks2-qt5
    )

# 保证 src 目录下头文件全局可见
include_directories(src)

# 同时生成 exe 和 lib 两个文件
# lib 文件有两个作用：
# (1) 开发阶段本地调试使用时，lib被exe以链接库形式使用；
# (2) 实际使用时，lib 会被安装到指定目录，lib 被 booster-dtkdeclarative 以插件形式解析


file(GLOB TS LIST_DIRECTORIES false translations/${CMAKE_PROJECT_NAME}*.ts)
set_source_files_properties(${TS} PROPERTIES OUTPUT_LOCATION ${PROJECT_SOURCE_DIR}/translations)
qt5_create_translation(QM ${SRC} ${SRCS} ${RCC_SOURCES} ${QMLSRC} ${GLOB})

if (NOT (${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    qt5_create_translation(QM ${SRC} ${SRCS} ${RCC_SOURCES} ${QMLSRC} ${TS})
endif ()

# Add LIB
add_library(${LIB_NAME} SHARED
    ${SRC}
    ${SRCS}
    ${RCC_SOURCES}
    ${CMAKE_PROJECT_NAME}.qrc
    res.qrc
    ${QM}
    )

# 由于 LIB 程序为本项目的功能实现部分，需要用到的第三方库都需要在此处添加。
target_include_directories(${LIB_NAME} PUBLIC ${3rd_lib_INCLUDE_DIRS}  )
target_link_libraries(${LIB_NAME}
    ${DtkDeclarative_LIBRARIES}
    Qt5::Quick
    Qt5::PrintSupport
    Qt5::Gui Qt5::Qml Qt5::Core Qt5::DBus Qt5::Concurrent Qt5::Svg Qt5::Sql GL pthread freeimage
    ${3rd_lib_LIBRARIES}
    )

# Add EXE
add_executable(${BIN_NAME} main.cpp)

# 由于 EXE 只是作为启动程序加载 LIB 使用，一般来说，只需链接 ${LIB_NAME} 既可。
target_link_libraries(${BIN_NAME}
    Qt5::Quick
    ${LIB_NAME}
    )

set_target_properties(${BIN_NAME} PROPERTIES INSTALL_RPATH ${DTK_QML_APP_PLUGIN_PATH})

set(PREFIX ${CMAKE_INSTALL_PREFIX})
set(AppIconDir ${PREFIX}/share/icons/hicolor/scalable/apps)
set(APPSHAREDIR  ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME})
# Add install rule
install(TARGETS ${BIN_NAME} DESTINATION ${APP_BIN_INSTALL_DIR})
install(TARGETS ${LIB_NAME} DESTINATION ${DTK_QML_APP_PLUGIN_PATH})
#desktop
install(FILES ${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}.desktop DESTINATION share/applications)
#manual
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/assets/deepin-album DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-manual/manual-assets/application/)
#icon
install(FILES ${PROJECT_SOURCE_DIR}/src/icon/deepin-album.svg DESTINATION ${AppIconDir})
#translations
install(DIRECTORY ${PROJECT_SOURCE_DIR}/translations
    DESTINATION ${APPSHAREDIR}
    FILES_MATCHING PATTERN "*.qm")
