#include <iostream>
#include <vector>
#include <string>
#include "OpenXLSX.hpp"

using namespace std;
using namespace OpenXLSX;

// Hàm tìm dòng dựa theo tên người dùng
int findRowByName(XLWorksheet& sheet, const string& name) {
    for (int row = 2; sheet.cell(row, 1).value().type() != XLValueType::Empty; ++row) {
        if (sheet.cell(row, 2).value().get<string>() == name) {
            return row;
        }
    }
    return -1;
}

// Hàm thực hiện giao dịch nạp điểm
void transferPoints(const string& filePath) {
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("User");

    string adminName = "admin";
    string clientName;
    int points;

    // Xác định dòng của admin và client
    int adminRow = findRowByName(sheet, adminName);
    if (adminRow == -1) {
        cout << "❌ Không tìm thấy admin!\n";
        return;
    }

    char choice;
    do {
        cout << "👉 Nhập tên client cần nạp điểm: ";
        cin >> clientName;

        int clientRow = findRowByName(sheet, clientName);
        if (clientRow == -1) {
            cout << "❌ Không tìm thấy client này!\n";
            continue;
        }

        // Lấy balance hiện tại
        int adminBalance = sheet.cell(adminRow, 7).value().get<int>();
        int clientBalance = sheet.cell(clientRow, 7).value().get<int>();

        // Nhập số điểm muốn nạp
        cout << "💸 Nhập số điểm muốn chuyển: ";
        cin >> points;

        if (points <= 0) {
            cout << "⚠️ Số điểm phải lớn hơn 0!\n";
            continue;
        }

        if (adminBalance < points) {
            cout << "❌ Admin không đủ điểm để thực hiện giao dịch!\n";
            continue;
        }

        // Thực hiện giao dịch
        sheet.cell(adminRow, 7).value() = adminBalance - points;
        sheet.cell(clientRow, 7).value() = clientBalance + points;

        // Lưu trạng thái giao dịch
        string status = "Nạp thành công " + to_string(points) + " điểm cho " + clientName;
        sheet.cell(clientRow, 8).value() = status;

        cout << "✅ Giao dịch thành công! Admin còn " << adminBalance - points << " điểm.\n";
        cout << "📌 Trạng thái: " << status << "\n";

        // Hỏi tiếp tục hay không
        cout << "👉 Bạn có muốn tiếp tục nạp điểm cho client khác không? (y/n): ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    cout << "👋 Kết thúc giao dịch!\n";

    doc.save();
    doc.close();
}


struct User {
    std::string username, password, role;
    int balance, user_id;
    bool is_admin;
};

// Hàm tìm user trong sheet
User findUser(XLWorksheet& sheet, const std::string& username) {
    for (int row = 2; sheet.cell(row, 1).value().type() != XLValueType::Empty; ++row) {
        if (sheet.cell(row, 1).value().get<std::string>() == username) {
            return { username,
                     sheet.cell(row, 2).value().get<std::string>(),
                     sheet.cell(row, 13).value().get<std::string>(),
                     sheet.cell(row, 10).value().get<int>(),
                     row,
                     sheet.cell(row, 11).value().get<int>() == 1 };
        }
    }
    return { "", "", "", 0, 0, false };
}

// Hàm ghi log giao dịch
void logTransaction(XLWorksheet& logSheet, int userId, const std::string& action, int oldValue, int newValue, int senderId, int recipientId) {
    int newRow = 2;
    while (logSheet.cell(newRow, 1).value().type() != XLValueType::Empty) ++newRow;

    logSheet.cell(newRow, 1).value() = newRow - 1;
    logSheet.cell(newRow, 2).value() = userId;
    logSheet.cell(newRow, 3).value() = action;
    logSheet.cell(newRow, 4).value() = oldValue;
    logSheet.cell(newRow, 5).value() = newValue;
    logSheet.cell(newRow, 6).value() = senderId;
    logSheet.cell(newRow, 7).value() = recipientId;
    logSheet.cell(newRow, 8).value() = "23/03/2025";
    logSheet.cell(newRow, 9).value() = "23/03/2025";
}

// Hàm thực hiện giao dịch
void transferPoints(const std::string& filePath) {
    XLDocument doc;
    doc.open(filePath);
    auto userSheet = doc.workbook().worksheet("User");
    auto logSheet = doc.workbook().worksheet("Transaction");

    std::string senderName, recipientName;
    int amount;

    std::cout << "🔧 Nhập tài khoản chuyển: ";
    std::cin >> senderName;
    User sender = findUser(userSheet, senderName);
    if (sender.username.empty() || sender.balance <= 0) {
        std::cout << "❌ Tài khoản không hợp lệ hoặc không đủ điểm!\n";
        return;
    }

    std::cout << "🎯 Nhập tài khoản nhận: ";
    std::cin >> recipientName;
    User recipient = findUser(userSheet, recipientName);
    if (recipient.username.empty()) {
        std::cout << "❌ Tài khoản nhận không tồn tại!\n";
        return;
    }

    std::cout << "💸 Nhập số điểm muốn chuyển: ";
    std::cin >> amount;
    if (amount <= 0 || amount > sender.balance) {
        std::cout << "❌ Số điểm không hợp lệ!\n";
        return;
    }

    // Cập nhật điểm và trạng thái giao dịch
    int oldSenderBalance = sender.balance;
    int oldRecipientBalance = recipient.balance;
    sender.balance -= amount;
    recipient.balance += amount;

    userSheet.cell(sender.user_id, 10).value() = sender.balance;
    userSheet.cell(sender.user_id, 11).value() = "Chuyển tiền thành công";

    userSheet.cell(recipient.user_id, 10).value() = recipient.balance;
    userSheet.cell(recipient.user_id, 11).value() = "Nhận tiền";

    // Ghi log giao dịch cho cả người gửi và người nhận
    logTransaction(logSheet, sender.user_id, "Chuyển tiền thành công", oldSenderBalance, sender.balance, sender.user_id, recipient.user_id);
    logTransaction(logSheet, recipient.user_id, "Nhận tiền", oldRecipientBalance, recipient.balance, sender.user_id, recipient.user_id);

    std::cout << "✅ Giao dịch thành công!\n";

    doc.save();
    doc.close();
}
