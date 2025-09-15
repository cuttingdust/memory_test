#include <algorithm>
#include <iostream>
class XData
{
public:
    int x = 0;
    int y = 0;
};
#define ARRSIZE 3
int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;
        /// 栈二维数组初始化
        /// 2x3
        /// 1 2 3
        /// 4 5 6
        unsigned char arr1[2][ARRSIZE] = { { 1, 2, 3 }, { 4, 5, 6 } };
        unsigned char arr2[][ARRSIZE]  = { { 1, 2, 3 }, { 2, 3, 4 }, { 3, 4, 5 } };
        std::cout << "arr1[2][3] sizeof(arr1) = " << sizeof(arr1) << std::endl;
        std::cout << "arr2[][3] sizeof(arr2) = " << sizeof(arr2) << std::endl;

        int arr3[2][3][4] = {
            { { 1, 2, 3, 4 }, { 2, 3, 4, 5 }, { 1, 2, 3, 4 } },
            { { 1, 2, 3, 4 }, { 2, 3, 4, 5 }, { 1, 2, 3, 4 } },
        };
        int arr4[][3][4] = {
            { { 1, 2, 3, 4 }, { 2, 3, 4, 5 }, { 1, 2, 3, 4 } },
            { { 1, 2, 3, 4 }, { 2, 3, 4, 5 }, { 1, 2, 3, 4 } },
            { { 1, 2, 3, 4 }, { 2, 3, 4, 5 }, { 1, 2, 3, 4 } },
        };
        for (auto arr : arr2)
        {
            for (int i = 0; i < ARRSIZE; i++)
            {
                std::cout << static_cast<int>(arr[i]) << " " << std::flush;
            }
            std::cout << '\n';
        }

        int width  = ARRSIZE;                                          /// 列数
        int height = sizeof(arr2) / (ARRSIZE * sizeof(unsigned char)); /// 行数

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                arr2[i][j]++;
                std::cout << static_cast<int>(arr2[i][j]) << "-" << std::flush;
            }
            std::cout << '\n';
        }
    }
    {
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

        std::cout << "&arr2[0] = " << &arr2[0] << std::endl;
        std::cout << "&arr2[1] = " << &arr2[1] << std::endl;
        std::cout << "arr2+2 = " << arr2 + 2 << std::endl;
        //std::begin(arr1);
        //std::end(arr1);
        std::ranges::find(arr5, 3);

        //////////////////////////////////////////////////////////////////
        /// 堆空间
        int* parr1 = new int[1024];
        int  psize = 2048;
        auto parr2 = new unsigned char[psize];
        memset(parr2, 0, psize);
        auto parr3 = new int[psize];
        memset(parr3, 0, psize * sizeof(int)); //sizeof(parr3)??? 32=>4 64=>8
        /// for(auto s:parr2)
        int* parr4 = new int[3]{ 1, 2, 3 };
        int* parr5 = new int[]{ 1, 2, 3, 6 };
        int* parr6 = new int[]{ 0 };

        std::cout << "parr5[2] = " << parr5[2] << std::endl;       /// 3
        std::cout << "*(parr5+3) = " << *(parr5 + 3) << std::endl; /// 6
        std::cout << "&parr5[0] = " << &parr5[0] << std::endl;     /// 首地址
        std::cout << "&parr5[1] = " << &parr5[1] << std::endl;     /// 第二个地址
        std::cout << "parr5+2 = " << parr5 + 2 << std::endl;       /// 第三个地址  /// 空间连续

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
        XData* datas = new XData[1024];
        delete[] datas;
        datas = nullptr;

        return 0;
    }

    getchar();
    return 0;
}
