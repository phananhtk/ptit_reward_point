# WalletSystem: Hệ Thống Quản Lý Điểm Thưởng

## Tổng Quan
WalletSystem là một ứng dụng dòng lệnh để quản lý tài khoản người dùng và ví điểm thưởng. Hệ thống hỗ trợ đăng ký, đăng nhập, quản lý mật khẩu, chuyển điểm và ghi lại lịch sử giao dịch. Mật khẩu được bảo mật bằng hàm băm, và các thao tác nhạy cảm được xác thực bằng OTP. Hệ thống phân biệt giữa người dùng thường và quản trị viên, với quản trị viên có thêm quyền như tạo tài khoản và xem toàn bộ lịch sử giao dịch.

## Tính Năng
- **Quản Lý Người Dùng**: Đăng ký, đăng nhập và quản lý tài khoản (người dùng thường và quản trị viên).
- **Bảo Mật Mật Khẩu**: Hỗ trợ tạo mật khẩu tự động và yêu cầu đổi mật khẩu cho tài khoản mới.
- **Xác Thực OTP**: Gửi OTP qua email để xác minh khi đổi thông tin cá nhân hoặc chuyển điểm.
- **Chuyển Điểm Nguyên Tử**: Đảm bảo giao dịch chuyển điểm an toàn, với cơ chế khôi phục nếu lỗi.
- **Lịch Sử Giao Dịch**: Ghi lại và xem lịch sử giao dịch (riêng tư cho người dùng, toàn bộ cho quản trị viên).
- **Phân Quyền**: Quản trị viên có thể tạo tài khoản, xem danh sách người dùng và lịch sử giao dịch.

## Đóng Góp Nhóm
1. **Phan Anh**:
   - Chuẩn bị dự án, viết hàm `main`.
   - Xử lý đăng nhập (`loginUser`), tạo tài khoản quản trị mặc định.
   - Gửi OTP qua email với thư viện cURL (`sendEmailOTP`, `verifyOTP`).
   - Cài đặt thư viện liên quan.
2. **Tiến Dũng**:
   - Đăng ký người dùng (`registerUser`).
   - Quản trị viên tạo tài khoản mới (`adminCreateUser`).
   - Tải dữ liệu từ tệp `users.txt` (`loadUsersFromFile`), tìm người dùng (`findUserIndex`).
   - Liệt kê danh sách người dùng (`listAllUsers`).
3. **Nguyễn Tùng**:
   - Xem thông tin tài khoản cá nhân (`viewPersonalInfo`).
   - Đổi mật khẩu (`changePassword`).
   - Cập nhật thông tin cá nhân (`updatePersonalInfo`).
4. **Dương Lê**:
   - Chuyển điểm (`transferPoints`).
   - Xem toàn bộ lịch sử giao dịch (`viewTransactionLog`).
   - Xem lịch sử giao dịch cá nhân (`viewMyTransactions`).

------------------

# Hướng Dẫn Cài Đặt và Cấu Hình Dự Án WalletSystem

Hướng dẫn này cung cấp các bước chi tiết để cài đặt môi trường, thư viện OpenSSL và cURL, cũng như build và chạy chương trình trên Windows (với VSCode) và macOS (với Xcode).

## Điều Kiện Tiền Tố
- **Windows**: Windows 10 hoặc cao hơn, Visual Studio Code, MSYS2, MinGW-w64.
- **macOS**: macOS Catalina 10.15 hoặc cao hơn, Xcode, Homebrew.
- **Thư viện**: OpenSSL, cURL.

## Cài Đặt trên Windows (VSCode)

### 1. Cài Đặt MSYS2 và MinGW-w64
1. Tải và cài đặt MSYS2 từ [msys2.org](https://www.msys2.org/).
2. Mở terminal MSYS2, cập nhật gói và cài đặt công cụ:
   ```
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb
   ```
3. Kiểm tra phiên bản:
   ```
   g++ --version
   ```

### 2. Cài Đặt Thư Viện OpenSSL và cURL
1. Cài đặt OpenSSL và cURL qua MSYS2:
   ```
   pacman -S mingw-w64-x86_64-openssl mingw-w64-x86_64-curl
   ```

### 3. Cấu Hình Makefile
1. Đảm bảo tệp `Makefile` trong thư mục dự án có nội dung:
   ```
   # Makefile cho WalletSystem
   OPENSSL_PREFIX = /mingw64
   CXX = g++
   CXXFLAGS = -std=c++17 -Wall -Wextra
   INCLUDES = -I$(OPENSSL_PREFIX)/include
   LIBS = -L$(OPENSSL_PREFIX)/lib -lssl -lcrypto -lcurl
   TARGET = wallet_system
   SRC = main.cpp

   all: $(TARGET)

   $(TARGET): $(SRC)
       $(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SRC) $(LIBS)

   clean:
       rm -f $(TARGET)
   ```

### 4. Build và Chạy
1. Mở terminal trong thư mục dự án, chạy:
   ```
   make
   ```
2. Chạy chương trình:
   ```
   ./wallet_system.exe
   ```
   - Đảm bảo tệp `users.txt` nằm trong cùng thư mục.
   - Nếu thiếu DLL (`libcrypto-3-x64.dll`, `libssl-3-x64.dll`, `libcurl-x64.dll`), sao chép từ `/mingw64/bin` vào thư mục chứa `wallet_system.exe`.

## Cài Đặt trên macOS (Xcode)

### 1. Cài Đặt Homebrew và Công Cụ
1. Cài đặt Homebrew (nếu chưa có):
   ```
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```
2. Cập nhật Homebrew và cài đặt g++:
   ```
   brew update
   brew install gcc
   ```
3. Kiểm tra phiên bản:
   ```
   g++ --version
   ```

### 2. Cài Đặt Thư Viện OpenSSL và cURL
1. Cài đặt OpenSSL và cURL qua Homebrew:
   ```
   brew install openssl@3
   brew install curl
   ```
2. Kiểm tra đường dẫn OpenSSL (thường là `/opt/homebrew/opt/openssl@3` trên Apple Silicon hoặc `/usr/local/opt/openssl@3` trên Intel):
   ```
   brew --prefix openssl@3
   brew --prefix curl
   ```

### 3. Cấu Hình Makefile
1. Đảm bảo tệp `Makefile` trong thư mục dự án có nội dung:
   ```
   # Makefile cho WalletSystem
   OPENSSL_PREFIX = /opt/homebrew/opt/openssl@3
   CXX = g++
   CXXFLAGS = -std=c++17 -Wall -Wextra
   INCLUDES = -I$(OPENSSL_PREFIX)/include
   LIBS = -L$(OPENSSL_PREFIX)/lib -lssl -lcrypto -lcurl
   TARGET = wallet_system
   SRC = main.cpp

   all: $(TARGET)

   $(TARGET): $(SRC)
       $(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SRC) $(LIBS)

   clean:
       rm -f $(TARGET)
   ```
   - Nếu sử dụng Intel-based Mac, thay `OPENSSL_PREFIX = /usr/local/opt/openssl@3`.

### 4. Cấu Hình Xcode
1. Mở Xcode, tạo dự án mới:
   - Chọn **File > New > Project > macOS > Command Line Tool**, đặt tên là `WalletSystem`.
   - Thêm tệp `main.cpp` và `Makefile` vào dự án.
2. Cấu hình build:
   - Trong **Build Settings**, thêm:
     - **Header Search Paths**: `/opt/homebrew/opt/openssl@3/include` (hoặc `/usr/local/opt/openssl@3/include` cho Intel).
     - **Library Search Paths**: `/opt/homebrew/opt/openssl@3/lib` (hoặc `/usr/local/opt/openssl@3/lib` cho Intel).
     - **Other Linker Flags**: `-lssl -lcrypto -lcurl`.
3. Tạo **Build Phase** để chạy `make`:
   - Trong **Build Phases**, thêm **New Run Script Phase**:
   ```
   cd $SRCROOT
   make
   ```

### 5. Build và Chạy
1. Trong Xcode, nhấn **Product > Build** (`Cmd+B`) để build.
2. Chạy chương trình:
   - Nhấn **Product > Run** (`Cmd+R`), hoặc chạy từ terminal:
   ```
   ./wallet_system
   ```
   - Đảm bảo tệp `users.txt` nằm trong cùng thư mục với `wallet_system`.

## Xử Lý Lỗi
- **Windows**:
  - **Thiếu DLL**: Sao chép từ `/mingw64/bin` vào thư mục chứa `wallet_system.exe`.
  - **Lỗi Build**: Kiểm tra đường dẫn OpenSSL trong `Makefile` hoặc cài lại thư viện:
    ```
    pacman -S mingw-w64-x86_64-openssl mingw-w64-x86_64-curl
    ```
  - **Lỗi Chạy**: Đảm bảo định dạng `users.txt` đúng: `username;role;passwordHash;balance;fullname;email;needChangePassword`.
- **macOS**:
  - **Lỗi Thư Viện**: Kiểm tra đường dẫn OpenSSL trong `Makefile`. Chạy `brew --prefix openssl@3` để xác minh.
  - **Lỗi Build**: Đảm bảo g++, OpenSSL và cURL được cài đặt:
    ```
    brew reinstall openssl@3 curl
    ```
  - **Lỗi Chạy**: Đảm bảo định dạng `users.txt` đúng và tệp nằm trong thư mục chứa `wallet_system`.

## Ghi Chú
- Tài khoản quản trị mặc định: `username: admin`, `password: admin` (yêu cầu đổi mật khẩu khi đăng nhập lần đầu).
- Cập nhật Homebrew (macOS): `brew update`.
- Cập nhật MSYS2 (Windows): `pacman -Syu`.
- Ngày cập nhật: 11:40, Thứ Ba, 20/05/2025.