# ptit_reward_point
Reward Point Management
Thành Viên dự án 
1. Phan Anh 
  - chuẩn bị dự án 
  - viết hàm `main` 
  - Hàm xử lý đăng nhập người dùng `loginUser`
  - chức năng tạo tài khoản tự động (role admin) khi chạy chương trình lần đầu
  - xử lí chức năng gửi OTP về email với thư viện "CURL" `sendEmailOTP`  ` verifyOTP` 
  - cài đặt thư viện liên quan
2. Tiến Dũng
  - Đăng kí user `registerUser`
  - Hàm cho quản trị viên tạo tài khoản mới `adminCreateUser`
  - Hàm tải dữ liệu người dùng từ tệp `users.txt` `loadUsersFromFile()`
  - Hàm tra cứu người dùng `findUserIndex`
  - Hàm liệt kê tất cả người dùng trong hệ thống `listAllUsers`
3. Nguyễn Tùng
  - Hàm xem thông tin tài khoản cá nhân `viewPersonalInfo`
  - Hàm đổi mật khẩu cho người dùng đã đăng nhập `changePassword`
  - Hàm cập nhật thông tin cá nhân `updatePersonalInfo`
4. Dương Lê
  - Hàm chuyển điểm `transferPoints`
  - Hàm xem toàn bộ lịch sử giao dịch (dành cho quản trị viên) `viewTransactionLog`
  - Hàm xem lịch sử giao dịch của người dùng `viewMyTransactions`
------------------
# Hướng Dẫn Chi Tiết Cài Đặt và Cấu Hình Dự Án WalletSystem

Hướng dẫn này cung cấp các bước chi tiết để cài đặt CMake, tích hợp vcpkg, cài đặt thư viện OpenSSL và cURL, cũng như cấu hình các file trong thư mục `.vscode` và file `CMakeLists.txt` cho dự án WalletSystem trên Windows với MSYS2 và MinGW-w64.

---

## Điều Kiện Tiền Tiết
- **Hệ Điều Hành**: Windows 10 hoặc cao hơn.
- **Công Cụ**:
  - Visual Studio Code (VSCode).
  - Git (tùy chọn, để clone vcpkg).
  - MSYS2 và MinGW-w64.

---

## Bước 1: Cài Đặt MSYS2 và MinGW-w64
1. Tải và cài đặt MSYS2 từ [https://www.msys2.org/](https://www.msys2.org/).
2. Mở terminal MSYS2 và cập nhật gói:
   ```
   pacman -Syu
   ```
3. Cài đặt MinGW-w64 và CMake:
   ```
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-cmake
   ```
4. Kiểm tra phiên bản:
   ```
   g++ --version
   gdb --version
   cmake --version
   ```
   - Ghi chú đường dẫn: `C:\msys64\mingw64\bin`.

---

## Bước 2: Cài Đặt CMake trong VSCode
1. Mở VSCode.
2. Cài đặt extension **CMake Tools**:
   - Nhấn `Ctrl+Shift+X`, tìm "CMake Tools", và cài đặt.
3. Đảm bảo CMake được nhận diện:
   - Nhấn `Ctrl+Shift+P`, chọn **CMake: Select a Kit**.
   - Chọn kit `GCC x.x.x x86_64-w64-mingw32` từ `C:\msys64\mingw64`.

---

## Bước 3: Cài Đặt vcpkg
1. Mở Command Prompt (cmd).
2. Clone vcpkg:
   ```
   git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
   ```
3. Di chuyển đến thư mục vcpkg:
   ```
   cd C:\vcpkg
   ```
4. Khởi tạo vcpkg:
   ```
   bootstrap-vcpkg.bat
   ```
5. Thêm vcpkg vào PATH (tùy chọn):
   - Nhấp chuột phải vào This PC > Properties > Advanced system settings > Environment Variables.
   - Thêm `C:\vcpkg` vào PATH.
   - Khởi động lại cmd.

---

## Bước 4: Cài Đặt Thư Viện OpenSSL và cURL
1. Tích hợp vcpkg với dự án:
   - Mở file `CMakeLists.txt` (tạo mới nếu chưa có) và thêm:
     ```
     set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
     ```

2. Cài đặt thư viện:
   - Mở Command Prompt và chạy:
     ```
     C:\vcpkg\vcpkg install openssl:x64-windows
     C:\vcpkg\vcpkg install curl:x64-windows
     ```
   - Kiểm tra trong `C:\vcpkg\installed\x64-windows` để xác nhận thư mục `include\openssl`, `include\curl`, và `lib\*.lib`.

---

## Bước 5: Cấu Hình File `CMakeLists.txt`
Tạo hoặc chỉnh sửa file `CMakeLists.txt` với nội dung sau:

```
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
```

- Lưu file trong thư mục gốc của dự án.

---

## Bước 6: Cấu Hình File trong Thư Mục `.vscode`
1. Tạo hoặc chỉnh sửa file `.vscode/tasks.json`:
```
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "CMake Configure",
      "type": "shell",
      "command": "cmake",
      "args": [
        "-B",
        "build",
        "-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake",
        "-G",
        "MinGW Makefiles"
      ],
      "group": {
        "kind": "build",
        "isDefault": false
      },
      "problemMatcher": [],
      "detail": "Cấu hình dự án CMake với vcpkg"
    },
    {
      "label": "CMake Build",
      "type": "shell",
      "command": "cmake",
      "args": [
        "--build",
        "build"
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "problemMatcher": [],
      "detail": "Build CMake"
    }
  ]
}
```

2. Tạo hoặc chỉnh sửa file `.vscode/launch.json`:
```
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "C/C++ Wallet System: Debug",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/bin/Debug/wallet_system.exe",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": true,
      "MIMode": "gdb",
      "miDebuggerPath": "C:/msys64/mingw64/bin/gdb.exe",
      "setupCommands": [
        {
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ],
      "preLaunchTask": "CMake Build"
    }
  ]
}
```

3. Tạo hoặc chỉnh sửa file `.vscode/c_cpp_properties.json`:
```
{
  "configurations": [
    {
      "name": "windows-gcc-x64",
      "includePath": [
        "${workspaceFolder}/**",
        "C:/vcpkg/installed/x64-windows/include",
        "C:/vcpkg/installed/x64-windows/include/openssl",
        "C:/vcpkg/installed/x64-windows/include/curl",
        "C:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/13.2.0/include"
      ],
      "compilerPath": "C:/msys64/mingw64/bin/g++.exe",
      "cStandard": "c17",
      "cppStandard": "c++17",
      "intelliSenseMode": "windows-gcc-x64",
      "compilerArgs": [],
      "defines": [
        "_DEBUG",
        "UNICODE",
        "_UNICODE"
      ]
    }
  ],
  "version": 4
}
```

- Đảm bảo đường dẫn trong các file khớp với hệ thống của bạn (ví dụ: `C:/msys64/mingw64`).

---

## Bước 7: Build và Chạy Chương Trình
1. Mở dự án trong VSCode.
2. Nhấn `Ctrl+Shift+P`, chọn **CMake: Configure**.
3. Nhấn `Ctrl+Shift+B` để build.
4. Mở Command Prompt, di chuyển đến `build/bin`:
   ```
   cd path\to\your\project\build\bin
   ```
   - Ví dụ: `cd D:\Projects\WalletSystem\build\bin`.
5. Chạy chương trình:
   ```
   wallet_system.exe
   ```
   - Đảm bảo `users.txt` và các file DLL (`libcrypto-3-x64.dll`, `libssl-3-x64.dll`, `libcurl-x64.dll`) nằm trong cùng thư mục.

---

## Xử Lý Lỗi
- **Thiếu DLL**: Sao chép từ `C:\vcpkg\installed\x64-windows\bin` vào `build\bin`.
- **Lỗi Build**: Kiểm tra output CMake và cài lại thư viện nếu cần.
- **Lỗi Chạy**: Kiểm tra định dạng `users.txt` (ví dụ: `username;role;passwordHash;balance;fullname;email;needChangePassword`).

---

## Ghi Chú Thêm
- Cập nhật vcpkg: `C:\vcpkg\vcpkg update`.
- Ngày tạo: 11:11 PM +07, Thứ Hai, 19/05/2025.