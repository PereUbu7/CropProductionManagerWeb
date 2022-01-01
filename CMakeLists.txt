cmake_minimum_required(VERSION 3.1...3.21)

project(
    CropProductionManager
    VERSION 0.1
    LANGUAGES CXX
)

option(ENABLE_WARNINGS_SETTINGS "Allow target_set_warnings to add flags and defines.
                            Set this to OFF if you want to provide your own warning parameters." ON)
option(ENABLE_LTO "Enable link time optimization" ON)
option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                            Tests in test/*.cpp will still be enabled." ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(ConfigSafeGuards)
include(Colors)
include(CTest)
include(Doctest)
include(Documentation)
include(LTO)
include(Mics)
include(Warnings)

find_lto(CXX)

set(SOURCES
    src/Infrastructure/cropRepository.cpp
    src/Core/cropCore.cpp
    src/Api/cropApi.cpp
    src/InternalModel/Infrastructure/crop.cpp
    src/InternalModel/Core/crop.cpp
    src/Serializer/crop.cpp
)

set(TESTFILES
    tests/main.cpp
)

set(LIBRARY_NAME CropProductionManager)

# Build:

add_library(${LIBRARY_NAME} OBJECT ${SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/extern/)

target_link_libraries(${LIBRARY_NAME} PUBLIC doctest)

# target_set_warnings(${LIBRARY_NAME} ENABLE ALL AS_ERROR ALL DISABLE Annoying)

add_executable(cpm apps/Program.cpp)
target_link_libraries(cpm PRIVATE ${LIBRARY_NAME})
# target_set_warnings(cpm ENABLE ALL AS_ERROR ALL DISABLE Annoying)
target_enable_lto(cpm optimized)

set_target_properties(
    ${LIBRARY_NAME} cpm
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
)

add_subdirectory(tests)