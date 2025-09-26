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
        std::cin >> *mem1;
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
    }

    getchar();
    return 0;
}
