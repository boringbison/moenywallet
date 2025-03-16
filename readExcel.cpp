#include <iostream>
#include "OpenXLSX/OpenXLSX/OpenXLSX.hpp"

using namespace OpenXLSX;
using namespace std;

void readAndWriteExcel(const string& filePath) {
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("User");

    cout << "Danh sách người dùng:\n";
    for (int row = 1; sheet.cell(row, 2).value().type() != XLValueType::Empty; ++row) {
        string username = sheet.cell(row, 2).value().get<string>(); // Cột 2 (Username)
        string email = sheet.cell(row, 3).value().get<string>();    // Cột 3 (Email)

        // Ghi giá trị mới vào cột 6
        sheet.cell(row, 6).value() = "Đã xử lý";

        cout << "User: " << username << " | Email: " << email << " | Cột 6: Đã xử lý" << endl;
    }

    doc.save(); // Lưu file sau khi sửa đổi
    doc.close();
}


void findByName(const string& filePath, const string& target) {
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("User");

    bool found = false;

    // Duyệt từng hàng
    for (int row = 1; sheet.cell(row, 1).value().type() != XLValueType::Empty; ++row) {
        bool hasValue = false;

        // Duyệt từng ô trong hàng
        for (int col = 1;; ++col) {
            auto cell = sheet.cell(row, col);

            // Dừng khi gặp ô trống trong cột
            if (cell.value().type() == XLValueType::Empty) {
                break;
            }

            // Chuyển đổi giá trị trong ô thành chuỗi
            string cellValue;
            switch (cell.value().type()) {
                case XLValueType::String:
                    cellValue = cell.value().get<string>();
                    break;
                case XLValueType::Integer:
                    cellValue = to_string(cell.value().get<int>());
                    break;
                case XLValueType::Float:
                    cellValue = to_string(cell.value().get<double>());
                    break;
                case XLValueType::Boolean:
                    cellValue = cell.value().get<bool>() ? "true" : "false";
                    break;
                default:
                    cellValue = "";
                    break;
            }

            if (!cellValue.empty()) {
                hasValue = true;
            }

            // So sánh giá trị cần tìm
            if (cellValue == target) {
                found = true;
                cout << "✅ Tìm thấy ở dòng " << row << ": ";

                // In ra toàn bộ hàng
                for (int c = 1;; ++c) {
                    auto cellContent = sheet.cell(row, c);
                    if (cellContent.value().type() == XLValueType::Empty) break;
                    string contentValue;

                    // Chuyển đổi ô trong dòng thành chuỗi
                    switch (cellContent.value().type()) {
                        case XLValueType::String:
                            contentValue = cellContent.value().get<string>();
                            break;
                        case XLValueType::Integer:
                            contentValue = to_string(cellContent.value().get<int>());
                            break;
                        case XLValueType::Float:
                            contentValue = to_string(cellContent.value().get<double>());
                            break;
                        case XLValueType::Boolean:
                            contentValue = cellContent.value().get<bool>() ? "true" : "false";
                            break;
                        default:
                            contentValue = "";
                            break;
                    }

                    cout << contentValue << " | ";
                }

                cout << endl;
                break;  // Dừng kiểm tra tiếp dòng này
            }
        }

        if (!hasValue) continue;  // Bỏ qua hàng trống hoàn toàn
    }

    if (!found) {
        cout << "❌ Không tìm thấy giá trị '" << target << "' trong file!" << endl;
    }

    doc.close();
}


void updateById(const std::string& filePath, int targetId) {
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("User");

    bool found = false;

    // Đọc hàng đầu tiên để lấy tên cột
    std::vector<std::string> headers;
    for (int col = 1; ; ++col) {
        auto cell = sheet.cell(1, col);
        if (cell.value().type() == XLValueType::Empty) break;  // Dừng khi gặp cột trống
        headers.push_back(cell.value().get<std::string>());
    }

    // Tìm dòng có ID khớp
    for (int row = 2; sheet.cell(row, 1).value().type() != XLValueType::Empty; ++row) {
        int id = sheet.cell(row, 1).value().get<int>();

        if (id == targetId) {
            found = true;
            std::cout << "🔧 Nhập thông tin cập nhật:\n";

            // Lặp qua từng cột (bỏ qua cột ID)
            for (int col = 1; col < headers.size(); ++col) {
                std::string newValue;
                std::cout << "Nhập 'update" << headers[col] << "': ";
                std::cin >> newValue;

                // Cập nhật giá trị vào ô tương ứng
                sheet.cell(row, col + 1).value() = newValue;
            }

            std::cout << "✅ Đã cập nhật thông tin thành công!\n";
            break;
        }
    }

    if (!found) {
        std::cout << "❌ Không tìm thấy người dùng với ID: " << targetId << std::endl;
    }

    doc.save();
    doc.close();
}


// int main() {
//     readAndWriteExcel("F:/qt/file.xlsx");
//     return 0;
// }


void deleteRowByName(const string& filePath, const string& target) {
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("User");

    bool found = false;

    // Duyệt từng hàng
    for (int row = 1; sheet.cell(row, 1).value().type() != XLValueType::Empty; ++row) {
        // Duyệt từng ô trong hàng
        for (int col = 1;; ++col) {
            auto cell = sheet.cell(row, col);

            // Dừng nếu gặp ô trống
            if (cell.value().type() == XLValueType::Empty) break;

            // Chuyển giá trị trong ô thành chuỗi
            string cellValue;
            switch (cell.value().type()) {
                case XLValueType::String:
                    cellValue = cell.value().get<string>();
                    break;
                case XLValueType::Integer:
                    cellValue = to_string(cell.value().get<int>());
                    break;
                case XLValueType::Float:
                    cellValue = to_string(cell.value().get<double>());
                    break;
                case XLValueType::Boolean:
                    cellValue = cell.value().get<bool>() ? "true" : "false";
                    break;
                default:
                    cellValue = "";
                    break;
            }

            // So sánh giá trị cần tìm
            if (cellValue == target) {
                found = true;

                // Xóa nguyên hàng
                cout << "🗑️ Đã xóa dòng " << row << " chứa giá trị: " << target << endl;
                for (int c = 1; sheet.cell(row, c).value().type() != XLValueType::Empty; ++c) {
                    sheet.cell(row, c).value().clear();
                }

                // Lùi các dòng phía dưới lên
                int nextRow = row + 1;
                while (sheet.cell(nextRow, 1).value().type() != XLValueType::Empty) {
                    for (int c = 1;; ++c) {
                        auto nextCell = sheet.cell(nextRow, c);
                        if (nextCell.value().type() == XLValueType::Empty) break;

                        // Di chuyển dữ liệu lên hàng trên
                        sheet.cell(row, c).value() = nextCell.value();
                        nextCell.value().clear();
                    }
                    row++;  // Cập nhật vị trí hàng vừa lùi lên
                    nextRow++;
                }

                doc.save();
                doc.close();
                return;  // Kết thúc ngay sau khi xóa
            }
        }
    }

    if (!found) {
        cout << "❌ Không tìm thấy giá trị '" << target << "' trong file!" << endl;
    }

    doc.close();
}


void addRowAfterLast(const std::string& filePath) {
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("User");

    std::vector<std::string> headers;

    // Lấy tiêu đề từ dòng 1
    for (int col = 1;; ++col) {
        auto cell = sheet.cell(1, col);
        if (cell.value().type() == XLValueType::Empty) break;
        headers.push_back(cell.value().get<std::string>());
    }

    char choice;
    do {
        // Xác định dòng cuối cùng có dữ liệu
        int newRow = 2;
        while (sheet.cell(newRow, 1).value().type() != XLValueType::Empty) {
            ++newRow;
        }

        std::cout << "🔧 Nhập thông tin cho dòng mới (hàng " << newRow << "):\n";

        // Nhập dữ liệu theo từng cột
        for (int col = 0; col < headers.size(); ++col) {
            std::string newValue;
            std::cout << "Nhập giá trị cho '" << headers[col] << "': ";
            std::cin >> newValue;

            sheet.cell(newRow, col + 1).value() = newValue;
        }

        std::cout << "✅ Đã thêm dòng mới thành công tại dòng " << newRow << "!\n";

        // Hỏi tiếp tục hay không
        std::cout << "👉 Bạn có muốn thêm dòng mới nữa không? (y/n): ";
        std::cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    std::cout << "👋 Kết thúc thêm dòng!\n";

    doc.save();
    doc.close();
}
