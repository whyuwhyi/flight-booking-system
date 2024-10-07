cmake_minimum_required(VERSION 3.16)

project(flight-booking VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_BUILD_TYPE Debug)

# 查找 Qt6 库
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools WebEngineWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools WebEngineWidgets)

include_directories(include)

# 定义源文件
file(GLOB_RECURSE SOURCE_CODE "src/*.cpp")
file(GLOB_RECURSE INCLUDE_CODE "include/*.h")

set(PROJECT_SOURCES
    ${SOURCE_CODE}
    ${INCLUDE_CODE}
    "resources/resources.qrc"
)

# 使用 Qt6 创建可执行文件
qt_add_executable(flight-booking
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# 链接 Qt6 库
target_link_libraries(flight-booking PRIVATE Qt6::Widgets Qt6::WebEngineWidgets)

# 设置 macOS 和 Windows 属性
set_target_properties(flight-booking PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 安装目标
include(GNUInstallDirs)
install(TARGETS flight-booking
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(flight-booking)
