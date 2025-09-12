#include <iostream>


int main(int argc, char* argv[])
{
    {
        std::cout << "101 pointer" << std::endl;
        /// 指针类型 p1变量存在栈中
        /// new int 堆中
        int* p1 = new int;
        ///  *间接符号
        *p1            = 101;
        int        i   = 10;
        int& ref = i; /// 创建对 x 的 const 引用
        std::cout << "i = " << i << std::endl;
        int* p2 = &i;
        *p2     = 102;
        std::cout << "i = " << i << std::endl;
        std::cout << "p1 = " << p1 << std::endl;
        std::cout << "*p1 = " << *p1 << std::endl;
        std::cout << "sizeof(p1) = " << sizeof(p1) << std::endl;
        std::cout << "sizeof(*p1) = " << sizeof(*p1) << std::endl;
        std::cout << "sizeof(int&) = " << sizeof(ref) << std::endl;
        delete p1;
        std::cout << "after delete p1 = " << p1 << std::endl; /// 此时地址 也是野的
        if (p1)
        {
            std::cout << "p1 not empty?" << std::endl;
        }
        p1 = nullptr; /// nullptr c++11 NULL 0
        delete p1;    /// null delete 安全
        if (!p1)
        {
            std::cout << "p1 is empty!" << std::endl;
        }
        delete p1; /// 多次 delete null安全
    }

    getchar();
    return 0;
}
