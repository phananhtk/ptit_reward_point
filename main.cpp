#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

bool sendEmailOTP() {
    return true;
}

// Cấu trúc lưu thông tin một người dùng trong hệ thống
struct User {
    string username;         // Tên đăng nhập
    string passwordHash;     // Mật khẩu đã băm (SHA-256)
    string fullname;         // Họ tên đầy đủ
    string email;            // Email liên hệ
    long long balance;       // Số dư ví điểm thưởng
    bool isAdmin;            // Quyền quản trị (true nếu là admin)
    bool needChangePassword; // Đánh dấu cần đổi mật khẩu (nếu dùng mật khẩu tự động hoặc do admin đặt)
};

// Danh sách người dùng (tải từ tệp khi khởi động chương trình)
vector<User> users;
const string USERS_FILE = "users.txt";          // Tệp lưu thông tin tài khoản người dùng

// (Dành cho quản trị viên) Tạo tài khoản người dùng mới (có thể là user thường hoặc admin khác)
void adminCreateUser() {
    cout << "Tạo tài khoản mới thành công.\n";
}

// Xử lý đăng nhập: kiểm tra tên đăng nhập và mật khẩu có khớp trong hệ thống không.
// Trả về chỉ số người dùng (index trong vector `users`) nếu đăng nhập thành công, hoặc -1 nếu thất bại.
int loginUser() {
    string username;
    cout << "Tên đăng nhập: ";
    getline(cin, username);
    string password;
    cout << "Mật khẩu: ";
    getline(cin, password);
    int idx = findUserIndex(username);
    if (idx == -1) {
        cout << "Tên đăng nhập không tồn tại.\n";
        return -1;
    }
    string hashInput = password;
    if (hashInput != users[idx].passwordHash) {
        cout << "Mật khẩu không đúng.\n";
        return -1;
    }
    return idx;
}

// Gửi mã OTP đến email người dùng và yêu cầu họ nhập mã để xác nhận.
bool verifyOTP(const string &email) {
    return true;
}

// Hàm lưu toàn bộ danh sách người dùng ra tệp USERS_FILE.
// Mỗi người dùng là một dòng với các trường cách nhau bởi dấu ';'.
void saveUsersToFile() {
    ofstream fout(USERS_FILE);
    if (!fout) {
        cerr << "Lỗi: Không thể ghi tệp dữ liệu người dùng.\n";
        return;
    }
    for (const auto &u : users) {
        fout << u.username << ";"
             << (u.isAdmin ? "admin" : "user") << ";"    // lưu vai trò
             << u.passwordHash << ";"
             << u.balance << ";"
             << u.fullname << ";"
             << u.email << ";"
             << (u.needChangePassword ? "1" : "0")       // 1 nếu cần đổi mật khẩu, 0 nếu không
             << "\n";
    }
    fout.close();
}

// Hàm tải dữ liệu người dùng từ tệp USERS_FILE vào vector `users`.
// Trả về số lượng người dùng đã tải được.
int loadUsersFromFile() {
    return 0;
}

// Tìm vị trí (index) của người dùng trong vector `users` dựa trên `username`.
// Trả về -1 nếu không tìm thấy.
int findUserIndex(const string &username) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].username == username) {
            return (int)i;
        }
    }
    return -1;
}

// Chức năng đăng ký tài khoản mới cho người dùng thường
void registerUser() {
}

// (Dành cho quản trị viên) Tạo tài khoản người dùng mới (có thể là user thường hoặc admin khác)
void adminCreateUser() {
}

// Xử lý đăng nhập: kiểm tra tên đăng nhập và mật khẩu băm có khớp trong hệ thống không.
// Trả về chỉ số người dùng (index trong vector `users`) nếu đăng nhập thành công, hoặc -1 nếu thất bại.
int loginUser() {
    return -1;
}

// Đổi mật khẩu cho người dùng đã đăng nhập (idx là vị trí trong vector `users`)
void changePassword(int idx) {
}

// Cập nhật thông tin cá nhân (họ tên, email) của người dùng, có xác thực OTP
void updatePersonalInfo(int idx) {
}

// Xem toàn bộ lịch sử giao dịch (dành cho quản trị viên)
void viewTransactionLog() {
}

// Xem lịch sử giao dịch của chính người dùng hiện tại (chỉ liệt kê các giao dịch mà user này gửi hoặc nhận)
void viewMyTransactions(int idx) {
}

// Xem thông tin tài khoản cá nhân của người dùng (username, họ tên, email, số dư, vai trò)
void viewPersonalInfo(int idx) {
}

// Liệt kê danh sách tất cả người dùng trong hệ thống (dành cho quản trị viên)
void listAllUsers() {
}



// -------------------- HÀM MAIN (CHƯƠNG TRÌNH CHÍNH) --------------------
int main() {
    // Vòng lặp vô hạn cho menu đăng nhập/đăng ký
    while (true) {
        cout << "\n=== MENU ===\n";
        cout << "1. Đăng nhập\n";
        cout << "2. Đăng ký\n";
        cout << "0. Thoát\n";
        cout << "Lựa chọn: ";
        string choice;
    }

    return 0;
}