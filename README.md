# WalletSystem: Hệ Thống Quản Lý Điểm Thưởng

## Tổng Quan
WalletSystem là một ứng dụng dòng lệnh để quản lý tài khoản người dùng và ví điểm thưởng. Hệ thống hỗ trợ đăng ký, đăng nhập, quản lý mật khẩu, chuyển điểm và ghi lại lịch sử giao dịch. Mật khẩu được bảo mật bằng hàm băm, và các thao tác nhạy cảm được xác thực bằng OTP. Hệ thống phân biệt giữa người dùng thường và quản trị viên, với quản trị viên có thêm quyền như tạo tài khoản và xem toàn bộ lịch sử giao dịch.

## Tính Năng
- **Quản Lý Người Dùng**: Đăng ký, đăng nhập và quản lý tài khoản (người dùng thường và quản trị viên).
- **Bảo Mật Mật Khẩu**: Hỗ trợ tạo mật khẩu tự động và yêu cầu đổi mật khẩu cho tài khoản mới.
- **Xác Thực OTP**: Gửi OTP qua email để xác minh khi đổi thông tin cá nhân hoặc chuyển điểm.
- **Chuyển Điểm**: Đảm bảo giao dịch chuyển điểm an toàn, với cơ chế khôi phục nếu lỗi.
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
2. Mở terminal MSYS2, cập nhật gói:
   ```
   pacman -Syu
   ```
   - Nếu được yêu cầu đóng terminal, đóng và mở lại, sau đó chạy lại lệnh trên.
3. Cài đặt MinGW-w64 và các công cụ:
   ```
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb
   ```
4. Thêm MinGW-w64 vào biến PATH của Windows:
   - Nhấn **Win + R**, gõ `sysdm.cpl` để mở System Properties.
   - Chọn **Advanced > Environment Variables**.
   - Trong **System Variables**, tìm **Path**, chỉnh sửa và thêm: `C:\msys64\mingw64\bin`.
   - Mở terminal mới, kiểm tra phiên bản:
     ```
     g++ --version
     ```

### 2. Cài Đặt vcpkg và Thư Viện OpenSSL, cURL
1. Mở Command Prompt, clone vcpkg:
   ```
   git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
   cd C:\vcpkg
   ```
2. Khởi tạo vcpkg:
   ```
   bootstrap-vcpkg.bat
   ```
3. Cài đặt OpenSSL và cURL:
   ```
   vcpkg install openssl:x64-windows
   vcpkg install curl:x64-windows
   ```
4. Tích hợp vcpkg với hệ thống:
   ```
   vcpkg integrate project
   ```
5. Kiểm tra thư viện:
   - OpenSSL và cURL sẽ được cài vào `C:\vcpkg\installed\x64-windows`. Đảm bảo thư mục `include\openssl`, `include\curl`, và `lib` tồn tại.

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

### 4. Build
1. Mở terminal trong thư mục dự án, chạy:
   ```
   make
   ```

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
### 3. Cài Đặt  Thư Viện OpenSSL, cURL với vcpkg
1. Mở terminal, clone vcpkg:
   ```
   git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
   cd ~/vcpkg
   ```
2. Khởi tạo vcpkg:
   ```
   ./bootstrap-vcpkg.sh
   ```
3. Cài đặt OpenSSL và cURL:
   ```
   ./vcpkg install openssl:x64-osx
   ./vcpkg install curl:x64-osx
   ```
4. Tích hợp vcpkg với hệ thống:
   ```
   ./vcpkg integrate project
   ```
5. Kiểm tra thư viện:
   - OpenSSL và cURL sẽ được cài vào `~/vcpkg/installed/x64-osx`. Đảm bảo thư mục `include\openssl`, `include\curl`, và `lib` tồn tại.
   
### 4. Cấu Hình Makefile
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

### 5. Cấu Hình Xcode
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

### 6. Build
1. Trong Xcode, nhấn **Product > Build** (`Cmd+B`) để build.

## Mô tả Cách Chạy Chương Trình

### 1. Chạy Chương Trình
- **Windows**:
  1. Mở terminal trong thư mục dự án (nơi chứa `wallet_system.exe`).
  2. Chạy:
     ```
     ./wallet_system.exe
     ```
  - Đảm bảo tệp `users.txt` nằm trong cùng thư mục. Nếu chưa có, chương trình sẽ tự tạo tài khoản quản trị mặc định (`username: admin`, `password: admin`).
  - Nếu thiếu DLL (`libcrypto-3-x64.dll`, `libssl-3-x64.dll`, `libcurl-x64.dll`), sao chép từ `/mingw64/bin` vào thư mục chứa `wallet_system.exe`.
- **macOS**:
  1. Trong Xcode, nhấn **Product > Run** (`Cmd+R`) để chạy.
  2. Hoặc mở terminal trong thư mục dự án, chạy:
     ```
     ./wallet_system
     ```
  - Đảm bảo tệp `users.txt` nằm trong cùng thư mục.

### 2. Các Thao Tác Chính
Sau khi chạy chương trình, bạn sẽ thấy menu chính:
```
=== MENU ===
1. Đăng nhập
2. Đăng ký
0. Thoát
Lựa chọn: 
```

#### a. Đăng Nhập
- Chọn `1`, nhập tên đăng nhập và mật khẩu.
- Nếu sử dụng tài khoản quản trị mặc định (`admin`/`admin`) hoặc tài khoản có mật khẩu tự động, hệ thống sẽ yêu cầu đổi mật khẩu:
  ```
  Bạn đang sử dụng mật khẩu tạm thời. Vui lòng đổi mật khẩu mới.
  Mật khẩu mới: [nhập mật khẩu mới]
  Xác nhận mật khẩu mới: [nhập lại]
  ```
- Sau khi đăng nhập:
  - **Người dùng thường**:
    ```
    --- MENU NGƯỜI DÙNG ---
    1. Xem thông tin cá nhân
    2. Đổi mật khẩu
    3. Cập nhật thông tin cá nhân
    4. Chuyển điểm
    5. Xem lịch sử giao dịch của tôi
    0. Đăng xuất
    Lựa chọn: 
    ```
  - **Quản trị viên**:
    ```
    --- MENU QUẢN TRỊ VIÊN ---
    1. Xem thông tin cá nhân
    2. Đổi mật khẩu
    3. Cập nhật thông tin cá nhân
    4. Chuyển điểm cho người dùng
    5. Xem danh sách người dùng
    6. Tạo tài khoản mới
    7. Xem lịch sử giao dịch
    0. Đăng xuất
    Lựa chọn: 
    ```

#### b. Đăng Ký
- Chọn `2` từ menu chính.
- Nhập thông tin:
  ```
  Tên đăng nhập mới: [nhập tên]
  Tên người dùng: [nhập họ tên]
  Email: [nhập email]
  Mật khẩu (nhấn Enter để sử dụng mật khẩu tự động): [nhập hoặc bỏ trống]
  ```
- Nếu bỏ trống mật khẩu, hệ thống sẽ tạo mật khẩu ngẫu nhiên (8 ký tự) và yêu cầu đổi khi đăng nhập lần đầu.

#### c. Chuyển Điểm
- Từ menu người dùng hoặc quản trị viên, chọn `4`.
- Nhập thông tin:
  ```
  Nhập tên tài khoản người nhận: [nhập tên]
  Nhập số điểm cần chuyển: [nhập số]
  ```
- Hệ thống sẽ gửi OTP đến email của bạn (in ra màn hình trong chế độ thử nghiệm). Nhập OTP để xác nhận:
  ```
  Mã OTP đã được gửi đến email của bạn ([email])
  Nhập mã OTP để xác nhận: [nhập mã]
  ```

#### d. Các Thao Tác Khác
- **Xem thông tin cá nhân** (lựa chọn `1`): Hiển thị tên, email, số dư, loại tài khoản.
- **Đổi mật khẩu** (lựa chọn `2`): Nhập mật khẩu hiện tại, mật khẩu mới và xác nhận.
- **Cập nhật thông tin cá nhân** (lựa chọn `3`): Cập nhật họ tên, email, yêu cầu OTP xác nhận.
- **Xem lịch sử giao dịch** (lựa chọn `5` cho người dùng, `7` cho quản trị viên): Hiển thị các giao dịch liên quan đến bạn hoặc toàn bộ hệ thống (quản trị viên).
- **Tạo tài khoản mới** (lựa chọn `6`, chỉ quản trị viên): Tạo tài khoản với vai trò người dùng hoặc quản trị viên.

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
- Ngày cập nhật: 11:49, Thứ Ba, 20/05/2025.