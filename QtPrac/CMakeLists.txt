cmake_minimum_required(VERSION 3.16)
project(QtPrac LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 开启 Qt 元对象编译器 (moc) 自动生成
set(CMAKE_AUTOMOC ON)

# 查找刚才安装的 Qt6 核心库 (Qt6Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(signals_slots_demo main.cpp)

# 链接 Qt6 核心库
target_link_libraries(signals_slots_demo PRIVATE Qt6::Core)
