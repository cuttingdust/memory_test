#include <iostream>
class XData
{
public:
    int x = 0;
    int y = 0;
};

int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;
        /// 栈空间 未初始化数组 空间内部不定
        int arr1[10];                  //
        memset(arr1, 0, sizeof(arr1)); /// 数组可以通过sizeof获取内存大小

        int  arr2[5]    = { 1, 2, 3, 4, 5 };
        int  arr3[32]   = { 1, 2, 3 };
        int  arr4[1024] = { 0 }; /// 全部设置为0
        int  arr5[]     = { 1, 2, 3, 4 };
        char str1[]     = "test001";
        std::cout << "sizeof(arr5) = " << sizeof(arr5) << std::endl;
        std::cout << "sizeof(str1) = " << sizeof(str1) << std::endl;

        for (auto s : str1) /// 只能访问数组
        {
            std::cout << s << "-" << std::flush;
        }
        std::cout << std::endl;

        std::cout << "&arr2[0] = " << &arr2[0] << std::endl; /// 内存空间是连续的
        std::cout << "&arr2[1] = " << &arr2[1] << std::endl;
        std::cout << "arr2+2 = " << arr2 + 2 << std::endl;

        //std::begin(arr1);
        //std::end(arr1);
        std::find(std::begin(arr5), std::end(arr5), 3);

        //////////////////////////////////////////////////////////////////

        /// 堆空间
        int* parr1 = new int[1024];
        int  psize = 2048;
        auto parr2 = new unsigned char[psize];

        memset(parr2, 0, psize);
        auto parr3 = new int[psize];
        std::cout << "sizeof(parr3) = " << sizeof(parr3) << std::endl; /// sizeof(parr3)??? 32=>4 64=>8
        memset(parr3, 0, psize * sizeof(int));

        /// for(auto s:parr2)
        int* parr4 = new int[3]{ 1, 2, 3 };
        int* parr5 = new int[]{ 1, 2, 3, 6 };
        int* parr6 = new int[]{ 0 };

        std::cout << "parr5[2] = " << parr5[2] << std::endl;       /// 3
        std::cout << "*(parr5+3) = " << *(parr5 + 3) << std::endl; /// 6
        std::cout << "&parr5[0] = " << &parr5[0] << std::endl;     /// 首地址
        std::cout << "&parr5[1] = " << &parr5[1] << std::endl;     /// 第二个地址
        std::cout << "parr5+2 = " << parr5 + 2 << std::endl;       /// 第三个地址

        delete[] parr1;
        parr1 = nullptr; //NULL 0
        delete[] parr2;
        parr2 = nullptr;
        delete[] parr3;
        parr3 = nullptr;
        delete[] parr4;
        parr4 = nullptr;
        delete[] parr5;
        parr5 = nullptr;
        delete[] parr6;
        parr6        = nullptr;
        int    alloc_size = 1024;
        XData* datas      = new XData[alloc_size];
        std::cout << "sizeof(datas) = " << sizeof(datas) << std::endl;
        std::cout << "alloc_size * sizeof(datas) = " << alloc_size * sizeof(datas) << std::endl; /// 那片空间的大小
        delete[] datas;
        datas = nullptr;
    }

    getchar();
    return 0;
}
