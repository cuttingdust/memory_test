#include <iostream>

/// 经过参数传递 数组转成指针
void PtrFunction(const char data[])
{
    std::cout << "sizeof(data) = " << sizeof(data) << std::endl;
}

void PtrFunction(const char* data, int size)
{
    std::cout << "sizeof(data) = " << sizeof(data) << std::endl;
}

/// 传递参数 获取数组大小
template <class Ty, size_t Size>
void TestMemArr(Ty (&data)[Size])
{
    std::cout << "sizeof(data)=" << sizeof(data) << std::endl;
}

/// 指针输出
/// 1 修改原内存
/// 2 使用函数内创建的内存
char* TestMem()
{
    /// error 不能返回栈中空间
    char buf[1024] = "test";
    return buf;
}

void TestMem(char* out)
{
    /// 修改的局部变量 内存泄漏
    out = new char[1024];
}

/// 返回的空间需要注释，由用户释放
int TestMem(char** out)
{
    int size = 1024;
    /// 修改的局部变量 修改指向
    *out = new char[size];


    return size;
}

int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;
        char data[] = "test mem ptr";
        /// 为什么C语言 指针传输的时候 一定要传大小 因为指针指向的空间不知道大小
        PtrFunction(data, sizeof(data));
        PtrFunction(data);

        TestMemArr(data);
        std::cout << data << std::endl;

        /// 异常空间已经释放，结果不确定
        std::cout << TestMem() << std::endl;

        char* out = nullptr;
        TestMem(out);
        std::cout << "out  " << reinterpret_cast<int>(out) << std::endl;
        TestMem(&out);
        std::cout << "out  " << reinterpret_cast<int>(out) << std::endl;
        delete out;
        out = nullptr;
    }

    getchar();
    return 0;
}
