#include <functional>
#include <iostream>

class Data
{
public:
    static int count;
    int        index = 0;
    Data()
    {
        count++;
        index = count;
        std::cout << index << ":Create Data: " << this << std::endl;
    }
    ~Data()
    {
        std::cout << index << ":Drop Data: " << this << std::endl;
    }
};
int Data::count = 0;


struct XPacket
{
    unsigned char* data = nullptr;
    int            size = 0;
    /// 构造函数
    XPacket()
    {
        std::cout << "XPacket created with size: " << size << std::endl;
    }

    /// 析构函数
    ~XPacket()
    {
        std::cout << "XPacket destroyed" << std::endl;
    }
};

struct PacketDelete /// 仿函数（也叫 函数对象 可调用函数对象，英语：Functor）。
{
    void Close()
    {
        /// 提供主动删除函数
        std::cout << "Call PakcetDelete Close!" << std::endl;
    }
    void operator()(XPacket* ptr) const
    {
        std::cout << "call PacketDelete()" << std::endl;
        delete ptr->data;
        ptr->data = nullptr;
        ptr->size = 0;
        delete ptr; /// 自定义释放逻辑
        ptr = nullptr;
    }
};

int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;
        /// 1 unique_ptr 初始化
        /// RAII资源获取即初始化（Resource Acquisition Is Initialization）
        {
            std::unique_ptr<int>  p1(new int);
            std::unique_ptr<Data> p2(new Data); /// 1
            std::cout << "p2 = " << p2 << std::endl;
            std::cout << "p2.get() = " << p2.get() << std::endl;
            std::cout << "======1======" << std::endl;
            auto p3 = std::make_unique<Data>(); /// 2

            /// 空智能指针
            std::unique_ptr<Data> p4; /// p4 = null
            std::cout << "p4 = " << p4 << std::endl;
            std::cout << "p4.get() = " << p4.get() << std::endl;    /// 居然可以 /// 栈上std::unique_ptr对象  先进后出
            std::cout << "*p4 size = " << sizeof(*p4) << std::endl; ///指向的同一块地址
            std::cout << "*p4.get() size = " << sizeof(*(p4.get())) << std::endl;

            p4.swap(p3); /// p4 = p3;
            std::cout << "p4 = " << p4 << std::endl;
            std::cout << "p4.get() = " << p4.get() << std::endl;
            std::cout << "p4 size = " << sizeof(*p4) << std::endl;

            int                    arraySize = 1024;
            std::unique_ptr<int[]> pa1       = std::make_unique<int[]>(arraySize);
            std::cout << "Size of pa1: " << arraySize * sizeof(int) << " bytes" << std::endl;
            std::unique_ptr<int[]> pa2(new int[1024]);

            std::unique_ptr<Data[]> pa3(new Data[3]);
            std::unique_ptr<Data[]> pa4 = std::make_unique<Data[]>(3);

            ////////////////////////////////////////////////////////////////////////////////////
            /// unique_ptr智能指针访问和移动构造赋值
            std::cout << "======3======" << std::endl;
            *p1 = 10;
            std::cout << "p1 value=" << *p1 << std::endl;
            std::cout << "(*p2).index =" << (*p2).index << std::endl;
            std::cout << "p2->index =" << p2->index << std::endl;
            auto                  d1 = new Data();
            std::unique_ptr<Data> p5(d1);
            std::cout << "p5 address:" << p5 << std::endl;
            std::cout << "d1 address:" << d1 << std::endl;
            std::cout << "p5.get():" << p5.get() << std::endl;

            /// unique_ptr智能指针数组访问
            pa2[1] = 100;
            pa2[0] = 1;

            std::cout << "pa2[0] = " << pa2[0] << std::endl;
            std::cout << "pa2[1] = " << pa2[1] << std::endl;
            std::cout << "pa2.get()[1] = " << pa2.get()[1] << std::endl;
            std::cout << "*pa2.get()= " << *pa2.get() << std::endl;

            std::cout << " pa3[1].index = " << pa3[1].index << std::endl;
            std::cout << "pa3.get()[2].index = " << pa3.get()[2].index << std::endl;

            //////////////////////////////////////////////////////////////////
            std::cout << "======5======" << std::endl;
            /// 重置和移动内存资源
            /// unique_ptr
            std::unique_ptr<Data> p6(new Data());
            std::cout << "p6 = " << p6 << std::endl;

            /// 不可复制构造
            // std::unique_ptr<Data> p7 = p6;
            /// 不可以赋值
            // std::unique_ptr<Data> p8;
            // p8 = p6;
            /// 支持移动构造 p6释放所有权 转移到p7
            std::unique_ptr<Data> p7 = move(p6);
            std::cout << "p7 = " << p7 << std::endl;
            std::unique_ptr<Data> p8(new Data());
            std::cout << "p8 = " << p8 << std::endl; /// 11:Create Data

            /// 支持移动赋值 p7 是有实例的 所以移动的话 会释放p7的  10:Drop Data
            p7 = move(p8);
            std::cout << "after move(p8), p7 = " << p7 << std::endl;

            ///重置空间，原空间清理
            p7.reset(new Data());
            std::cout << "after reset new, p7 = " << p7 << std::endl;

            //////////////////////////////////////////////////////////////////
            /// 释放所有权和自定义空间删除方法
            /// 主动释放空间
            p7 = nullptr; /// 这个好 主动释放 也没有重复释放
            std::cout << "p7 = null, p7 = " << p7 << std::endl;

            std::unique_ptr<Data> p9(new Data);
            std::cout << "p9 = " << p9 << std::endl;
            std::cout << "p9->index = " << p9->index << std::endl;
            /// 释放所有权 不会主动释放
            auto ptr9 = p9.release(); /// p9 自动 null
            std::cout << "auto ptr9 = p9.release()， ptr9 = " << ptr9 << std::endl;
            std::cout << "auto ptr9 = p9.release()， p9 = " << p9 << std::endl;
            delete ptr9; /// 还原老指针控制方法 ptr9野了
            std::cout << "after delete ptr9 , ptr9 = " << ptr9 << std::endl;
            // p9 = nullptr;
            // std::cout << "after p9 = null, p9 = " << p9 << std::endl; /// 这个时候 = null 也不会主动释放 p9已经null了
            ptr9 = nullptr;

            //////////////////////////////////////////////////////////////////
            /// 自定义空间删除方法
            std::cout << "======7======" << std::endl;

            /// 自定义删除器，负责释放 XPacket 结构和其内部的 data
            /// lamdba
            auto customDeleter = [](XPacket* ptr) /// 全局函数 或者 有名lamdba
            {
                std::cout << "Custom deleter called for XPacket" << std::endl;
                delete ptr->data;
                ptr->size = 0;
                delete ptr; /// 这里调用析构函数，释放内存
            };
            /// <> 里面必须是指定类型
            std::unique_ptr<XPacket, decltype(customDeleter)> pd1(new XPacket);

            std::unique_ptr<XPacket, std::function<void(XPacket*)>> pd2(
                    new XPacket,
                    [](XPacket* my) -> void
                    {
                        std::cout << "anonymous lambda deleter called for XPacket" << std::endl;
                        delete my->data;
                        my->size = 0;
                        delete my; /// 这里调用析构函数，释放内存
                    });

            std::unique_ptr<XPacket, PacketDelete> pd3(new XPacket);

            std::unique_ptr<XPacket, PacketDelete> pd4(new XPacket);
            pd4.get_deleter().Close();
            pd4.get_deleter()(pd4.get()); /// 如果主动调用删除器 那就必须 归还所有权
            pd4.release();

            std::cout << "======8======" << std::endl;
            std::cout << "======6======" << std::endl;
            std::cout << "======4======" << std::endl;
        }
        std::cout << "======2======" << std::endl;
    }

    getchar();
    return 0;
}
