#include <iostream>
#include <list>
#include <vector>
/*
## 分配器allocator
- 分配器用于实现容器算法时，将其与存储细节隔离从而解耦合。
- 分配器提供存储分配与释放的标准方法
- 对象内存分配和构造分离
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
        std::cout << index << ":Move Data: " << this <<  " " << &other <<  std::endl;
    }

    XData& operator=(const XData& other)
    {
        if (this != &other)
        { /// 防止自我赋值
            index = other.index;
            std::cout << index << ": Copy Assignment Data: " << this << " "<< &other <<   std::endl;
        }
        return *this;
    }

    XData& operator=(XData&& other) noexcept
    {
        if (this != &other)
        { /// 防止自我赋值
            index       = other.index;
            other.index = 0;
            std::cout << index << ": Move Assignment Data: " << this << " "<< &other <<   std::endl;
        }
        return *this;
    }

    ~XData()
    {
        std::cout << index << ":Drop Data: " << this << std::endl;
    }
};
int XData::count = 0;

template <class Ty>
class MyAllocator
{
public:
    /// 下面是个类型别名，实现中的可选项
    using value_type    = Ty;
    using pointer       = Ty*;
    using const_pointer = const Ty*;

    using void_pointer       = void*;
    using const_void_pointer = const void*;

    using size_type  = size_t;
    using difference = std::ptrdiff_t;

    using propagate_on_container_move_assignment           = std::true_type;
    using is_always_equal _CXX20_DEPRECATE_IS_ALWAYS_EQUAL = std::true_type;

    constexpr MyAllocator() noexcept {};
    ~MyAllocator() = default;

    template <class Other>
    constexpr MyAllocator(const MyAllocator<Other>&) noexcept
    {
    }

    /// 内存分配
    pointer allocate(const size_type numObjects)
    {
        auto alloc_area = std::malloc(sizeof(Ty) * numObjects);
        std::cout << typeid(Ty).name() << ":" << __func__ << ": " << "size = " << numObjects << " ptr = " << alloc_area  << std::endl;
        allocCount_ += numObjects;
        return static_cast<Ty*>(alloc_area);
    }

    /// 分配内存
    pointer allocate(size_type numObjects, const_void_pointer hint)
    {
        return allocate(numObjects);
    }

    /// 内存释放
    void deallocate(pointer const ptr, const size_type numObjects)
    {
        std::cout << typeid(Ty).name() << ":" << __func__ << ": " << "size = " << numObjects << " ptr = " << ptr
                  << std::endl;
        std::free(ptr);
        allocCount_ = allocCount_ - numObjects;
    }

    /// 分配器支持最大的内存数
    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max();
    }

    // /// 构造对象
    // template <class U, class... Args>
    // void construct(U* p, Args&&... args)
    // {
    //     new (p) U(std::forward<Args>(args)...);
    // }
    //
    // /// 销毁对象
    // template <class U>
    // void destroy(U* p)
    // {
    //     p->~U();
    // }

private:
    size_type allocCount_ = std::numeric_limits<size_type>::min(); ///< 统计当前内存的使用量
};


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
    std::cout << "=======================================================================================" << std::endl;

    {
        std::cout << "==================== vector =======================" << std::endl;
        std::vector<XData, MyAllocator<XData>> data_list;
        XData                                  data;
        data.index = 111;
        data_list.emplace_back(data);
        data.index = 222;
        data_list.emplace_back(data);
        data.index = 333;
        data_list.emplace_back(data);

        std::cout << "==================== list =======================" << std::endl;
        std::list<XData, MyAllocator<XData>> datas; /// 非动态内存管理
        data.index = 444;
        datas.push_back(data);
        data.index = 555;
        datas.push_back(data);
        data.index = 666;
        datas.push_back(data);
    }

    getchar();
    return 0;
}
