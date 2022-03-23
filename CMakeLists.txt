cmake_minimum_required(VERSION 3.0)

project(edaPark)

set (CMAKE_CXX_STANDARD 11)

# From "Working with CMake" documentation:
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # AddressSanitizer (ASan)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # UndefinedBehaviorSanitizer (UBSan)
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()


add_executable(${PROJECT_NAME} main.cpp MQTTClient.cpp controllerEDAbot.cpp)

# Packages
find_package(raylib CONFIG REQUIRED)
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v4.1.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()
find_package(OpenSSL REQUIRED)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_package(mosquitto REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${raylib_INCLUDE_DIRS} ${MOSQUITTO_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${raylib_LIBRARIES} ${MOSQUITTO_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto raylib_cpp)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # From "Working with CMake" documentation:
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(${PROJECT_NAME} PRIVATE m ${CMAKE_DL_LIBS} pthread GL rt X11 raylib raylib_cpp mosquitto)
endif()
