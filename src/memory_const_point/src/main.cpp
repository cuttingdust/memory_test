#include <iostream>


int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;

        /// 常量指针 and 指针常量
        /// 常量指针和指针常量
        const int  i1  = 100;
        const int  i2  = 102;
        const int* pi1 = &i1; /// 指向常量的指针
        // (*pi1)++;             /// 指向的值不可修改
        pi1 = &i2; /// 指针本身可以修改

        //////////////////////////////////////////////////////////////////
        int const* pi2 = &i1;

        // (*pi2)++;
        /// 指针本身是常量 指向不能修改 指向的值可以修改
        int* const pi3 = new int;
        // pi3++;
        // pi3 = new int;
        *pi3 = 200;
        delete pi3;
        const int* const pi4 = &i1; /// 指向和值都不能修改
        // pi4 = new int;
        // *pi4 = 300;
    }

    getchar();
    return 0;
}
