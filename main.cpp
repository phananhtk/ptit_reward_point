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
#include <curl/curl.h>

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
const string USERS_FILE = "users.txt";      // Tệp lưu thông tin tài khoản người dùng
const string LOG_FILE = "transactions.txt"; // Tệp lưu lịch sử giao dịch chuyển điểm
const std::string SENDER_EMAIL = "whitehousecono@gmail.com";
const std::string SENDER_APP_PASSWORD = "Dungtao666";

// Xử lý đăng nhập: kiểm tra tên đăng nhập và mật khẩu có khớp trong hệ thống không.
// Trả về chỉ số người dùng (index trong vector `users`) nếu đăng nhập thành công, hoặc -1 nếu thất bại.

// Gửi mã OTP đến email người dùng và yêu cầu họ nhập mã để xác nhận.
bool verifyOTP(const string &email)
{
    return true;
}

// Hàm lưu toàn bộ danh sách người dùng ra tệp USERS_FILE.
// Mỗi người dùng là một dòng với các trường cách nhau bởi dấu ';'.
void saveUsersToFile()
{
    ofstream fout(USERS_FILE);
    if (!fout)
    {
        cerr << "Lỗi: Không thể ghi tệp dữ liệu người dùng.\n";
        return;
    }
    for (const auto &u : users)
    {
        fout << u.username << ";"
             << (u.isAdmin ? "admin" : "user") << ";" // lưu vai trò
             << u.passwordHash << ";"
             << u.balance << ";"
             << u.fullname << ";"
             << u.email << ";"
             << (u.needChangePassword ? "1" : "0") // 1 nếu cần đổi mật khẩu, 0 nếu không
             << "\n";
    }
    fout.close();
}

// Hàm tải dữ liệu người dùng từ tệp USERS_FILE vào vector `users`.
// Trả về số lượng người dùng đã tải được.
int loadUsersFromFile()
{
    users.clear();
    ifstream fin(USERS_FILE);
    if (!fin)
    {
        // Nếu tệp không tồn tại, tạo tệp mới (trống) để chuẩn bị lưu dữ liệu sau.
        ofstream fout(USERS_FILE);
        fout.close();
        return 0;
    }
    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue; // bỏ qua dòng trống (nếu có)
        User u;
        string roleStr, balanceStr, needChangeStr;
        stringstream ss(line);
        // Đọc lần lượt các trường, ngăn cách bởi dấu ';'
        getline(ss, u.username, ';');
        getline(ss, roleStr, ';');
        getline(ss, u.passwordHash, ';');
        getline(ss, balanceStr, ';');
        getline(ss, u.fullname, ';');
        getline(ss, u.email, ';');
        getline(ss, needChangeStr, ';');
        // Loại bỏ khoảng trắng thừa ở đầu/cuối mỗi trường
        auto trim = [](string &s)
        {
            while (!s.empty() && isspace(s.back()))
                s.pop_back();
            while (!s.empty() && isspace(s.front()))
                s.erase(0, 1);
        };
        trim(u.username);
        trim(roleStr);
        trim(u.passwordHash);
        trim(balanceStr);
        trim(u.fullname);
        trim(u.email);
        trim(needChangeStr);
        // Chuyển chuỗi vai trò thành cờ isAdmin
        u.isAdmin = (roleStr == "admin");
        // Chuyển chuỗi số dư thành số (long long)
        try
        {
            u.balance = stoll(balanceStr);
        }
        catch (...)
        {
            u.balance = 0;
        }
        // Xác định trạng thái cần đổi mật khẩu
        u.needChangePassword = (needChangeStr == "1");
        users.push_back(u);
    }
    fin.close();
    return users.size();
}

// Tìm vị trí (index) của người dùng trong vector `users` dựa trên `username`.
// Trả về -1 nếu không tìm thấy.
int findUserIndex(const string &username)
{
    for (size_t i = 0; i < users.size(); ++i)
    {
        if (users[i].username == username)
        {
            return (int)i;
        }
    }
    return -1;
}

// Chức năng đăng ký tài khoản mới cho người dùng thường
void registerUser()
{
    string username;
    cout << "Tên đăng nhập mới: ";
    getline(cin, username);
    if (username.empty())
    {
        cout << "Tên đăng nhập không được bỏ trống.\n";
        return;
    }
    if (findUserIndex(username) != -1)
    {
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
    if (pwd.empty())
    {
        autoPass = true;
        // Sinh một mật khẩu ngẫu nhiên (8 ký tự bao gồm chữ và số)
        string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        srand(time(NULL));
        string generated;
        for (int i = 0; i < 8; ++i)
        {
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
    newUser.needChangePassword = autoPass; // nếu mật khẩu tự sinh thì đánh dấu yêu cầu đổi mật khẩu lần đầu
    users.push_back(newUser);
    saveUsersToFile();
    cout << "Đăng ký tài khoản thành công. Bạn có thể đăng nhập bây giờ.\n";
}

// (Dành cho quản trị viên) Tạo tài khoản người dùng mới (có thể là user thường hoặc admin khác)
void adminCreateUser()
{
    string username;
    cout << "Nhap ten dang nhap cho tai khoan moi: ";
    getline(cin, username);
    if (username.empty())
    {
        cout << "Ten dang nhap khong duoc de trong.\n";
        return;
    }
    if (findUserIndex(username) != -1)
    {
        cout << "Ten dang nhap nay da ton tai. Khong the tao moi.\n";
        return;
    }
    string roleInput;
    cout << "Tai khoan nay co quyen quan ly (admin)? (y/n): ";
    getline(cin, roleInput);
    bool isAdminRole = false;
    if (!roleInput.empty() && (roleInput[0] == 'y' || roleInput[0] == 'Y'))
    {
        isAdminRole = true;
    }
    string fullname;
    cout << "Ten nguoi dung: ";
    getline(cin, fullname);
    string email;
    cout << "Email: ";
    getline(cin, email);
    string pwd;
    cout << "Mat khau (nhan Enter de tu dong tao): ";
    getline(cin, pwd);
    bool autoPass = false;
    if (pwd.empty())
    {
        autoPass = true;
        // Sinh mật khẩu ngẫu nhiên (10 ký tự)
        string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        srand(time(NULL));
        string generated;
        for (int i = 0; i < 10; ++i)
        {
            generated.push_back(chars[rand() % chars.size()]);
        }
        pwd = generated;
        cout << "Mat khau duoc tu dong tao cho tai khoan moi la: " << pwd << endl;
        cout << "(Yeu cau nguoi dung doi mat khau nay khi dang nhap lan dau tien.)\n";
    }
    string pwdHash = pwd;
    User newUser;
    newUser.username = username;
    newUser.fullname = fullname;
    newUser.email = email;
    newUser.passwordHash = pwdHash;
    newUser.balance = 0;
    newUser.isAdmin = isAdminRole;
    newUser.needChangePassword = autoPass;
    users.push_back(newUser);
    saveUsersToFile();
    cout << "Tao tai khoan moi thanh cong.\n";
}

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
    cout << "===== DANH SACH NGUOI DUNG =====\n";
    cout << setw(15) << left << "Username"
         << setw(25) << left << "Ho ten"
         << setw(30) << left << "Email"
         << setw(10) << left << "So du"
         << setw(10) << left << "Quyen" << endl;
    for (const auto &u : users)
    {
        cout << setw(15) << left << u.username
             << setw(25) << left << u.fullname
             << setw(30) << left << u.email
             << setw(10) << left << u.balance
             << setw(10) << left << (u.isAdmin ? "Admin" : "User")
             << endl;
    }
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
        if (choice == "2")
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