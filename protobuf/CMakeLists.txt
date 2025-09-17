cmake_minimum_required(VERSION 3.10)

project(prototest)
 
file(GLOB c_files *.c *.cpp *.cc)

set(ABSEIL_ROOT "C:/Program Files (x86)/protobuf")
set(utf8_range_DIR "E:/CProgram/protobuf-32.0/build/third_party/utf8_range")
list(APPEND CMAKE_PREFIX_PATH ${ABSEIL_ROOT})
list(APPEND CMAKE_PREFIX_PATH ${utf8_range_DIR})

include_directories(E:/CProgram/protobuf-32.0/_deps/absl-src)
include_directories(E:/CProgram/protobuf-32.0/src)
 

# find_package(utf8_range  ${utf8_range_DIR}/libutf8_range.a)
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

add_executable(prototest ${c_files})

set(protobufLib E:/firstrepository/protobuf/libprotobuf.a)
set(utf8_range ${utf8_range_DIR}/libutf8_range.a)
set(utf8_validity ${utf8_range_DIR}/libutf8_validity.a)

target_link_libraries(prototest ${protobufLib} ${ABSEL_LIBARARY} pthread ${utf8_range} ${utf8_validity})

