#include "admin.h"
#include <iostream>
#include <OpenXLSX.hpp>

using namespace std;
using namespace OpenXLSX;

void viewUserList(const string &filename)
{
    try
    {
        XLDocument doc;
        doc.open(filename);
        auto wks = doc.workbook().worksheet("Sheet1");

        cout << "========== DANH SÁCH NGƯỜI DÙNG (ROLE: user) ==========\n";

        int row = 2;
        while (true)
        {
            auto usernameCell = wks.cell("B" + to_string(row));
            if (usernameCell.value().type() == XLValueType::Empty)
                break;

            string username = usernameCell.value().get<string>();

            string fullName = "";
            if (wks.cell("D" + to_string(row)).value().type() == XLValueType::String)
                fullName = wks.cell("D" + to_string(row)).value().get<string>();

            string email = "";
            if (wks.cell("G" + to_string(row)).value().type() == XLValueType::String)
                email = wks.cell("G" + to_string(row)).value().get<string>();

            string role = "";
            if (wks.cell("O" + to_string(row)).value().type() == XLValueType::String)
                role = wks.cell("O" + to_string(row)).value().get<string>();

            double balance = 0.0;
            auto balanceCell = wks.cell("K" + to_string(row));
            if (balanceCell.value().type() == XLValueType::Float || balanceCell.value().type() == XLValueType::Integer)
            {
                balance = balanceCell.value().get<double>();
            }

            if (role == "user")
            {
                cout << "Tài khoản: " << username << "\n";
                cout << "Họ tên:    " << fullName << "\n";
                cout << "Email:     " << email << "\n";
                cout << "Số dư:     " << balance << " điểm\n";
                cout << "-----------------------------------------\n";
            }

            row++;
        }

        doc.close();
    }
    catch (const exception &e)
    {
        cerr << "Lỗi khi đọc danh sách người dùng: " << e.what() << endl;
    }
}
