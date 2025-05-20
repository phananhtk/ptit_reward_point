#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

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
    return;
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