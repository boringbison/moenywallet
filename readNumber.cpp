#include <iostream>
using namespace std;

int readNumber() {  // Đổi void thành int nếu muốn trả về số
    int number;

    cout << "Nhập một số: ";
    cin >> number;

    cout << "Số bạn vừa nhập là: " << number << endl;

    return number;  // Trả về số vừa nhập
}