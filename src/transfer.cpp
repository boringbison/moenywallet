#include "transfer.h"
#include "OTPManager.h"
#include <ctime>

// Hàm lấy thời gian hiện tại
string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

// Hàm tìm user trong sheet
User findUser(XLWorksheet& sheet, const std::string& username) {
    for (int row = 2; sheet.cell(row, 1).value().type() != XLValueType::Empty; ++row) {
        if (sheet.cell(row, 2).value().get<std::string>() == username) {
            return { 
                username,
                sheet.cell(row, 3).value().type() == XLValueType::String ? 
                    sheet.cell(row, 3).value().get<std::string>() : "",  // Nếu rỗng, gán ""
                sheet.cell(row, 14).value().type() == XLValueType::String ? 
                    sheet.cell(row, 14).value().get<std::string>() : "",
                sheet.cell(row, 11).value().type() == XLValueType::Integer ? 
                    sheet.cell(row, 11).value().get<int>() : 0,
                row,
                sheet.cell(row, 12).value().type() == XLValueType::Integer ? 
                    (sheet.cell(row, 12).value().get<int>() == 1) : false
            };
        }
    }
    return { "", "", "", 0, 0, false };
}

User findAdmin(XLWorksheet &sheet)
{
    for (int row = 2; sheet.cell(row, 1).value().type() != XLValueType::Empty; ++row)
    {
        if (sheet.cell(row, 13).value().type() == XLValueType::Integer &&
            sheet.cell(row, 13).value().get<int>() == 1)
        {

            return {
                sheet.cell(row, 2).value().get<std::string>(), // Username
                sheet.cell(row, 3).value().type() == XLValueType::String ? sheet.cell(row, 3).value().get<std::string>() : "",
                sheet.cell(row, 14).value().type() == XLValueType::String ? sheet.cell(row, 14).value().get<std::string>() : "",
                sheet.cell(row, 11).value().type() == XLValueType::Integer ? sheet.cell(row, 11).value().get<int>() : 0,
                row,
                true // is_admin = true
            };
        }
    }
    return {"", "", "", 0, 0, false}; // Không tìm thấy
}

// Hàm ghi log giao dịch
void logTransaction(XLWorksheet& logSheet, int userId, const std::string& action, 
                    int oldValue, int newValue, int senderId, int recipientId) {
    if (logSheet.rowCount() == 0) {  // Kiểm tra xem sheet có dữ liệu không
        cout << "⚠️ Lỗi: Sheet log giao dịch không hợp lệ hoặc trống!" << endl;
        return;
    }

    // Tìm hàng trống đầu tiên (kiểm tra nếu cột 1 trống)
    int newRow = 2;
    while (logSheet.cell(newRow, 1).value().type() != XLValueType::Empty) {
        ++newRow;
    }

    string currentTime = getCurrentTime();

    logSheet.cell(newRow, 1).value() = newRow - 1;  // ID giao dịch
    logSheet.cell(newRow, 2).value() = userId;
    logSheet.cell(newRow, 3).value() = action;
    logSheet.cell(newRow, 4).value() = oldValue;
    logSheet.cell(newRow, 5).value() = newValue;
    logSheet.cell(newRow, 6).value() = senderId;
    logSheet.cell(newRow, 7).value() = recipientId;
    logSheet.cell(newRow, 8).value() = currentTime;
}



// Hàm thực hiện giao dịch
void transferPoints(const std::string& filePathUser, const std::string& filePathLog,  const std::string& userNameSender) {
    XLDocument docUser;
    docUser.open(filePathUser);
     XLDocument docLog;
    docLog.open(filePathLog);
    auto userSheet = docUser.workbook().worksheet("Sheet1");
    auto logSheet = docLog.workbook().worksheet("Sheet1");


bool hasAdmin = false;
for (int row = 2; row <= userSheet.rowCount(); ++row) { // Bắt đầu từ hàng 2 (bỏ qua tiêu đề)
    if (userSheet.cell(row, 13).value().get<int>() == 1) { // Cột M = index 13
        hasAdmin = true;
        break;
    }
}

if (!hasAdmin) {
    cout << "❌ Không tìm thấy tài khoản admin nào!\n";
    return;
}

User admin = findAdmin(userSheet);
if (admin.username.empty())
{
    cout << "❌ Không tìm thấy tài khoản admin!\n";
    return;
}

    string senderName, recipientName;
    int amount;


    User sender = findUser(userSheet, userNameSender);
    if (sender.username.empty()) {
        cout << "❌ Tài khoản gửi không hợp lệ!\n";
        return;
    }

    cout << "🎯 Nhập tài khoản nhận: ";
    cin >> recipientName;
    User recipient = findUser(userSheet, recipientName);
    if (recipient.username.empty()) {
        cout << "❌ Tài khoản nhận không hợp lệ!\n";
        return;
    }

    cout << "💸 Nhập số điểm muốn chuyển: ";
    cin >> amount;
    if (amount <= 0) {
        cout << "❌ Số điểm không hợp lệ!\n";
        return;
    }

    // Nếu người gửi là client và muốn chuyển nhiều hơn số tiền của admin → không cho phép
    if (!sender.is_admin && amount > admin.balance) {
        cout << "❌ Số điểm cần chuyển lớn hơn số dư của admin. Giao dịch bị từ chối!\n";
        return;
    }

    // Nếu người gửi không đủ tiền
    if (amount > sender.balance) {
        cout << "❌ Tài khoản không đủ điểm!\n";
        return;
    }

    // Cập nhật số dư
    int oldSenderBalance = sender.balance;
    int oldRecipientBalance = recipient.balance;
    sender.balance -= amount;
    recipient.balance += amount;

    userSheet.cell(sender.user_id, 11).value() = sender.balance;
    userSheet.cell(sender.user_id, 12).value() = "Chuyển tiền thành công";

    userSheet.cell(recipient.user_id, 11).value() = recipient.balance;
    userSheet.cell(recipient.user_id, 12).value() = "Nhận tiền";

    // Ghi log giao dịch
    logTransaction(logSheet, sender.user_id, "Chuyển tiền thành công", oldSenderBalance, sender.balance, sender.user_id, recipient.user_id);
    logTransaction(logSheet, recipient.user_id, "Nhận tiền", oldRecipientBalance, recipient.balance, sender.user_id, recipient.user_id);

    cout << "✅ Giao dịch thành công!\n";

    docUser.save();
    docUser.close();

    docLog.save();
    docLog.close();
}
