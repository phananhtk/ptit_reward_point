#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <cctype>

using namespace std;

bool sendEmailOTP()
{
    return true;
}

// Cấu trúc lưu thông tin một người dùng trong hệ thống
struct User
{
    string username;         // Tên đăng nhập
    string passwordHash;     // Mật khẩu đã băm (SHA-256)
    string fullname;         // Họ tên đầy đủ
    string email;            // Email liên hệ
    long long balance;       // Số dư ví điểm thưởng
    bool isAdmin;            // Quyền quản trị (true nếu là admin)
    bool needChangePassword; // Đánh dấu cần đổi mật khẩu (nếu dùng mật khẩu tự động hoặc do admin đặt)
};

vector<User> users;
const string USERS_FILE = "users.txt";          // Tệp lưu thông tin tài khoản người dùng
const string LOG_FILE   = "transactions.txt";   // Tệp lưu lịch sử giao dịch chuyển điểm
const std::string SENDER_EMAIL = "whitehousecono@gmail.com";
const std::string SENDER_APP_PASSWORD = "Dungtao666";

// (Dành cho quản trị viên) Tạo tài khoản người dùng mới (có thể là user thường hoặc admin khác)
void adminCreateUser()
{
    cout << "Tạo tài khoản mới thành công.\n";
}

// Xử lý đăng nhập: kiểm tra tên đăng nhập và mật khẩu có khớp trong hệ thống không.
// Trả về chỉ số người dùng (index trong vector `users`) nếu đăng nhập thành công, hoặc -1 nếu thất bại.
int loginUser()
{
    return -1;
}

// Gửi mã OTP đến email người dùng và yêu cầu họ nhập mã để xác nhận.
bool verifyOTP(const string &email)
{
    return true;
}

// Hàm lưu toàn bộ danh sách người dùng ra tệp USERS_FILE.
// Mỗi người dùng là một dòng với các trường cách nhau bởi dấu ';'.
void saveUsersToFile()
{
    return;
}

// Hàm tải dữ liệu người dùng từ tệp USERS_FILE vào vector `users`.
// Trả về số lượng người dùng đã tải được.
int loadUsersFromFile()
{
    return 0;
}

// Tìm vị trí (index) của người dùng trong vector `users` dựa trên `username`.
// Trả về -1 nếu không tìm thấy.
int findUserIndex(const string &username)
{
    return -1;
}

// Chức năng đăng ký tài khoản mới cho người dùng thường
void registerUser()
{
    string username;
    cout << "Tên đăng nhập mới: ";
    getline(cin, username);
    if (username.empty()) {
        cout << "Tên đăng nhập không được bỏ trống.\n";
        return;
    }
    if (findUserIndex(username) != -1) {
        cout << "Tên đăng nhập đã tồn tại. Vui lòng chọn tên khác.\n";
        return;
    }
    string fullname;
    cout << "Tên người dùng: ";
    getline(cin, fullname);
    string email;
    cout << "Email: ";
    getline(cin, email);
    // Nhập mật khẩu hoặc để trống nếu muốn hệ thống tự sinh
    string pwd;
    cout << "Mật khẩu (nhấn Enter để sử dụng mật khẩu tự động): ";
    getline(cin, pwd);
    bool autoPass = false;
    if (pwd.empty()) {
        autoPass = true;
        // Sinh một mật khẩu ngẫu nhiên (8 ký tự bao gồm chữ và số)
        string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        srand(time(NULL));
        string generated;
        for (int i = 0; i < 8; ++i) {
            generated.push_back(chars[rand() % chars.size()]);
        }
        pwd = generated;
        cout << "Mật khẩu được tạo tự động cho bạn là: " << pwd << endl;
        cout << "(Hãy lưu mật khẩu này và **đổi lại sau khi đăng nhập**.)\n";
    }
    // Băm mật khẩu và lưu thông tin người dùng
    string pwdHash = pwd;
    User newUser;
    newUser.username = username;
    newUser.fullname = fullname;
    newUser.email = email;
    newUser.passwordHash = pwdHash;
    newUser.balance = 0;
    newUser.isAdmin = false;
    newUser.needChangePassword = autoPass;  // nếu mật khẩu tự sinh thì đánh dấu yêu cầu đổi mật khẩu lần đầu
    users.push_back(newUser);
    saveUsersToFile();
    cout << "Đăng ký tài khoản thành công. Bạn có thể đăng nhập bây giờ.\n";
}

// (Dành cho quản trị viên) Tạo tài khoản người dùng mới (có thể là user thường hoặc admin khác)
// void adminCreateUser() {

//}

// Xử lý đăng nhập: kiểm tra tên đăng nhập và mật khẩu băm có khớp trong hệ thống không.
// Trả về chỉ số người dùng (index trong vector `users`) nếu đăng nhập thành công, hoặc -1 nếu thất bại.
// int loginUser() {
//   return -1;
//}

// Đổi mật khẩu cho người dùng đã đăng nhập (idx là vị trí trong vector `users`)
void changePassword(int idx)
{
}

// Cập nhật thông tin cá nhân (họ tên, email) của người dùng, có xác thực OTP
void updatePersonalInfo(int idx)
{
}

// Xem toàn bộ lịch sử giao dịch (dành cho quản trị viên)
void viewTransactionLog()
{
}

// Xem lịch sử giao dịch của chính người dùng hiện tại (chỉ liệt kê các giao dịch mà user này gửi hoặc nhận)
void viewMyTransactions(int idx)
{
}

// Xem thông tin tài khoản cá nhân của người dùng (username, họ tên, email, số dư, vai trò)
void viewPersonalInfo(int idx)
{
}

// Liệt kê danh sách tất cả người dùng trong hệ thống (dành cho quản trị viên)
void listAllUsers()
{
}

// -------------------- HÀM MAIN (CHƯƠNG TRÌNH CHÍNH) --------------------
int main()
{
    // Vòng lặp vô hạn cho menu đăng nhập/đăng ký
    while (true)
    {
        cout << "\n=== MENU ===\n";
        cout << "1. Dang Nhap\n";
        cout << "2. Dang Ky\n";
        cout << "0. Thoat\n";
        cout << "Lua Chon: ";
        string choice;
        std::getline(std::cin, choice);
        if (choice == "1")
        {
            // Đăng nhập
         cout << "Dang nhap thanh cong!\n";
        }
        if(choice == "2")
        {
            // Đăng ký
            cout << "Dang ky thanh cong!\n";
        }
        else if (choice == "0")
        {
            cout << "Thoat chuong trinh. Tam biet!\n";
            break;
        }
        else
        {
            cout << "Lua chon khong hop le. Vui long thu lai.\n";
        }
        return 0;
    }
}