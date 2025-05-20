#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <curl/curl.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

using namespace std;

// Hàm băm SHA-256 để lưu mật khẩu an toàn vào hệ thống.
string sha256(const string& str) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) return "";

    if (1 != EVP_DigestInit_ex(ctx, EVP_sha256(), NULL)) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    if (1 != EVP_DigestUpdate(ctx, str.c_str(), str.size())) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    if (1 != EVP_DigestFinal_ex(ctx, hash, &hash_len)) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    EVP_MD_CTX_free(ctx);

    stringstream ss;
    for (unsigned int i = 0; i < hash_len; ++i) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}
// Tao payload de send SMTP mail
size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp) {
    const char** payload = (const char**)userp;
    size_t len = strlen(*payload);
    if (len == 0) return 0;
    memcpy(ptr, *payload, len);
    *payload += len;
    return len;
}

bool sendEmailOTP() {
    CURL* curl;
    CURLcode res = CURLE_OK;
    curl = curl_easy_init();

    if (!curl) return false;

    // Replace with your Gmail and App Password
    const string fromEmail = "phananh1304@gmail.com";
    const string appPassword = "dbec imyt kkqm lpaw";

    string fullPayload =
        "To: " + toEmail + "\r\n" +
        "From: " + fromEmail + "\r\n" +
        "Subject: " + subject + "\r\n" +
        "\r\n" + body + "\r\n";

    const char* payload = fullPayload.c_str();

    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_USERNAME, fromEmail.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, appPassword.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, ("<" + fromEmail + ">").c_str());

    struct curl_slist* recipients = NULL;
    recipients = curl_slist_append(recipients, ("<" + toEmail + ">").c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &payload);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    res = curl_easy_perform(curl);

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        return false;
    }

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
    string hashInput = sha256(password);
    if (hashInput != users[idx].passwordHash) {
        cout << "Mật khẩu không đúng.\n";
        return -1;
    }
    return idx;
}

// Gửi mã OTP đến email người dùng và yêu cầu họ nhập mã để xác nhận.
bool verifyOTP(const string &email) {
    // Sinh ngẫu nhiên một mã OTP 6 chữ số
    int otpCode = 100000 + rand() % 900000;
    // Thông báo gửi OTP
    sendEmailOTP(email, "Mã OTP", std::to_string(otpCode));
    cout << "Mã OTP đã được gửi đến email của bạn";
    if (!email.empty()) {
        cout << " (" << email << ")";
    }
    cout << "Nhập mã OTP để xác nhận: ";
    string input;
    getline(cin, input);
    if (input != to_string(otpCode)) {
        cout << "Mã OTP không đúng. Hủy thao tác.\n";
        return false;
    }
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
    // Tải dữ liệu người dùng từ tệp vào vector `users`
    int userCount = loadUsersFromFile();
    if (userCount == 0) {
        // Nếu hệ thống chưa có người dùng nào, tự động tạo tài khoản quản trị mặc định
        User admin;
        admin.username = "admin";
        admin.fullname = "Administrator";
        admin.email = "";  // chưa có email
        string defaultPass = "admin";
        admin.passwordHash = sha256(defaultPass);
        admin.balance = 1000;       // khởi tạo ví admin với 1000 điểm để thử giao dịch
        admin.isAdmin = true;
        admin.needChangePassword = true;  // yêu cầu đổi mật khẩu do dùng mật khẩu mặc định
        users.push_back(admin);
        saveUsersToFile();
        cout << "Đã tạo tài khoản quản trị mặc định (username: admin, password: admin). Vui lòng đăng nhập và đổi mật khẩu.\n";
    }
    // Vòng lặp vô hạn cho menu đăng nhập/đăng ký
    while (true) {
        cout << "\n=== MENU ===\n";
        cout << "1. Đăng nhập\n";
        cout << "2. Đăng ký\n";
        cout << "0. Thoát\n";
        cout << "Lựa chọn: ";
        string choice;
        if (!getline(cin, choice)) {
            break;
        }
    }

    return 0;
}