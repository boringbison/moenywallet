#include "update_user.h"
#include "menu.h"
#include <iostream>
#include <OpenXLSX.hpp>
#include "validate.h"

using namespace std;
using namespace OpenXLSX;

void updateUserInfo(const std::string &username, const std::string &expectedOtp, bool adminMode)
{
    string otpInput;
    cout << "=== Cập nhật thông tin người dùng ===\n";

    UserUpdateData data = inputUserData();

    if (!adminMode)
    {
        cout << "Yêu cầu nhập mã OTP để xác nhận thay đổi: ";
        getline(cin, otpInput);
        if (otpInput != expectedOtp)
        {
            cout << "❌ OTP không hợp lệ. Hủy cập nhật.\n";
            return;
        }
    }
    else
    {
        cout << "⚠️ Đang cập nhật với quyền quản lý (bỏ qua OTP)...\n";
    }

    try
    {
        XLDocument doc;
        doc.open("../data/users.xlsx");
        auto wks = doc.workbook().worksheet("Sheet1");
        bool userFound = false;

        for (uint32_t row = 2; row <= wks.rowCount(); ++row)
        {
            string uname = wks.cell("B" + to_string(row)).value().get<string>();
            if (uname == username)
            {
                userFound = true;

                if (!data.password.empty())
                {
                    if (!isValidPassword(data.password))
                    {
                        cout << "❌ Mật khẩu không hợp lệ( mật khẩu phải từ 6 ký tự). Hủy cập nhật.\n";
                        return;
                    }
                    wks.cell("C" + to_string(row)).value() = data.password;
                }

                if (!data.fullName.empty())
                {
                    if (!isValidFullName(data.fullName))
                    {
                        cout << "❌ Tên người dùng có ký tự không hợp lệ. Hủy cập nhật.\n";
                        return;
                    }
                    wks.cell("D" + to_string(row)).value() = data.fullName;
                }

                if (!data.phone.empty())
                {
                    if (!isValidPhone(data.phone))
                    {
                        cout << "❌ Số điện thoại người dùng không hợp lệ. Hủy cập nhật.\n";
                        return;
                    }
                    wks.cell("E" + to_string(row)).value() = data.phone;
                }

                if (!data.address.empty())
                    wks.cell("F" + to_string(row)).value() = data.address;

                if (!data.email.empty())
                {
                    if (!isValidEmail(data.email))
                    {
                        cout << "❌ Email người dùng không hợp lệ. Hủy cập nhật.\n";
                        return;
                    }
                    wks.cell("G" + to_string(row)).value() = data.email;
                }

                if (!data.birthday.empty())
                {
                    if (!isValidBirthday(data.birthday))
                    {
                        cout << "❌ Ngày sinh người dùng không hợp lệ. Hủy cập nhật.\n";
                        return;
                    }
                    wks.cell("H" + to_string(row)).value() = data.birthday;
                }

                // ✅ Cộng thêm balance nếu có
                if (data.balance.has_value())
                {
                    auto balanceCell = wks.cell("K" + to_string(row));
                    double currentBalance = 0.0;

                    if (balanceCell.value().type() != XLValueType::Empty)
                    {
                        try
                        {
                            currentBalance = balanceCell.value().get<double>();
                        }
                        catch (...)
                        {
                            try
                            {
                                currentBalance = stod(balanceCell.value().get<std::string>());
                            }
                            catch (...)
                            {
                                currentBalance = 0.0;
                            }
                        }
                    }

                    double newBalance = currentBalance + data.balance.value();
                    balanceCell.value() = newBalance;
                    cout << "✅ Đã nạp thêm " << data.balance.value() << " điểm.\n";
                }

                cout << "✅ Cập nhật thông tin thành công!\n";
                break;
            }
        }

        if (!userFound)
        {
            cout << "❌ Không tìm thấy người dùng trong hệ thống.\n";
        }

        doc.save();
        doc.close();
    }
    catch (const exception &e)
    {
        cout << "⚠️ Lỗi khi cập nhật file Excel: " << e.what() << endl;
    }
}
