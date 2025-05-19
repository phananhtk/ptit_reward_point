cmake_minimum_required(VERSION 3.10)
project(WalletSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tích hợp vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Tìm và liên kết OpenSSL
find_package(OpenSSL 3.0 REQUIRED)
if (OPENSSL_FOUND)
    message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
    message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenSSL not found")
endif()

# Tìm và liên kết cURL
find_package(CURL REQUIRED)
if (CURL_FOUND)
    message(STATUS "cURL found: ${CURL_VERSION_STRING}")
    message(STATUS "cURL include dir: ${CURL_INCLUDE_DIRS}")
    message(STATUS "cURL libraries: ${CURL_LIBRARIES}")
else()
    message(FATAL_ERROR "cURL not found")
endif()

# Tạo executable
add_executable(wallet_system main.cpp)

# Thêm include directories
target_include_directories(wallet_system PRIVATE ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})

# Liên kết thư viện
target_link_libraries(wallet_system PRIVATE ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})

# Thêm các thư viện bổ trợ cho Windows
if(WIN32)
    target_link_libraries(wallet_system PRIVATE ws2_32 crypt32 wldap32)
endif()

# Sao chép DLL cho Windows
if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    file(GLOB OPENSSL_DLLS "${CMAKE_PREFIX_PATH}/bin/libcrypto*.dll" "${CMAKE_PREFIX_PATH}/bin/libssl*.dll")
    file(GLOB CURL_DLLS "${CMAKE_PREFIX_PATH}/bin/libcurl*.dll")
    foreach(DLL ${OPENSSL_DLLS} ${CURL_DLLS})
        add_custom_command(TARGET wallet_system POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${DLL}"
                    $<TARGET_FILE_DIR:wallet_system>)
    endforeach()
endif()