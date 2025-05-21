💰 MoenyWallet
MoenyWallet là một ứng dụng quản lý ví cá nhân được phát triển bằng ngôn ngữ C++. Dự án sử dụng thư viện OpenXLSX để lưu trữ và thao tác dữ liệu người dùng trong các file Excel.

👥 Thành viên dự án & Phân công công việc
Họ tên Công việc đảm nhận
Hoàn Phân tích, phát triển hệ thống, tạo mã otp, fix bug
Đức Anh Tạo tài khoản, xóa tài khoản
Nguyên Đăng nhập, đăng xuất, phân luồng, cập nhật thông tin tài khoản, theo dõi thông tin tài khoản
Quang Xử lý các chức năng liên quan đến ví (nạp, chuyển, nhận điểm, ghi nhận giao dịch)

🧩 Phân tích & Đặc tả chức năng

1. Tài khoản
   Tạo tài khoản:

Admin: Khi tạo mới, mật khẩu sẽ được sinh ngẫu nhiên (role: admin).

Client: Khi tạo mới, người dùng tự đặt mật khẩu (role: user).

Cập nhật thông tin tài khoản:

Admin: Có quyền cập nhật mọi tài khoản.

Client: Chỉ cập nhật được tài khoản của chính mình. Nếu tài khoản do admin tạo, bắt buộc đổi mật khẩu dựa vào trường isAdmin.

Xóa tài khoản:

Admin: Xóa được tất cả tài khoản trừ tài khoản admin.

Client: Chỉ xóa được tài khoản của chính mình.

2. Ví điện tử
   Nạp, chuyển, nhận điểm:

Admin: Khi client nạp điểm, số dư admin giảm, số dư client tăng (ghi trạng thái vào transaction_status).

Client: Khi nạp/chuyển điểm, số dư thay đổi tương ứng (ghi trạng thái vào transaction_status).

Theo dõi lịch sử giao dịch và số dư:

Khi chuyển tiền, lưu giá trị cũ (old_value), giá trị mới (new_value), trạng thái (action: chuyển thành công/thất bại), id người gửi và nhận vào file Excel.

Khi nhận tiền, lưu tương tự, action là "Nhận tiền".

🗂 Cấu trúc thư mục
text
src/
├── add_user.cpp / .h # Thêm người dùng
├── delete_user.cpp / .h # Xóa người dùng
├── get_user_info.cpp / .h # Lấy thông tin người dùng
├── login.cpp / .h # Đăng nhập
├── transfer.cpp / .h # Chuyển tiền
├── update_user.cpp / .h # Cập nhật người dùng
├── validate.cpp / .h # Kiểm tra hợp lệ
├── main.cpp # Chương trình chính
├── menu.cpp / .h # Menu CLI
└── user.h # Struct định nghĩa User

data/
├── users.xlsx # Thông tin người dùng
└── logs.xlsx # Lịch sử giao dịch

⚙️ Cài đặt

1. Yêu cầu hệ thống
   Trình biên dịch C++ (GCC / MSVC / Clang)

CMake

Thư viện OpenXLSX

2. Tải và build dự án
   bash
   git clone https://github.com/boringbison/moenywallet.git
   cd moenywallet
   mkdir build && cd build
   cmake ..
   make

3. Chạy chương trình

Cách 1:
bash
./main
Cách 2:
cd build
cmake --build .
moneywallet.exe

🚀 Hướng dẫn sử dụng
Khi chạy chương trình, bạn sẽ thấy menu giao diện dòng lệnh (CLI).

Chọn các chức năng như: tạo tài khoản, đăng nhập, chuyển tiền, xem lịch sử giao dịch, v.v.

Dữ liệu sẽ được lưu vào file Excel trong thư mục data/.
