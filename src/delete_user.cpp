#include <iostream>
#include <OpenXLSX.hpp>
#include <vector>
#include <string>
#include "user.h"

using namespace OpenXLSX;
using namespace std;

void deleteUser(const string& filePath, const string& targetUser, bool isAdmin) {
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("User");
    bool found = false;

    for (int row = 2; sheet.cell(row, 2).value().type() != XLValueType::Empty; ++row) {
        if (sheet.cell(row, 2).value().get<string>() == targetUser) {
            string role = sheet.cell(row, 4).value().get<string>();
            if (!isAdmin && role == "admin") {
                cout << "Bam khong the xoa tai khoan admin!" << endl;
                return;
            }

            found = true;
            cout << "da xoa: " << targetUser << endl;
            for (int col = 1; col <= 4; ++col) {
                sheet.cell(row, col).value().clear();
            }
            break;
        }
    }
    if (!found) cout << "Khong tim thay!" << endl;

    doc.save();
    doc.close();
}