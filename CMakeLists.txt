cmake_minimum_required(VERSION 3.5)

project(tfc_alloys)

#### Check --------------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5UiTools REQUIRED)

# include(cotire)

#### Configure ----------------------------

set(EXEC "tfc_alloys")
set(SOURCE_DIR "src")

set(SOURCES
  ${SOURCE_DIR}/Alloys.cpp
  ${SOURCE_DIR}/Calculator.cpp
  ${SOURCE_DIR}/MainWindow.cpp
  ${SOURCE_DIR}/Materials.cpp
  ${SOURCE_DIR}/main.cpp
)

include_directories(${SOURCE_DIR})

set(RESOURCES "resources.qrc")

add_executable(${EXEC} ${SOURCES} ${RESOURCES})
set_property(TARGET ${EXEC} PROPERTY CXX_STANDARD 14)
target_link_libraries(${EXEC} Qt5::Widgets Qt5::UiTools)

target_compile_options(${EXEC} PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address>
    $<$<CONFIG:Debug>:-fsanitize=leak>
    $<$<CONFIG:Debug>:-fsanitize=undefined>
    $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
)
target_link_libraries(${EXEC}
    $<$<CONFIG:Debug>:-fsanitize=address>
    $<$<CONFIG:Debug>:-fsanitize=undefined>
)

# export CCACHE_SLOPPINESS="pch_defines,time_macros"
# set_target_properties(${EXEC} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "${SOURCE_DIR}/external.hpp")
# cotire(${EXEC})
