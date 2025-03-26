#include <iostream>
#include "login.h"
#include "user.h"
#include "get_user_info.h"
#include "update_user.h"
#include "menu.h"
#include "admin.h"

using namespace std;

void showMainMenu()
{
    cout << "===============================" << endl;
    cout << " CHAO MUNG DEN HE THONG VI DIEM" << endl;
    cout << "===============================" << endl;
    cout << "1. Dang nhap" << endl;
    cout << "2. Dang ky" << endl;
    cout << "0. Thoat" << endl;
    cout << "===============================" << endl;
    cout << "Nhap lua chon cua ban: ";
}

void userMenu(const User &user)
{
    int choice;
    do
    {
        cout << "===============================" << endl;
        cout << " CHAO MUNG DEN HE THONG VI DIEM" << endl;
        cout << "===============================" << endl;
        cout << "1. Kiểm tra thông tin" << endl;
        cout << "2. Cập nhật thông tin" << endl;
        cout << "3. Giao dịch" << endl;
        cout << "0. Thoát" << endl;
        cout << "===============================" << endl;
        cout << "Nhap lua chon cua ban: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            getUserInfo("../data/users.xlsx", user.username);
            break;
        case 2:
            updateUserInfo(user.username, "123456"); // giả định OTP đúng là 123456
            break;
        case 3:
            cout << "💸 Thuc hien giao dich (chua xu ly)\n";
            break;
        case 0:
            cout << "⬅️ Dang xuat khoi tai khoan nguoi dung...\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
            break;
        }
        cout << endl;
    } while (choice != 0);
}
void adminMenu(const User &user)
{
    int choice;
    do
    {
        cout << "=== MENU QUAN LY (Admin) ===\n";
        cout << "1. Xem danh sách người dùng\n";
        cout << "2. Tạo tài khoản mới\n";
        cout << "3. Chỉnh sửa tài khoản\n";
        cout << "0. Đăng xuất\n";
        cout << "Chọn chức năng: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            string filename = "../data/users.xlsx";
            viewUserList(filename);
            break;
        }
        case 2:
            // createNewUser();
            break;
        case 3:
        {
            string targetUser;
            cout << "Nhập username cần cập nhật: ";
            cin >> targetUser;
            updateUserInfo(targetUser, "", true); // adminMode = true
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

pair<string, string> getLoginInput()
{
    string username, password;
    cout << "Nhap ten dang nhap: ";
    cin >> username;
    cout << "Nhap mat khau: ";
    cin >> password;
    return {username, password};
}

void handleMenu()
{
    int choice;
    do
    {
        showMainMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            auto [username, password] = getLoginInput();
            auto userOpt = loginUser(username, password);

            if (userOpt.has_value())
            {
                User user = userOpt.value();
                if (user.role == "admin")
                {
                    adminMenu(user);
                }
                else
                {
                    userMenu(user);
                }
            }
            break;
        }
        case 2:
            cout << "Dang ky (chua xu ly)\n";
            break;
        case 0:
            cout << "Tam biet!\n";
            break;
        default:
            cout << "Lua chon khong hop le. Vui long thu lai.\n";
        }
        cout << endl;

    } while (choice != 0);
}

UserUpdateData inputUserData()
{
    UserUpdateData userData;
    int choice;

    do
    {
        cout << "\n--- CẬP NHẬT THÔNG TIN ---\n";
        cout << "1. Thay đổi mật khẩu\n";
        cout << "2. Thay đổi tên người dùng\n";
        cout << "3. Thay đổi số điện thoại\n";
        cout << "4. Thay đổi địa chỉ\n";
        cout << "5. Thay đổi email\n";
        cout << "6. Thay đổi ngày sinh\n";
        cout << "7. Nạp thêm điểm vào ví\n";
        cout << "0. Hoàn tất cập nhật\n";
        cout << "Chọn mục bạn muốn thay đổi: ";
        cin >> choice;
        cin.ignore(); // Xóa \n trước khi getline

        switch (choice)
        {
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
            if (!tmp.empty())
            {
                try
                {
                    userData.balance = stod(tmp);
                }
                catch (...)
                {
                    cout << "⚠️ Giá trị nhập không hợp lệ. Bỏ qua.\n";
                }
            }
            break;
        }
        case 0:
            cout << " Hoàn tất nhập thông tin.\n";
            break;
        default:
            cout << " Lựa chọn không hợp lệ. Vui lòng thử lại.\n";
        }
    } while (choice != 0);

    return userData;
}