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
size_t payload_source(void* ptr, [[maybe_unused]] size_t size, [[maybe_unused]] size_t nmemb, void* userp)
{
    const char **payload = (const char **)userp;
    size_t len = strlen(*payload);
    if (len == 0)
        return 0;
    memcpy(ptr, *payload, len);
    *payload += len;
    return len;
}

bool sendEmailOTP(const string& toEmail, const string& subject, const string& body) {
    CURL* curl;
    CURLcode res = CURLE_OK;
    curl = curl_easy_init();

    if (!curl)
        return false;

    // Replace with your Gmail and App Password
    const string fromEmail = "phananh1304@gmail.com";
    const string appPassword = "dbec imyt kkqm lpaw";

    string fullPayload =
        "To: " + toEmail + "\r\n" +
        "From: " + fromEmail + "\r\n" +
        "Subject: " + subject + "\r\n" +
        "\r\n" + body + "\r\n";

    const char *payload = fullPayload.c_str();

    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_USERNAME, fromEmail.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, appPassword.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, ("<" + fromEmail + ">").c_str());

    struct curl_slist *recipients = NULL;
    recipients = curl_slist_append(recipients, ("<" + toEmail + ">").c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &payload);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    res = curl_easy_perform(curl);

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        return false;
    }

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

// Xử lý đăng nhập: kiểm tra tên đăng nhập và mật khẩu có khớp trong hệ thống không.
// Trả về chỉ số người dùng (index trong vector `users`) nếu đăng nhập thành công, hoặc -1 nếu thất bại.
int loginUser()
{
    string username;
    cout << "Ten dang nhap: ";
    getline(cin, username);
    string password;
    cout << "Mat khau: ";
    getline(cin, password);
    int idx = findUserIndex(username);
    if (idx == -1)
    {
        cout << "Ten dang nhap khong ton tai.\n";
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

bool verifyOTP(const string &email)
{
   // Sinh ngẫu nhiên một mã OTP 6 chữ số
    int otpCode = 100000 + rand() % 900000;
    // Thông báo (giả lập) gửi OTP
    sendEmailOTP(email, "Ma OTP", std::to_string(otpCode));
    cout << "Ma OTP da duoc gui den email cua ban";
    if (!email.empty())
    {
        cout << " (" << email << ")";
    }
    // In ra mã OTP (trong thực tế sẽ gửi qua email, SMS; ở đây in ra để người dùng nhập)
    // cout << "Mã OTP: " << otpCode << endl;
    cout << "Nhap ma OTP de xac nhan: ";
    string input;
    getline(cin, input);
    if (input != to_string(otpCode))
    {
        cout << "Ma OTP khong dung. Huy thao tac.\n";
        return false;
    }
    return true;
}

// Hàm lưu toàn bộ danh sách người dùng ra tệp USERS_FILE.
// Mỗi người dùng là một dòng với các trường cách nhau bởi dấu ';'.
void saveUsersToFile()
{
    ofstream fout(USERS_FILE);
    if (!fout)
    {
        cerr << "Loi: Khong the ghi tep du lieu nguoi dung.\n";
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

// Chức năng đăng ký tài khoản mới cho người dùng thường
void registerUser()
{
     string username;
    cout << "Ten dang nhap moi: ";
    getline(cin, username);
    if (username.empty())
    {
        cout << "Ten dang nhap khong duoc bo trong.\n";
        return;
    }
    if (findUserIndex(username) != -1)
    {
        cout << "Ten dang nhap da ton tai. Vui long chon ten khac.\n";
        return;
    }
    string fullname;
    cout << "Ten nguoi dung: ";
    getline(cin, fullname);
    string email;
    cout << "Email: ";
    getline(cin, email);
    // Nhập mật khẩu hoặc để trống nếu muốn hệ thống tự sinh
    string pwd;
    cout << "Mat khau (nhan Enter de su dung mat khau tu dong): ";
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
        cout << "Mat khau duoc tao tu dong cho ban la: " << pwd << endl;
        cout << "(Hay luu mat khau nay va **doi lai sau khi dang nhap**.)\n";
    }
    // Băm mật khẩu và lưu thông tin người dùng
    string pwdHash = sha256(pwd);
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
    cout << "Dang ky tai khoan thanh cong. Ban co the dang nhap bay gio.\n";
}

// Đổi mật khẩu cho người dùng đã đăng nhập (idx là vị trí trong vector `users`)
void changePassword(int idx) {
     string currentPwd;
    cout << "Nhap mat khau hien tai: ";
    getline(cin, currentPwd);
    string currentHash = sha256(currentPwd);;
    if (currentHash != users[idx].passwordHash)
    {
        cout << "Mat khau hien tai khong chinh xac.\n";
        return;
    }
    string newPwd, confirmPwd;
    cout << "Mat khau moi: ";
    getline(cin, newPwd);
    cout << "Xac nhan mat khau moi: ";
    getline(cin, confirmPwd);
    if (newPwd.empty())
    {
        cout << "Mat khau moi khong duoc de trong.\n";
        return;
    }
    if (newPwd != confirmPwd)
    {
        cout << "Mat khau xac nhan khong khop.\n";
        return;
    }
    if (sha256(newPwd)  == users[idx].passwordHash)
    {
        cout << "Mat khau moi trung voi mat khau cu. Hay chon mat khau khac.\n";
        return;
    }
    // Cập nhật mật khẩu
    users[idx].passwordHash = sha256(newPwd);
    users[idx].needChangePassword = false; // sau khi tự đổi mật khẩu thì không cần đổi nữa
    saveUsersToFile();
    cout << "Doi mat khau thanh cong.\n";
}

// Cập nhật thông tin cá nhân (họ tên, email) của người dùng, có xác thực OTP
void updatePersonalInfo(int idx) {
    cout << "Ten hien tai: " << users[idx].fullname << ". Nhap ten moi (Enter de giu nguyen): ";
    string newName;
    string input;
    getline(cin, input);
    if (!input.empty())
        newName = input;
    else
        newName = users[idx].fullname;
    cout << "Email hien tai: " << users[idx].email << ". Nhap email moi (Enter de giu nguyen): ";
    string newEmail;
    input.clear();
    getline(cin, input);
    if (!input.empty())
        newEmail = input;
    else
        newEmail = users[idx].email;
    if (newName == users[idx].fullname && newEmail == users[idx].email)
    {
        cout << "Khong co thay doi thong tin.\n";
        return;
    }
    // Yêu cầu xác thực OTP trước khi thay đổi thông tin quan trọng
    if (!verifyOTP(users[idx].email))
    {
        // Nếu OTP sai, hủy thao tác
        return;
    }
    // Cập nhật thông tin nếu OTP đúng
    users[idx].fullname = newName;
    users[idx].email = newEmail;
    saveUsersToFile();
    cout << "Cap nhat thong tin ca nhan thanh cong.\n";
}

// Thực hiện chuyển điểm từ tài khoản người dùng hiện tại (fromIdx) đến tài khoản đích (toUsername).
// Có xác thực OTP và đảm bảo tính nguyên tử của giao dịch.
void transferPoints(int fromIdx) {
    string toUsername;
    cout << "Nhập tên tài khoản người nhận: ";
    getline(cin, toUsername);
    int toIdx = findUserIndex(toUsername);
    if (toIdx == -1) {
        cout << "Tài khoản người nhận không tồn tại.\n";
        return;
    }
    if (toIdx == fromIdx) {
        cout << "Bạn không thể chuyển điểm cho chính mình.\n";
        return;
    }
    string amountStr;
    cout << "Nhập số điểm cần chuyển: ";
    getline(cin, amountStr);
    long long amount = 0;
    try {
        amount = stoll(amountStr);
    } catch (...) {
        amount = 0;
    }
    if (amount <= 0) {
        cout << "Số điểm phải là số dương lớn hơn 0.\n";
        return;
    }
    if (users[fromIdx].balance < amount) {
        cout << "Số dư không đủ để thực hiện giao dịch.\n";
        return;
    }
    // Yêu cầu OTP xác nhận giao dịch chuyển điểm
    if (!verifyOTP(users[fromIdx].email)) {
        return;
    }
    // Thực hiện giao dịch chuyển điểm (đảm bảo nguyên tử)
    long long &senderBal = users[fromIdx].balance;
    long long &recvBal   = users[toIdx].balance;
    // Lưu lại số dư ban đầu để có thể khôi phục nếu cần (trường hợp giao dịch lỗi)
    long long oldSenderBal = senderBal;
    long long oldRecvBal   = recvBal;
    // 1. Trừ điểm từ ví người gửi
    senderBal -= amount;
    // 2. Cộng điểm vào ví người nhận
    recvBal   += amount;
    // 3. Lưu thay đổi xuống tệp dữ liệu người dùng (giả định không lỗi)
    if (!ofstream(USERS_FILE)) {
        // Nếu mở tệp không thành công (lỗi ghi file), khôi phục số dư cũ và hủy giao dịch
        senderBal = oldSenderBal;
        recvBal   = oldRecvBal;
        cerr << "Lỗi hệ thống: không thể ghi dữ liệu. Giao dịch bị hủy.\n";
        return;
    }
    saveUsersToFile();
    // 4. Ghi lịch sử giao dịch vào tệp log
    ofstream fout(LOG_FILE, ios::app);
    if (!fout) {
        cerr << "Không thể ghi lịch sử giao dịch (nhưng giao dịch đã hoàn tất).\n";
    } else {
        // Lấy thời gian hiện tại (timestamp)
        auto now = chrono::system_clock::now();
        time_t t = chrono::system_clock::to_time_t(now);
        tm localtm = *localtime(&t);
        // Ghi vào log: thời gian, người gửi, người nhận, số điểm
        fout << put_time(&localtm, "%Y-%m-%d %H:%M:%S") << ", "
             << users[fromIdx].username << ", "
             << users[toIdx].username << ", "
             << amount << "\n";
        fout.close();
    }
    cout << "Chuyển điểm thành công. Số dư hiện tại của bạn là " << users[fromIdx].balance << " điểm.\n";
}

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
    string pwdHash = sha256(pwd);
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

// Thực hiện chuyển điểm từ tài khoản người dùng hiện tại (fromIdx) đến tài khoản đích (toUsername).
// Có xác thực OTP và đảm bảo tính nguyên tử của giao dịch.
void transferPoints(int fromIdx) {
    string toUsername;
    cout << "Nhập tên tài khoản người nhận: ";
    getline(cin, toUsername);
    int toIdx = findUserIndex(toUsername);
    if (toIdx == -1) {
        cout << "Tài khoản người nhận không tồn tại.\n";
        return;
    }
    if (toIdx == fromIdx) {
        cout << "Bạn không thể chuyển điểm cho chính mình.\n";
        return;
    }
    string amountStr;
    cout << "Nhập số điểm cần chuyển: ";
    getline(cin, amountStr);
    long long amount = 0;
    try {
        amount = stoll(amountStr);
    } catch (...) {
        amount = 0;
    }
    if (amount <= 0) {
        cout << "Số điểm phải là số dương lớn hơn 0.\n";
        return;
    }
    if (users[fromIdx].balance < amount) {
        cout << "Số dư không đủ để thực hiện giao dịch.\n";
        return;
    }
    // Yêu cầu OTP xác nhận giao dịch chuyển điểm
    if (!verifyOTP(users[fromIdx].email)) {
        return;
    }
    // Thực hiện giao dịch chuyển điểm (đảm bảo nguyên tử)
    long long &senderBal = users[fromIdx].balance;
    long long &recvBal   = users[toIdx].balance;
    // Lưu lại số dư ban đầu để có thể khôi phục nếu cần (trường hợp giao dịch lỗi)
    long long oldSenderBal = senderBal;
    long long oldRecvBal   = recvBal;
    // 1. Trừ điểm từ ví người gửi
    senderBal -= amount;
    // 2. Cộng điểm vào ví người nhận
    recvBal   += amount;
    // 3. Lưu thay đổi xuống tệp dữ liệu người dùng (giả định không lỗi)
    if (!ofstream(USERS_FILE)) {
        // Nếu mở tệp không thành công (lỗi ghi file), khôi phục số dư cũ và hủy giao dịch
        senderBal = oldSenderBal;
        recvBal   = oldRecvBal;
        cerr << "Lỗi hệ thống: không thể ghi dữ liệu. Giao dịch bị hủy.\n";
        return;
    }
    saveUsersToFile();
    // 4. Ghi lịch sử giao dịch vào tệp log
    ofstream fout(LOG_FILE, ios::app);
    if (!fout) {
        cerr << "Không thể ghi lịch sử giao dịch (nhưng giao dịch đã hoàn tất).\n";
    } else {
        // Lấy thời gian hiện tại (timestamp)
        auto now = chrono::system_clock::now();
        time_t t = chrono::system_clock::to_time_t(now);
        tm localtm = *localtime(&t);
        // Ghi vào log: thời gian, người gửi, người nhận, số điểm
        fout << put_time(&localtm, "%Y-%m-%d %H:%M:%S") << ", "
             << users[fromIdx].username << ", "
             << users[toIdx].username << ", "
             << amount << "\n";
        fout.close();
    }
    cout << "Chuyển điểm thành công. Số dư hiện tại của bạn là " << users[fromIdx].balance << " điểm.\n";
}

// Xem toàn bộ lịch sử giao dịch (dành cho quản trị viên)
void viewTransactionLog() {
    ifstream fin(LOG_FILE);
    if (!fin) {
        cout << "Không có lịch sử giao dịch nào.\n";
        return;
    }
    cout << "===== LỊCH SỬ GIAO DỊCH HỆ THỐNG =====\n";
    string line;
    while (getline(fin, line)) {
        cout << line << endl;
    }
    fin.close();
}

// Xem lịch sử giao dịch của chính người dùng hiện tại (chỉ liệt kê các giao dịch mà user này gửi hoặc nhận)
void viewMyTransactions(int idx) {
    ifstream fin(LOG_FILE);
    if (!fin) {
        cout << "Không có lịch sử giao dịch.\n";
        return;
    }
    cout << "===== LỊCH SỬ GIAO DỊCH CỦA BẠN =====\n";
    string line;
    string uname = users[idx].username;
    // Duyệt từng dòng trong log, kiểm tra sự xuất hiện của username (ở cột người gửi hoặc người nhận)
    while (getline(fin, line)) {
        // Mỗi dòng format: TIMESTAMP, FROM_USER, TO_USER, AMOUNT
        // Sử dụng stringstream để tách các phần
        string ts, fromUser, toUser;
        long long amount;
        stringstream ss(line);
        getline(ss, ts, ',');
        getline(ss, fromUser, ',');
        getline(ss, toUser, ',');
        string amountStr;
        getline(ss, amountStr, ',');
        // Loại bỏ khoảng trắng thừa xung quanh các phần tử đã tách
        auto trim = [](string &s) {
            while (!s.empty() && isspace(s.back())) s.pop_back();
            while (!s.empty() && isspace(s.front())) s.erase(0, 1);
        };
        trim(ts);
        trim(fromUser);
        trim(toUser);
        trim(amountStr);
        try {
            amount = stoll(amountStr);
        } catch (...) {
            amount = 0;
        }
        // Kiểm tra nếu user hiện tại là người gửi hoặc người nhận
        if (fromUser == uname || toUser == uname) {
            // Xác định vai trò của user trong giao dịch (gửi hoặc nhận)
            string action = (fromUser == uname) ? "đã gửi" : "đã nhận";
            string otherUser = (fromUser == uname) ? toUser : fromUser;
            string direction = (fromUser == uname) ? "đến" : "từ";
            cout << ts << ": " << action << " " << amount << " điểm " << direction << " tài khoản " << otherUser << ".\n";
        }
    }
    fin.close();
}

// Xem thông tin tài khoản cá nhân của người dùng (username, họ tên, email, số dư, vai trò)
void viewPersonalInfo(int idx) {
    cout << "===== THÔNG TIN TÀI KHOẢN =====\n";
    cout << "Tên đăng nhập:  " << users[idx].username << endl;
    cout << "Họ và tên:      " << users[idx].fullname << endl;
    cout << "Email:          " << users[idx].email << endl;
    cout << "Số dư (điểm):   " << users[idx].balance << endl;
    cout << "Loại tài khoản: " << (users[idx].isAdmin ? "Quản trị viên" : "Người dùng thường") << endl;
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
            // Trường hợp ngắt input (EOF)
            break;
        }
        if (choice == "1") {
            // Xử lý đăng nhập
            int userIdx = loginUser();
            if (userIdx >= 0) {
                // Đăng nhập thành công
                // Nếu tài khoản đang dùng mật khẩu tạm (tự sinh hoặc do admin đặt), bắt buộc đổi mật khẩu mới
                if (users[userIdx].needChangePassword) {
                    cout << "\nBạn đang sử dụng mật khẩu tạm thời. Vui lòng đổi mật khẩu mới.\n";
                    string newPwd, confirmPwd;
                    // Bắt người dùng đổi mật khẩu đến khi hợp lệ
                    do {
                        cout << "Mật khẩu mới: ";
                        getline(cin, newPwd);
                        cout << "Xác nhận mật khẩu mới: ";
                        getline(cin, confirmPwd);
                        if (newPwd.empty()) {
                            cout << "Mật khẩu mới không được để trống.\n";
                        } else if (newPwd != confirmPwd) {
                            cout << "Mật khẩu xác nhận không khớp.\n";
                        } else if (sha256(newPwd) == users[userIdx].passwordHash) {
                            // Không cho phép trùng với mật khẩu cũ (tạm thời)
                            cout << "Mật khẩu mới trùng với mật khẩu tạm thời. Vui lòng chọn mật khẩu khác.\n";
                        } else {
                            // Mật khẩu hợp lệ và khác mật khẩu cũ
                            break;
                        }
                    } while (true);
                    // Cập nhật mật khẩu mới cho tài khoản
                    users[userIdx].passwordHash = sha256(newPwd);
                    users[userIdx].needChangePassword = false;
                    saveUsersToFile();
                    cout << "Đổi mật khẩu thành công. Bạn đã có thể sử dụng hệ thống.\n";
                }
                // Phân chia menu theo loại người dùng (admin hoặc thường)
                if (users[userIdx].isAdmin) {
                    // Menu dành cho quản trị viên
                    while (true) {
                        cout << "\n--- MENU QUẢN TRỊ VIÊN ---\n";
                        cout << "1. Xem thông tin cá nhân\n";
                        cout << "2. Đổi mật khẩu\n";
                        cout << "3. Cập nhật thông tin cá nhân\n";
                        cout << "4. Chuyển điểm cho người dùng\n";
                        cout << "5. Xem danh sách người dùng\n";
                        cout << "6. Tạo tài khoản mới\n";
                        cout << "7. Xem lịch sử giao dịch\n";
                        cout << "0. Đăng xuất\n";
                        cout << "Lựa chọn: ";
                        string adminChoice;
                        if (!getline(cin, adminChoice)) {
                            return 0;  // kết thúc chương trình nếu không nhận được input
                        }
                        if (adminChoice == "1") {
                            viewPersonalInfo(userIdx);
                        } else if (adminChoice == "2") {
                            changePassword(userIdx);
                        } else if (adminChoice == "3") {
                            updatePersonalInfo(userIdx);
                        } else if (adminChoice == "4") {
                            transferPoints(userIdx);
                        } else if (adminChoice == "5") {
                            listAllUsers();
                        } else if (adminChoice == "6") {
                            adminCreateUser();
                        } else if (adminChoice == "7") {
                            viewTransactionLog();
                        } else if (adminChoice == "0") {
                            cout << "Đăng xuất khỏi tài khoản quản trị viên.\n";
                            break;
                        } else {
                            cout << "Lựa chọn không hợp lệ. Vui lòng thử lại.\n";
                        }
                    }
                } else {
                    // Menu dành cho người dùng thường
                    while (true) {
                        cout << "\n--- MENU NGƯỜI DÙNG ---\n";
                        cout << "1. Xem thông tin cá nhân\n";
                        cout << "2. Đổi mật khẩu\n";
                        cout << "3. Cập nhật thông tin cá nhân\n";
                        cout << "4. Chuyển điểm\n";
                        cout << "5. Xem lịch sử giao dịch của tôi\n";
                        cout << "0. Đăng xuất\n";
                        cout << "Lựa chọn: ";
                        string userChoice;
                        if (!getline(cin, userChoice)) {
                            return 0;
                        }
                        if (userChoice == "1") {
                            viewPersonalInfo(userIdx);
                        } else if (userChoice == "2") {
                            changePassword(userIdx);
                        } else if (userChoice == "3") {
                            updatePersonalInfo(userIdx);
                        } else if (userChoice == "4") {
                            transferPoints(userIdx);
                        } else if (userChoice == "5") {
                            viewMyTransactions(userIdx);
                        } else if (userChoice == "0") {
                            cout << "Đăng xuất khỏi tài khoản người dùng.\n";
                            break;
                        } else {
                            cout << "Lựa chọn không hợp lệ. Vui lòng chọn lại.\n";
                        }
                    }
                }
            }
            // Nếu đăng nhập thất bại, vòng lặp menu chính sẽ tiếp tục (cho phép chọn lại)
        } else if (choice == "2") {
            // Xử lý đăng ký người dùng mới
            registerUser();
        } else if (choice == "0") {
            cout << "Thoát chương trình. Tạm biệt!\n";
            break;
        } else {
            cout << "Lựa chọn không hợp lệ. Vui lòng thử lại.\n";
        }
    }

    return 0;
}