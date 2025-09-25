#include <iostream>

/*

# 指针作为函数参数传递
## 指针作为参数设计说明
- 传递输入内存，需要提供内存大小（数组到指针无法知道大小），并设置为const
- 传递输出内存，同样需要提供内存大小，防止溢出
- 如果指向函数中创建堆内存，需要传递指针引用或者指向指针的指针
- 通过返回值只能返回堆中空间，比较危险，需要函数中注释说明，防止内存泄漏
*/

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
    *out = new char[size]{ 0 };
    return size;
}

//////////////////////////////////////////////////////////////////

void swap(int* a, int* b)
{
    int temp = *a;
    *a       = *b;   /// 使用解引用来访问原始变量
    *b       = temp; /// 或者 动态 但是记住释放
}

void changePointer(int** p)
{
    // int b = 500; ///  b 出去 就释放 所以野了
    static int b = 500;
    *p           = &b;
}

//////////////////////////////////////////////////////////////////

void allocateMemory(int** ptr)
{
    ///  动态分配内存
    *ptr  = new int; /// 分配一个整数的内存
    **ptr = 42;      /// 设置新分配内存的值
}

void freeMemory(int** ptr)
{
    delete *ptr;    /// 释放指针指向的内存
    *ptr = nullptr; /// 将指针设为 nullptr，避免悬挂指针
}

/// 智能指针作为函数参数和返回值
class XData
{
public:
    static int count;
    int        index = 0;
    XData()
    {
        count++;
        index = count;
        std::cout << index << ":Create Data: " << this << std::endl;
    }
    XData(const XData& other)
    {
        index = other.index;
        std::cout << index << ":Copy Data: " << this << std::endl;
    }

    XData(XData&& other) noexcept
    {
        index       = other.index;
        other.index = 0;
        std::cout << index << ":Move Data: " << this << std::endl;
    }

    XData& operator=(const XData& other)
    {
        if (this != &other)
        { /// 防止自我赋值
            index = other.index;
            std::cout << index << ": Copy Assignment Data: " << this << std::endl;
        }
        return *this;
    }

    XData& operator=(XData&& other) noexcept
    {
        if (this != &other)
        { /// 防止自我赋值
            index       = other.index;
            other.index = 0;
            std::cout << index << ": Move Assignment Data: " << this << std::endl;
        }
        return *this;
    }

    ~XData()
    {
        std::cout << index << ":Drop Data: " << this << std::endl;
    }
};
int XData::count = 0;


std::unique_ptr<XData> TestUnique(std::unique_ptr<XData> ptr)
{
    std::cout << "ptr->index=" << ptr->index << std::endl;
    std::unique_ptr<XData> re(new XData);
    re->index = 222;
    /// 编译器优化 1 如果返回值支持move 编译器自动调用move,不支持调用拷贝构造
    return re;
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

        //////////////////////////////////////////////////////////////////
        char* out = nullptr;
        TestMem(out);
        std::cout << "out  " << int(out) << std::endl; /// null
        TestMem(&out);
        std::cout << "out  " << int(out) << std::endl; /// not null
        delete out;
        out = nullptr;

        //////////////////////////////////////////////////////////////////

        int a = 10;
        int b = 5;
        std::cout << "a = " << a << "; " << "b = " << b << std::endl;
        swap(&a, &b);
        std::cout << "a = " << a << "; " << "b = " << b << std::endl;

        int  p = 100;
        int* q = &p;
        std::cout << "p = " << *q << std::endl;
        changePointer(&q); /// 修改指针指向的值
        std::cout << "p = " << *q << std::endl;

        //////////////////////////////////////////////////////////////////
        int* myPointer = nullptr; ///  初始化指针为 nullptr

        /// 分配内存并设置值
        allocateMemory(&myPointer);                                         /// 传递指向指针的指针
        std::cout << "Value after allocation: " << *myPointer << std::endl; /// 输出 42

        /// 释放内存
        freeMemory(&myPointer); /// 释放内存
        if (myPointer == nullptr)
        {
            std::cout << "Memory freed and pointer set to nullptr." << std::endl;
        }
    }

    //////////////////////////////////////////////////////////////////
    {
        XData s1;
        XData s2;
        s2 = s1; /// 拷贝运算符

        XData s3;
        XData s4 = s3; /// 拷贝构造

        XData s5;
        XData s6(s5); /// 拷贝构造


        XData s7;
        XData s8;
        s8 = std::move(s7);

        XData s9;
        XData s10 = std::move(s9); /// 移动构造

        XData s11;
        XData s12(std::move(s11)); /// 移动构造

        auto p1 = std::make_shared<XData>();
        auto p2 = std::make_shared<XData>();
        p2      = p1; /// 只是指针指向

        //////////////////////////////////////////////////////////////////
        /// 使用 std::unique_ptr
        std::cout << "Using std::unique_ptr:" << std::endl;
        std::unique_ptr<XData> uPtr1(new XData()); /// 创建 uPtr1，调用默认构造
        // std::unique_ptr<XData> uPtr2 = uPtr1; /// 错误：不能拷贝
        std::unique_ptr<XData> uPtr3 = std::move(uPtr1); /// 移动 uPtr1 到 uPtr3 ///  std::unique_ptr的移动构造
        /// uPtr1 现在为空
        if (uPtr1 == nullptr)
        {
            std::cout << "uPtr1 move success." << std::endl;
        }
        else
        {
            std::cout << uPtr1->index << ":uPtr1 move failded!!!" << std::endl;
        }
        /// 指针传递的话 不会dayin

        uPtr3->index = 111;
        std::cout << "uPtr3->index=" << uPtr3->index << std::endl;
        auto re = TestUnique(std::move(uPtr3));
        std::cout << "re->index=" << re->index << std::endl;
        if (uPtr3 == nullptr)
        {
            std::cout << "uPtr3 move success." << std::endl;
        }
        else
        {
            std::cout << uPtr3->index << ":uPtr1 move failded!!!" << std::endl;
        }
    }


    getchar();
    return 0;
}
