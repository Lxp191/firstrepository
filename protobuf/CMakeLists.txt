cmake_minimum_required(VERSION 3.10)

project(prototest)
 
file(GLOB c_files *.c *.cpp *.cc)

set(ABSEIL_ROOT "C:/Program Files (x86)/protobuf")
set(gRPC_DIR "C:/Program Files (x86)/grpc")
list(APPEND CMAKE_PREFIX_PATH ${ABSEIL_ROOT})
list(APPEND CMAKE_PREFIX_PATH ${gRPC_DIR})

include_directories(E:/CProgram/protobuf-32.0/_deps/absl-src)
include_directories(E:/CProgram/protobuf-32.0/src)
 
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(utf8_range CONFIG REQUIRED)
# find_package(utf8_validity  ${utf8_range_DIR}/utf8_validity.a)

# set(UTF8RANGE_LIBRARY utf8_range::utf8_validity utf8_range::utf8_range)

find_package(absl CONFIG REQUIRED)
set(ABSEL_LIBARARY
    absl::absl_check
    absl::absl_log
    absl::algorithm
    absl::base
    absl::bind_front
    absl::bits
    absl::btree
    absl::cleanup
    absl::cord
    absl::core_headers
    absl::debugging
    absl::die_if_null
    absl::dynamic_annotations
    absl::flags
    absl::flat_hash_map
    absl::flat_hash_set
    absl::function_ref
    absl::hash
    absl::layout
    absl::log_initialize
    absl::log_severity
    absl::memory
    absl::node_hash_map
    absl::node_hash_set
    absl::optional
    absl::span
    absl::status
    absl::statusor
    absl::strings
    absl::synchronization
    absl::time
    absl::type_traits
    absl::utility
    absl::variant
  )
set(grpc++ "C:/Program Files (x86)/grpc/lib/libgrpc++_unsecure.a")

add_executable(prototest main.cpp test.grpc.pb.cc test.pb.cc)
target_link_libraries(prototest ${grpc++} gRPC::grpc protobuf::libprotobuf ${ABSEL_LIBARARY} utf8_range::utf8_range utf8_range::utf8_validity)

add_executable(client client.cpp test.grpc.pb.cc test.pb.cc)
target_link_libraries(client ${grpc++} gRPC::grpc protobuf::libprotobuf ${ABSEL_LIBARARY} utf8_range::utf8_range utf8_range::utf8_validity)

