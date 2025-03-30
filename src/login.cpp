#include "login.h"
#include "user.h"
#include <OpenXLSX.hpp>
#include <iostream>
#include <optional>
#include <algorithm>

using namespace std;
using namespace OpenXLSX;

std::optional<User> loginUser(const std::string &username, const std::string &password)
{
    try
    {
        XLDocument doc;
        doc.open("../data/users.xlsx");
        auto wks = doc.workbook().worksheet("Sheet1");

        int row = 2;
        while (true)
        {
            auto &userCell = wks.cell("B" + to_string(row)).value();
            auto &passCell = wks.cell("C" + to_string(row)).value();
            auto &roleCell = wks.cell("O" + to_string(row)).value();    // role
            auto &balanceCell = wks.cell("K" + to_string(row)).value(); // balance
            auto &idCell = wks.cell("A" + to_string(row)).value();      // user_id
            auto &isAdminCell = wks.cell("M" + to_string(row)).value(); // is_admin

            if (userCell.type() == XLValueType::Empty)
                break;

            string storedUsername = (userCell.type() == XLValueType::String)
                                        ? userCell.get<string>()
                                        : to_string(userCell.get<int>());

            string storedPassword = (passCell.type() == XLValueType::String)
                                        ? passCell.get<string>()
                                        : to_string(passCell.get<int>());

            string storedRole = (roleCell.type() == XLValueType::String)
                                    ? roleCell.get<string>()
                                    : "user";

            std::transform(storedRole.begin(), storedRole.end(), storedRole.begin(), ::tolower);
            storedRole.erase(remove_if(storedRole.begin(), storedRole.end(), ::isspace), storedRole.end());

            if (storedUsername == username && storedPassword == password)
            {
                User user;
                user.username = storedUsername;
                user.password = storedPassword;
                user.role = storedRole;

                // Gán thêm thông tin bổ sung (nếu cần sử dụng sau này)
                user.user_id = (idCell.type() == XLValueType::Integer) ? idCell.get<int>() : 0;

                // Xử lý balance (có thể là string chứa dấu phẩy)
                try
                {
                    if (balanceCell.type() == XLValueType::Float)
                        user.balance = static_cast<int>(balanceCell.get<double>());
                    else
                        user.balance = stoi(balanceCell.get<string>());
                }
                catch (...)
                {
                    user.balance = 0;
                }

                // Xử lý is_admin (0 hoặc 1)
                user.is_admin = (isAdminCell.type() == XLValueType::Integer) ? isAdminCell.get<int>() == 1 : false;

                doc.close();
                cout << " >>> Đăng nhập thành công! Role: " << user.role << ", ID: " << user.user_id << "\n";
                return user;
            }

            row++;
        }

        doc.close();
        cout << "Tên đăng nhập hoặc mật khẩu không đúng.\n";
        return std::nullopt;
    }
    catch (const exception &e)
    {
        cerr << "Lỗi khi mở file Excel: " << e.what() << endl;
        return std::nullopt;
    }
}
