#include "get_user_info.h"
#include <iostream>
#include <OpenXLSX.hpp>
#include <iomanip>

using namespace OpenXLSX;
using namespace std;

// Trả về giá trị của ô dưới dạng chuỗi an toàn
string getCellAsString(XLWorksheet &wks, int row, int col)
{
    const auto &val = wks.cell(row, col).value();
    switch (val.type())
    {
    case XLValueType::String:
        return val.get<string>();
    case XLValueType::Float:
        return to_string(val.get<double>());
    case XLValueType::Integer:
        return to_string(val.get<int64_t>());
    case XLValueType::Boolean:
        return val.get<bool>() ? "true" : "false";
    case XLValueType::Empty:
    default:
        return "";
    }
}

void getUserInfo(const string &filePath, const string &username)
{
    try
    {
        XLDocument doc;
        doc.open(filePath);
        auto wks = doc.workbook().worksheet("Sheet1");

        int row = 2;
        bool found = false;

        while (wks.cell(row, 2).value().type() != XLValueType::Empty)
        {
            // Đọc giá trị username từ cột 2 (cột B)
            string currentUsername = getCellAsString(wks, row, 2);
            if (currentUsername == username)
            {
                found = true;
                break;
            }
            row++;
        }

        if (!found)
        {
            cout << " Không tìm thấy người dùng: " << username << endl;
            doc.close();
            return;
        }

        // In thông tin đầy đủ
        cout << " Thông tin người dùng:\n";
        cout << "ID               : " << getCellAsString(wks, row, 1) << endl;
        cout << "Username         : " << getCellAsString(wks, row, 2) << endl;
        cout << "Password         : " << getCellAsString(wks, row, 3) << endl;
        cout << "Họ và tên        : " << getCellAsString(wks, row, 4) << endl;
        cout << "SĐT              : " << getCellAsString(wks, row, 5) << endl;
        cout << "Địa chỉ          : " << getCellAsString(wks, row, 6) << endl;
        cout << "Email            : " << getCellAsString(wks, row, 7) << endl;
        cout << "Ngày sinh        : " << getCellAsString(wks, row, 8) << endl;
        double balance = wks.cell(row, 11).value().get<double>();
        cout << "Số dư (Balance)  : " << fixed << setprecision(2) << balance << endl;

        doc.close();
    }
    catch (const exception &e)
    {
        cerr << "Lỗi khi đọc thông tin người dùng: " << e.what() << endl;
    }
}
