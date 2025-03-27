cmake_minimum_required(VERSION 3.10)

# Định danh project
project(qt_project)

# Định nghĩa đường dẫn thư mục source
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Tìm tất cả file .cpp trong src
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)

# Tạo executable từ tất cả các file trong src
add_executable(qt_app ${SRC_FILES})

# Thêm thư viện OpenXLSX
add_subdirectory(${CMAKE_SOURCE_DIR}/OpenXLSX)

# Thêm đường dẫn include
include_directories(${CMAKE_SOURCE_DIR}/OpenXLSX/OpenXLSX ${SRC_DIR})

# Link thư viện vào project
target_link_libraries(qt_app PRIVATE OpenXLSX)
