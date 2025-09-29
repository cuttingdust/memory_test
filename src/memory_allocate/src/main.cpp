#include <iostream>
#include <vector>
/*
## 分配器allocator
- 分配器用于实现容器算法时，将其与存储细节隔离从而解耦合。
- 分配器提供存储分配与释放的标准方法
- 对象内存分配和构造分离
*/
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


/// #include <memory>
/// std::allocator
/// address (C++17 中弃用)(C++20 中移除)
/// allocate 分配未初始化的存储
/// deallocate 解分配存储
/// max_size(C++17 中弃用)(C++20 中移除)
/// construct (C++17 中弃用)(C++20 中移除)
/// destroy(C++17 中弃用)(C++20 中移除)

int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;

        std::allocator<XData> xdata_alloc;
        int                   size = 3;

        /// 分配对象空间 不调用构造
        auto data_arr = xdata_alloc.allocate(size);
        for (int i = 0; i < size; ++i)
        {
            /// allocator_traits 类模板提供访问分配器 (Allocator) 各种属性的标准化方式
            /// allocator_traits< allocator<XData> >

            /// 调用构造
            std::allocator_traits<decltype(xdata_alloc)>::construct(xdata_alloc, &data_arr[i]);

            /// 调用析构函数
            std::allocator_traits<decltype(xdata_alloc)>::destroy(xdata_alloc, &data_arr[i]);
        }

        /// 释放对象内存 不调用析构
        xdata_alloc.deallocate(data_arr, size);
    }

    getchar();
    return 0;
}
