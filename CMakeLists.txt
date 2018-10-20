cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

add_executable(minimal_example main.cpp
  person.h)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

set(CMAKE_BOOST_COMPONENTS "system" "thread")
find_package(Boost REQUIRED COMPONENTS ${CMAKE_BOOST_COMPONENTS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(Threads REQUIRED)

find_package(CURL REQUIRED)

include(FindVelocypack)
find_package(Velocypack)

if (NOT ${VELOCYPACK_FOUND})
  message(FATAL_ERROR "Velocypack not found. Clone from https://github.com/arangodb/velocypack and install it")
endif()

include(ECMEnableSanitizers)

target_link_libraries(minimal_example fuerte
  ${VELOCYPACK_LIBRARIES}
  ${CURL_LIBRARIES}
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)
