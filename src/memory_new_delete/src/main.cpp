#include <iostream>
#include <vector>
#include <string>
/*
## 重载new和delete
### 为什么要重载
- 监测内存创建销毁 ，统计和监控泄漏
- 内存对齐的处理
- 特定应用：例如多进程内存共享
### 重载全局的new和delete
*/

class TestMem
{
public:
    static int count;
    int        index = 0;
    TestMem()
    {
        count++;
        index = count;
        std::cout << index << ":Create Mem: " << this << std::endl;
    }

    ~TestMem()
    {
        std::cout << index << ":Drop Mem: " << this << std::endl;
    }

    void* operator new(size_t size)
    {
        auto mem = malloc(size);
        if (!mem)
        {
            throw std::bad_alloc();
        }
        std::cout << "TestMem new: " << size << ": " << mem << std::endl;
        return mem;
    }

    void operator delete(void* ptr)
    {
        std::cout << "TestMem operator delete: " << ptr << std::endl;
        std::free(ptr);
    }

    void* operator new[](size_t size)
    {
        auto mem = malloc(size);
        if (!mem)
        {
            throw std::bad_alloc();
        }
        std::cout << "TestMem new []: " << size << ": " << mem << std::endl;
        return mem;
    }

    void operator delete[](void* ptr)
    {
        std::cout << "TestMem operator delete[]: " << ptr << std::endl;
        std::free(ptr);
    }

    friend std::ostream& operator<<(std::ostream& os, const TestMem& obj)
    {
        os << "TestMem Object - Index: " << obj.index << ", Address: " << &obj;
        return os;
    }

    /// 重载 >> 运算符以将 index 提取到 std::string
    friend std::istream& operator>>(std::istream& is, TestMem& mem)
    {
        std::string input;
        is >> input; /// 从流中读取数据

        /// 将输入转换为数字并设置到 index
        mem.index = std::stoi(input); /// 假设输入是合法的整数
        return is;
    }

    /// 另一个重载版本，将 index 提取到 std::string
    friend std::ostream& operator>>(TestMem& mem, std::string& output)
    {
        output = "Index: " + std::to_string(mem.index); /// 提取 index 值到字符串
        return std::cout;                               /// 返回输出流以便链式调用
    }

    TestMem& operator<<(const char* inStr)
    {
        std::cout << "TestMem& operator << (const char* inStr): " << inStr << std::endl;
        return *this;
    }


    /// 重载的太空船运算符
    auto operator<=>(const TestMem& other) const
    {
        std::cout << "auto operator<=>(const TestMem& other) const" << std::endl;
        return index <=> other.index; /// 根据 index 进行比较
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// placement new
    void* operator new(size_t size, void* ptr) noexcept
    {
        std::cout << "placement new: " << size << ": " << ptr << std::endl;
        return ptr;
    }

    void operator delete(void* ptr1, void* ptr2) noexcept
    {
        std::cout << "placement delete ptr1, ptr2: " << ptr1 << " " << ptr2 << std::endl;
        ptr1 = nullptr;
        ptr2 = nullptr;
    }
};
int TestMem::count = 0;

// /// 重载 << 操作符
// std::ostream& operator<<(std::ostream& os, const TestMem& obj)
// {
//     os << "TestMem Object - Index: " << obj.index << ", Address: " << &obj;
//     return os;
// }

/// 重载全局的new和delete
void* operator new(size_t size)
{
    auto mem = std::malloc(size);
    if (!mem)
    {
        throw std::bad_alloc();
    }
    std::cout << "operator new :" << size << ": " << mem << std::endl;
    return mem;
}

void operator delete(void* ptr)
{
    std::cout << "operator delete:" << ptr << std::endl;
    std::free(ptr);
}

void* operator new[](size_t size)
{
    auto mem = std::malloc(size);
    if (!mem)
    {
        throw std::bad_alloc();
    }
    std::cout << "operator new [] :" << size << ": " << mem << std::endl;
    return mem;
}

void operator delete[](void* ptr)
{
    std::cout << "operator delete[]: " << ptr << std::endl;
    std::free(ptr);
}

int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;
        int* i1 = new int;
        delete i1;

        auto mem1 = new TestMem;
        std::cout << mem1 << std::endl;
        *mem1 << "change test";

        /// 使用重载的 >> 运算符提取 index 到 std::string
        std::string indexStr;
        *mem1 >> indexStr; /// 提取 index 值到 indexStr

        std::cout << indexStr << std::endl; /// 输出提取的 index 值
        // std::cin >> *mem1;
        std::cout << *mem1 << std::endl;

        auto mem2 = new TestMem;
        std::cout << mem2 << std::endl;
        std::cout << *mem2 << std::endl;
        /// 比较对象
        if (*mem1 < *mem2)
        {
            std::cout << "mem1 is less than mem2" << std::endl;
        }
        else if (*mem1 > *mem2)
        {
            std::cout << "mem1 is greater than mem2" << std::endl;
        }
        else
        {
            std::cout << "mem1 is equal to mem2" << std::endl;
        }
        delete mem1;
        delete mem2;

        int* arr1 = new int[4];
        delete[] arr1;

        TestMem* memarr1 = new TestMem[2];
        delete[] memarr1;
    }
    std::cout << "=======================================" << std::endl;

    {
        /// Placement new 是
        ///         C++ 中的一种特殊形式的 new 运算符，它允许您在已分配的内存地址上构造对象，而不是在堆上或栈上分配新的内存。Placement new 的主要用途是在特定的内存区域中构造对象，这在某些情况下是非常有用的。
        ///         主要是 STL 里面常用
        ///         以下是一些 placement new 的主要用途：
        ///
        ///         1. 自定义内存管理 在某些情况下，您可能需要控制对象的内存分配和释放。使用 placement
        ///                 new，您可以在特定的内存区域中构造对象，而不是依赖默认的内存分配器。例如，在嵌入式系统中，可能需要在特定的内存区域中构造对象，以保证高效性和资源利用。
        ///
        ///         2. 在已分配的内存中重用对象
        ///                 当您需要在同一块内存区域中重用对象时，placement new 是非常有用的。例如，您可以在一个缓冲区中多次构造和销毁对象，而不需要每次都重新分配内存。这样可以降低内存碎片和分配开销。
        ///
        ///         3. 性能优化 在性能敏感的应用中，使用
        ///                 placement new 可以减少内存分配和释放的开销。通过在预先分配的内存中直接构造对象，可以提高性能，尤其是在频繁创建和销毁对象的情况下。
        ///
        ///         4. 与低级 API 交互 当您使用一些低级 API（例如图形库或硬件接口）时，这些 API
        ///                 可能要求您在特定的内存地址上放置数据或对象。Placement new 允许您在这些特定地址上构造对象，以满足外部接口的要求。
        ///
        ///         5. 对齐需求 有些平台或数据结构可能要求特定的对齐方式。使用 placement
        ///                 new，您可以在对齐的内存区域中构造对象，以满足这些需求。

        int      buf1[1024] = { 0 }; /// 栈上
        TestMem* mem3       = new (buf1) TestMem;
        std::cout << "buf1 addr:" << buf1 << std::endl;
        std::cout << "mem3 addr:" << mem3 << std::endl;
        mem3->~TestMem(); /// 手动释放
        // delete mem3; /// 不能调用
        TestMem::operator delete(mem3, buf1);
        std::cout << "buf1 addr:" << buf1 << std::endl;
        std::cout << "mem3 addr:" << mem3 << std::endl;

        int* buf2 = new int[1024]{ 0 }; /// 堆上
        auto mem4 = new (buf2) TestMem;
        delete mem4;

        int* buf3 = new int[1024]{ 0 };
        auto mem5 = new (buf3) TestMem;
        std::cout << "buf3 addr:" << buf3 << std::endl;
        std::cout << "mem5 addr:" << mem5 << std::endl;
        mem4->~TestMem(); /// 手动释放
        delete[] buf3;
    }

    getchar();
    return 0;
}
