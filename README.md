WalletSystem: Hệ Thống Quản Lý Điểm Thưởng
Tổng Quan
WalletSystem là một ứng dụng dòng lệnh để quản lý tài khoản người dùng và ví điểm thưởng. Hệ thống hỗ trợ đăng ký, đăng nhập, quản lý mật khẩu, chuyển điểm và ghi lại lịch sử giao dịch. Mật khẩu được bảo mật bằng hàm băm, và các thao tác nhạy cảm được xác thực bằng OTP. Hệ thống phân biệt giữa người dùng thường và quản trị viên, với quản trị viên có thêm quyền như tạo tài khoản và xem toàn bộ lịch sử giao dịch.
Tính Năng

Quản Lý Người Dùng: Đăng ký, đăng nhập và quản lý tài khoản (người dùng thường và quản trị viên).
Bảo Mật Mật Khẩu: Hỗ trợ tạo mật khẩu tự động và yêu cầu đổi mật khẩu cho tài khoản mới.
Xác Thực OTP: Gửi OTP qua email để xác minh khi đổi thông tin cá nhân hoặc chuyển điểm.
Chuyển Điểm Nguyên Tử: Đảm bảo giao dịch chuyển điểm an toàn, với cơ chế khôi phục nếu lỗi.
Lịch Sử Giao Dịch: Ghi lại và xem lịch sử giao dịch (riêng tư cho người dùng, toàn bộ cho quản trị viên).
Phân Quyền: Quản trị viên có thể tạo tài khoản, xem danh sách người dùng và lịch sử giao dịch.

Đóng Góp Nhóm

Phan Anh: 
Chuẩn bị dự án, viết hàm main.
Xử lý đăng nhập (loginUser), tạo tài khoản quản trị mặc định.
Gửi OTP qua email với thư viện cURL (sendEmailOTP, verifyOTP).
Cài đặt thư viện liên quan.


Tiến Dũng:
Đăng ký người dùng (registerUser).
Quản trị viên tạo tài khoản mới (adminCreateUser).
Tải dữ liệu từ tệp users.txt (loadUsersFromFile), tìm người dùng (findUserIndex).
Liệt kê danh sách người dùng (listAllUsers).


Nguyễn Tùng:
Xem thông tin tài khoản cá nhân (viewPersonalInfo).
Đổi mật khẩu (changePassword).
Cập nhật thông tin cá nhân (updatePersonalInfo).


Dương Lê:
Chuyển điểm (transferPoints).
Xem toàn bộ lịch sử giao dịch (viewTransactionLog).
Xem lịch sử giao dịch cá nhân (viewMyTransactions).



Yêu Cầu Hệ Thống

Windows:
Hệ điều hành: Windows 10 hoặc cao hơn.
Công cụ: Visual Studio Code (VSCode), MSYS2, MinGW-w64.
Thư viện: OpenSSL, cURL.


macOS:
Hệ điều hành: macOS Catalina 10.15 hoặc cao hơn.
Công cụ: Xcode, Homebrew, g++.
Thư viện: OpenSSL, cURL.



Hướng Dẫn Cài Đặt
1. Cài Đặt trên Windows (sử dụng VSCode)
Cài Đặt MSYS2 và MinGW-w64

Tải MSYS2 từ msys2.org và cài đặt.
Mở terminal MSYS2, cập nhật gói và cài đặt công cụ:pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb


Kiểm tra phiên bản:g++ --version



Cài Đặt OpenSSL và cURL

Cài đặt OpenSSL và cURL qua MSYS2:pacman -S mingw-w64-x86_64-openssl mingw-w64-x86_64-curl



Cấu Hình Makefile

Đảm bảo tệp Makefile trong thư mục dự án có nội dung:# Makefile cho WalletSystem
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



Build và Chạy

Mở terminal trong thư mục dự án, chạy:make


Chạy chương trình:./wallet_system.exe


Đảm bảo tệp users.txt nằm trong cùng thư mục.
Nếu thiếu DLL (libcrypto-3-x64.dll, libssl-3-x64.dll, libcurl-x64.dll), sao chép từ /mingw64/bin vào thư mục chứa wallet_system.exe.



2. Cài Đặt trên macOS (sử dụng Xcode)
Cài Đặt Homebrew và Công Cụ

Cài đặt Homebrew (nếu chưa có):/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"


Cập nhật Homebrew và cài đặt g++:brew update
brew install gcc


Kiểm tra phiên bản g++:g++ --version



Cài Đặt OpenSSL và cURL

Cài đặt OpenSSL và cURL qua Homebrew:brew install openssl@3
brew install curl


Kiểm tra đường dẫn OpenSSL (thường là /opt/homebrew/opt/openssl@3 trên Apple Silicon hoặc /usr/local/opt/openssl@3 trên Intel-based Macs):brew --prefix openssl@3
brew --prefix curl



Cấu Hình Makefile

Đảm bảo tệp Makefile trong thư mục dự án có nội dung:# Makefile cho WalletSystem
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


Nếu sử dụng Intel-based Mac, thay OPENSSL_PREFIX = /usr/local/opt/openssl@3.



Cấu Hình Xcode

Mở Xcode, tạo dự án mới (hoặc mở thư mục dự án):
Chọn File > New > Project > macOS > Command Line Tool, đặt tên là WalletSystem.
Thêm tệp main.cpp và Makefile vào dự án.


Cấu hình build:
Vào Xcode > Preferences > Locations, đảm bảo Command Line Tools được chọn (g++).
Trong Build Settings, thêm các đường dẫn thư viện:
Header Search Paths: /opt/homebrew/opt/openssl@3/include (hoặc /usr/local/opt/openssl@3/include cho Intel).
Library Search Paths: /opt/homebrew/opt/openssl@3/lib (hoặc /usr/local/opt/openssl@3/lib cho Intel).
Other Linker Flags: -lssl -lcrypto -lcurl.




Tạo Build Phase để chạy make:
Trong Build Phases, thêm New Run Script Phase:cd $SRCROOT
make





Build và Chạy

Trong Xcode, nhấn Product > Build (hoặc Cmd+B) để build.
Chạy chương trình:
Trong Xcode, nhấn Product > Run (hoặc Cmd+R), hoặc chạy từ terminal:./wallet_system


Đảm bảo tệp users.txt nằm trong cùng thư mục với wallet_system.



Xử Lý Lỗi

Windows:
Thiếu DLL: Sao chép từ /mingw64/bin vào thư mục chứa wallet_system.exe.
Lỗi Build: Kiểm tra đường dẫn OpenSSL trong Makefile hoặc cài lại thư viện:pacman -S mingw-w64-x86_64-openssl mingw-w64-x86_64-curl


Lỗi Chạy: Đảm bảo định dạng users.txt đúng: username;role;passwordHash;balance;fullname;email;needChangePassword.


macOS:
Lỗi Thư Viện: Kiểm tra đường dẫn OpenSSL trong Makefile. Chạy brew --prefix openssl@3 để xác minh.
Lỗi Build: Đảm bảo g++, OpenSSL và cURL được cài đặt:brew reinstall openssl@3 curl


Lỗi Chạy: Đảm bảo định dạng users.txt đúng và tệp nằm trong thư mục chứa wallet_system.



Ghi Chú

Tài khoản quản trị mặc định: username: admin, password: admin (yêu cầu đổi mật khẩu khi đăng nhập lần đầu).
Cập nhật Homebrew (macOS): brew update.
Cập nhật MSYS2 (Windows): pacman -Syu.
Ngày cập nhật: Thứ Ba, 20/05/2025.

