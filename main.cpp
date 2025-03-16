#include <iostream>
#include <string>

// int readNumber(); // Khai báo hàm từ file khác
// void findByName(const std::string& filePath);

// Khai báo hàm
// int readNumber();
// void readAndWriteExcel(const std::string& filePath);
void findByName(const std::string& filePath, const std::string& searchValue);


void deleteRowByName(const std::string& filePath, const std::string& target);


void updateById(const std::string& filePath, int targetId);

void addRowAfterLast(const std::string& filePath);




using namespace std;

int main() {
    std::cout << "Chương trình chính bắt đầu!\n";

    string filePath = "F:/QT/file.xlsx";
    string searchValue;

    cout << "Nhập giá trị cần tìm: ";
    cin >> searchValue;

    // Gọi hàm tìm kiếm
    // findByName(filePath, searchValue);

    // Gọi hàm xóa hàng theo tên
    // deleteRowByName(filePath, searchValue);

    // Gọi hàm cập nhập theo id
    // updateById(filePath,1);

    // Gọi hàm thêm dòng mới
    // addRowAfterLast(filePath);

    return 0;
}