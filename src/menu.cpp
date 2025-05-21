#include <iostream>
#include "login.h"
#include "user.h"
#include "get_user_info.h"
#include "update_user.h"
#include "transfer.h"
#include "menu.h"
#include "admin.h"
#include "utils.h"
#include "add_user.h"
#include "delete_user.h"
using namespace std;

const std::string USER_FILE = "../data/users.xlsx";
const std::string LOG_FILE = "../data/logs.xlsx";

int getIntInput(const std::string& prompt, int min = 0, int max = 10) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < min || value > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "⚠️ Vui lòng nhập số từ " << min << " đến " << max << ".\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

void showMainMenu() {
    cout << "===============================\n";
    cout << " CHAO MUNG DEN HE THONG VI DIEM\n";
    cout << "===============================\n";
    cout << "1. Dang nhap\n";
    cout << "2. Dang ky\n";
    cout << "0. Thoat\n";
    cout << "===============================\n";
}

void userMenu(const User &user) {
    int choice;
    do {
        cout << "===============================\n";
        cout << " CHAO MUNG DEN HE THONG VI DIEM\n";
        cout << "===============================\n";
        cout << "1. Kiểm tra thông tin\n";
        cout << "2. Cập nhật thông tin\n";
        cout << "3. Giao dịch\n";
        cout << "4. Xóa tài khoản\n";
        cout << "0. Thoát\n";
        cout << "===============================\n";

        choice = getIntInput("Nhap lua chon cua ban: ", 0, 4);

        switch (choice) {
        case 1:
            getUserInfo(USER_FILE, user.username);
            break;
        case 2:
            updateUserInfo(user.username, "123456");
            break;
        case 3:
            transferPoints(USER_FILE, LOG_FILE, user.username);
            break;
        case 4:
            {
                string targetUser;
                cout << "Nhập tài khoản cần xóa: ";
                cin >> targetUser;
                char confirm;
                cout << "Bạn có chắc chắn muốn xóa tài khoản " << targetUser << "? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    deleteUser(USER_FILE, targetUser, false);
                } else {
                    cout << "Đã hủy xóa tài khoản.\n";
                }
                break;
            }
        case 0:
            cout << " Dang xuat khoi tai khoan nguoi dung...\n";
            break;
        default:
            cout << " Lua chon khong hop le! Vui long chon lai.\n";
            break;
        }
        cout << endl;
    } while (choice != 0);
}

void adminMenu(const User &user) {
    int choice;
    do {
        cout << "=== MENU QUAN LY (Admin) ===\n";
        cout << "1. Xem danh sách người dùng\n";
        cout << "2. Tạo tài khoản mới\n";
        cout << "3. Chỉnh sửa tài khoản\n";
        cout << "4. Chuyển điểm giữa các tài khoản\n";
        cout << "5. Xóa tài khoản\n";
        cout << "0. Đăng xuất\n";

        choice = getIntInput("Chọn chức năng: ", 0, 5);

        switch (choice) {
        case 1:
            viewUserList(USER_FILE);
            break;
        case 2:
            {
                string targetRole;
                cout << "Nhập role cần thêm mới (admin hoặc user): ";
                cin >> targetRole;
                addUser(USER_FILE, targetRole);
                break;
            }
        case 3:
            {
                string targetUser;
                cout << "Nhập username cần cập nhật: ";
                cin >> targetUser;
                updateUserInfo(targetUser, "", true);
                break;
            }
        case 4:
            transferPoints(USER_FILE, LOG_FILE, user.username);
            break;
        case 5:
            {
                string targetUser;
                cout << "Nhập tài khoản cần xóa: ";
                cin >> targetUser;
                char confirm;
                cout << "Bạn có chắc chắn muốn xóa tài khoản " << targetUser << "? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    deleteUser(USER_FILE, targetUser, true);
                } else {
                    cout << "Đã hủy xóa tài khoản.\n";
                }
                break;
            }
        case 0:
            cout << "Đang đăng xuất...\n";
            break;
        default:
            cout << " Lựa chọn không hợp lệ!\n";
        }
    } while (choice != 0);
}

pair<string, string> getLoginInput() {
    string username, password;
    cout << "Nhap ten dang nhap: ";
    cin >> username;
    cout << "Nhap mat khau: ";
    cin >> password;
    return {username, password};
}

void handleMenu() {
    int choice;
    do {
        showMainMenu();
        choice = getIntInput("Nhap lua chon cua ban: ", 0, 2);

        switch (choice) {
        case 1:
            {
                auto [username, password] = getLoginInput();
                auto userOpt = loginUser(username, password);

                if (userOpt.has_value()) {
                    User user = userOpt.value();
                    if (user.role == "admin") {
                        adminMenu(user);
                    } else {
                        userMenu(user);
                    }
                }
                break;
            }
        case 2:
            {
                string targetRole;
                cout << "Nhập role cần thêm mới (admin hoặc user): ";
                cin >> targetRole;
                addUser(USER_FILE, targetRole);
                break;
            }
        case 0:
            cout << "Tam biet!\n";
            break;
        default:
            cout << "Lua chon khong hop le. Vui long thu lai.\n";
        }
        cout << endl;
    } while (choice != 0);
}

UserUpdateData inputUserData() {
    UserUpdateData userData;
    int choice;

    do {
        cout << "\n--- CẬP NHẬT THÔNG TIN ---\n";
        cout << "1. Thay đổi mật khẩu\n";
        cout << "2. Thay đổi tên người dùng\n";
        cout << "3. Thay đổi số điện thoại\n";
        cout << "4. Thay đổi địa chỉ\n";
        cout << "5. Thay đổi email\n";
        cout << "6. Thay đổi ngày sinh\n";
        cout << "7. Nạp thêm điểm vào ví\n";
        cout << "8. Thoát cập nhật không thay đổi gì\n";
        cout << "0. Hoàn tất cập nhật\n";

        choice = getIntInput("Chọn mục bạn muốn thay đổi: ", 0, 8);

        switch (choice) {
        case 1:
            cout << "Nhập mật khẩu mới: ";
            getline(cin, userData.password);
            break;
        case 2:
            cout << "Nhập tên mới: ";
            getline(cin, userData.fullName);
            break;
        case 3:
            cout << "Nhập số điện thoại mới: ";
            getline(cin, userData.phone);
            break;
        case 4:
            cout << "Nhập địa chỉ mới: ";
            getline(cin, userData.address);
            break;
        case 5:
            cout << "Nhập email mới: ";
            getline(cin, userData.email);
            break;
        case 6:
            cout << "Nhập ngày sinh mới: ";
            getline(cin, userData.birthday);
            break;
        case 7:
            {
                cout << "Nhập số điểm cần thêm: ";
                string tmp;
                getline(cin, tmp);
                if (!tmp.empty()) {
                    try {
                        userData.balance = stod(tmp);
                    } catch (...) {
                        cout << "Giá trị nhập không hợp lệ. Bỏ qua.\n";
                    }
                }
                break;
            }
        case 8:
            cout << "Hủy bỏ cập nhật thông tin. Quay lại menu.\n";
            return {};
        case 0:
            cout << "Hoàn tất nhập thông tin.\n";
            break;
        default:
            cout << "Lựa chọn không hợp lệ. Vui lòng thử lại.\n";
        }
    } while (choice != 0);

    if (isEmpty(userData)) {
        cout << "Không có thông tin nào được cập nhật.\n";
        return {};
    }

    return userData;
}
