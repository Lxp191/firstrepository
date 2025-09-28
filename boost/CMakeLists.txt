cmake_minimum_required(VERSION 4.0.0)
project(asio)

include_directories("G:/ProgramFile/Boots/include/boost-1_87")
include_directories("G:/ProgramFile/Visual Leak Detector/include")

list(APPEND CMAKE_PREFIX_PATH "G:/ProgramFile/Boots")
message(STATUS "CMAKE_PREFIX_PATH " ${CMAKE_PREFIX_PATH})
find_package(Boost CONFIG REQUIRED COMPONENTS system)

message(STATUS "Boost_FOUND " ${Boost_FOUND})
message(STATUS "Boost_INCLUDE_DIRS - " ${Boost_INCLUDE_DIRS})
message(STATUS "Boost_LIBRARIES - " ${Boost_LIBRARIES})

add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
add_executable(main main.cpp)
# 检查架构匹配
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64位系统
    message(STATUS "64-bit system detected")
    set(VLD_LIB_DIR "G:/ProgramFile/Visual Leak Detector/lib/Win64")
else()
    # 32位系统  
    message(STATUS "32-bit system detected")
    set(VLD_LIB_DIR "G:/ProgramFile/Visual Leak Detector/lib/Win32")
endif()
target_link_libraries(main Boost::system "${VLD_LIB_DIR}/vld.lib")


add_executable(client client.cpp)
target_link_libraries(client Boost::system "${VLD_LIB_DIR}/vld.lib")

add_executable(test test.cpp)
link_directories(${VLD_LIB_DIR})
target_link_libraries(test ${VLD_LIB_DIR}/vld.lib)