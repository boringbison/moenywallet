#include <iostream>
#include <OpenXLSX.hpp>
#include <string>
#include "user.h"

using namespace OpenXLSX;
using namespace std;

#define COL_USERNAME 2
#define COL_ROLE 15
#define COL_IS_ACTIVE 14 // sửa lại theo đúng cột trong users.xlsx nếu khác

void deleteUser(const string &filePath, const string &targetUser, bool isAdmin)
{
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("Sheet1");

    bool found = false;

    for (int row = 2; sheet.cell(row, COL_USERNAME).value().type() != XLValueType::Empty; ++row)
    {
        auto usernameCell = sheet.cell(row, COL_USERNAME);
        if (usernameCell.value().type() == XLValueType::String &&
            usernameCell.value().get<string>() == targetUser)
        {
            auto roleCell = sheet.cell(row, COL_ROLE);
            string role = (roleCell.value().type() == XLValueType::String) ? roleCell.value().get<string>() : "";

            if (!isAdmin && role == "admin")
            {
                cout << "Bạn không thể vô hiệu hóa tài khoản admin!" << endl;
                doc.close();
                return;
            }

            sheet.cell(row, COL_IS_ACTIVE).value() = 0;
            cout << "Tài khoản " << targetUser << " đã bị vô hiệu hóa." << endl;

            found = true;
            break;
        }
    }

    if (!found)
        cout << "Không tìm thấy tài khoản cần xử lý!" << endl;

    doc.save();
    doc.close();
}
