#include <iostream>
#include <string>
#include <memory>    // 包含 std::unique_ptr
#include <ranges>    // 包含 ranges
#include <algorithm> // 包含 std::ranges::uninitialized_copy
#include <cstddef>   // 包含 std::size
#include <cstdlib>   // 包含 std::aligned_alloc
#include <iomanip>

/*
##  未初始化内存算法 uninitialized_copy
- 将范围内的对象复制到未初始化的内存区域 
- 为什么不用memcpy或者std::copy? uninitialized_copy会对对象进行复制（拷贝构造），
- 不复制如果内部有堆栈空间，将产生问题
*/


/// ## 自定义分配器
/// - 可以实现内存共享、内存泄漏探测，预分配对象存储、内存池
/// - 演示自定义vector和list分配器，并分析其源码

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
        std::cout << index << ":Copy Data: " << this << " " << &other << std::endl;
    }

    XData(XData&& other) noexcept
    {
        index       = other.index;
        other.index = 0;
        std::cout << index << ":Move Data: " << this << " " << &other << std::endl;
    }

    XData& operator=(const XData& other)
    {
        if (this != &other)
        { /// 防止自我赋值
            index = other.index;
            std::cout << index << ": Copy Assignment Data: " << this << " " << &other << std::endl;
        }
        return *this;
    }

    XData& operator=(XData&& other) noexcept
    {
        if (this != &other)
        { /// 防止自我赋值
            index       = other.index;
            other.index = 0;
            std::cout << index << ": Move Assignment Data: " << this << " " << &other << std::endl;
        }
        return *this;
    }

    ~XData()
    {
        std::cout << index << ":Drop Data: " << this << std::endl;
    }
};
int XData::count = 0;


int main(int argc, char* argv[])
{
    std::cout << "cppds.com" << std::endl;

    {
        std::cout << "===============memcpy==============" << std::endl;
        {
            XData         datas[3];
            unsigned char buf[1024] = { 0 };
            memcpy(buf, &datas, sizeof(datas));
        }

        std::cout << "===============copy==============" << std::endl;
        {
            XData         datas[3];
            unsigned char buf[1024] = { 0 }; /// 初始化过的内存 所知直接 dst=src /// 所以这边是运算符
            std::ranges::copy(datas, reinterpret_cast<XData*>(buf));
        }
        std::cout << "===============uninitialized_copy==============" << std::endl;
        {
            XData         datas[3];
            unsigned char buf[1024] = { 0 }; /// 拷贝直接构造 未初始化内存 /// 所以这边是拷贝构造
            std::uninitialized_copy(std::begin(datas), std::end(datas), reinterpret_cast<XData*>(buf));
        }
        std::cout << "===========================================" << std::endl;
    }

    /// c++17 20 construct_at destroy对象构造和销毁
    /// c++17 destroy对象和销毁
    /// c++20 construct_at 对象构造
    {
        std::cout << "===============construct_at destroy ============================" << std::endl;
        int  size = 3;
        auto data = static_cast<XData*>(std::malloc(sizeof(XData) * size)); /// 未初始化内存
        std::cout << "data: " << data << std::endl;
        std::ranges::uninitialized_default_construct_n(data, size);
        std::uninitialized_default_construct_n(data, size); /// range 和 非 range 一样
        std::uninitialized_default_construct(data, data + size);
        for (int i = 0; i < size; i++)
        {
            /// c++20 调用构造函数
            if (data)
                std::ranges::construct_at(&data[i]); /// 直接调用构造对象 在这块内存上面

            std::ranges::destroy_at(&data[i]); /// 析构对象
        }
        /// c++ 17 调用析构
        std::ranges::destroy(data, data + size);
        std::ranges::destroy_n(data, size); /// 析构所有的
        std::free(data);
        std::cout << "===========================================" << std::endl;
    }
    getchar();
    return 0;
}
