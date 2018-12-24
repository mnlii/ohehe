cmake_minimum_required(VERSION 3.13)
project(Bookstore)

set(CMAKE_CXX_STANDARD 14)

add_executable(Bookstore main.cpp DB/BLL.cpp DB/BLL.h DB/User.cpp DB/User.h DB/Book.cpp DB/Book.h DB/Refer.cpp DB/Refer.h Reporter/Reporter.cpp Reporter/Reporter.h Reporter/Finance.cpp Reporter/Finance.h Reporter/Command.cpp Reporter/Command.h Controller/UserController.cpp Controller/UserController.h Controller/BookController.cpp Controller/BookController.h CommandReader.cpp CommandReader.h basic_function.h)