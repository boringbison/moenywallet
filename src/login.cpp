#include "login.h"
#include "user.h"
#include <OpenXLSX.hpp>
#include <iostream>
#include <optional>

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
            auto &roleCell = wks.cell("O" + to_string(row)).value(); // Cột Role

            if (userCell.type() == XLValueType::Empty)
                break;

            string storedUsername, storedPassword, storedRole;

            // Username
            storedUsername = (userCell.type() == XLValueType::String)
                                 ? userCell.get<string>()
                                 : to_string(userCell.get<int>());

            // Password
            storedPassword = (passCell.type() == XLValueType::String)
                                 ? passCell.get<string>()
                                 : to_string(passCell.get<int>());

            // Role
            if (roleCell.type() == XLValueType::String)
                storedRole = roleCell.get<string>();
            else
                storedRole = "user"; // mặc định

            if (storedUsername == username && storedPassword == password)
            {
                cout << "✅ Dang nhap thanh cong!\n";
                doc.close();
                return User{storedUsername, storedRole};
            }

            row++;
        }

        doc.close();
        cout << "❌ Ten dang nhap hoac mat khau khong dung.\n";
        return std::nullopt;
    }
    catch (const exception &e)
    {
        cerr << "🚫 Loi khi mo file Excel: " << e.what() << endl;
        return std::nullopt;
    }
}
