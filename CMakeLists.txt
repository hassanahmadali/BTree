cmake_minimum_required(VERSION 3.22)
project(BTree)

set(CMAKE_CXX_STANDARD 17)

add_executable(BTree main.cpp "Btree.h" "Btree.cpp" "BTreeNode.h" "BTreeNode.cpp")
