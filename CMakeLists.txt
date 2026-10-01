cmake_minimum_required(VERSION 3.24)
project(CleanFlow VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Test)
qt_standard_project_setup()

qt_add_executable(CleanFlow
    app/main.cpp
    src/core/PhotoshopSlotRegistry.cpp
    src/platform/windows/PhotoshopWindowManager.cpp
)

qt_add_qml_module(CleanFlow
    URI CleanFlow
    VERSION 1.0
    QML_FILES app/qml/Main.qml
)

target_include_directories(CleanFlow PRIVATE src)
target_link_libraries(CleanFlow PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick)

if (WIN32)
    target_compile_definitions(CleanFlow PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

include(CTest)
if (BUILD_TESTING)
    qt_add_executable(CleanFlowTests
        tests/PhotoshopSlotRegistryTests.cpp
        src/core/PhotoshopSlotRegistry.cpp
    )
    target_include_directories(CleanFlowTests PRIVATE src)
    target_link_libraries(CleanFlowTests PRIVATE Qt6::Core Qt6::Test)
    add_test(NAME CleanFlowTests COMMAND CleanFlowTests)
endif()
