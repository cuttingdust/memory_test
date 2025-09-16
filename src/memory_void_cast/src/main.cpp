#include <iostream>


int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;

        /// void 指针和指针类型转换
        void* ptr1 = malloc(1024); /// 通常C语言 堆中申请 大部分都是 void*
        free(ptr1);

        int   num  = 1;
        void* ptr  = &num;
        int*  ptr2 = static_cast<int*>(ptr); /// void *  static_cast<int*> 好处是原先const 属性不变
        int*  ptr3 = (int*)ptr;
        ///static_cast 增加了一些验证
        const int* cptr1 = new int[1024]{ 0 };

        //int* ptr4 = static_cast<int*>(cptr1);
        int* ptr4 = (int*)(cptr1);
        int* ptr6 = const_cast<int*>(cptr1); /// 去掉const

        unsigned char* ucptr = new unsigned char[1024]{ 0 };
        // int* ptr5 = static_cast<int*>(ucptr); /// 1. 编译会报错 static_cast优点 类型检查 2. const 之类的属性保留
        auto ptr5 = (int*)ucptr;
        auto ptr7 = reinterpret_cast<int*>(ucptr); /// 替换指针类型


        delete[] ucptr;
        delete[] cptr1;
    }

    getchar();
    return 0;
}
