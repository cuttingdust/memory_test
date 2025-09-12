#include <iostream>

/// 保留区             0地址开始C 库
/// 代码区             .text 函数代码                                  高地址
/// 常量区             .rodata 字符串
/// 全局(静态)变量区   .data 非0区和.bass 未初始化（全局 静态）
/// 堆区                                                                 ↑
/// 栈区                                                                 ↓
/// 命令行参数空间
/// 内核（windows2G/linux1G）                                          低地址

/// 整体 上面的就像倒着的杯子 看待  整体开始内存不连续

/// 堆 向下增长 (低地址 向 高地址) 倒是连续的 但是 和 内存对齐的策略有点关系
/// 栈 向上增长 (高地址 向低地址)


constexpr int g0 = 4;
int           g1; ///< 全局变量 未初始化
int           g2  = 0;
int           g3  = 1;
int           g4  = 2;
static int    sg1 = 3;
int           main(int argc, char* argv[])
{
    {
        static int s1 = 5;
        std::cout << "memory address! cppds.com" << std::endl;
        /// 代码区 .text
        std::cout << "代码区 main =" << main << std::endl;
        /// 常量区
        std::cout << "常量区 g0 =" << &g0 << std::endl;
        std::cout << "全局未初始化  g1=" << &g1 << std::endl;
        /// &g1 + 4 = &g2
        std::cout << "全局初始化为0 g2=" << &g2 << std::endl;
        /// 但是 &g2 - 28 =  &g3  g3 = 0
        /// 0 不一样
        std::cout << "全局初始化为1 g3=" << &g3 << std::endl;
        /// 从这个开始 后面一次依次向右4地址
        std::cout << "全局初始化为2 g4=" << &g4 << std::endl;
        std::cout << "静态全局初始化为3 sg1=" << &sg1 << std::endl;
        std::cout << "静态局部初始化为5 s1=" << &s1 << std::endl;
        /// 全局区 静态区 是在一块内存
        ///
        int* p1 = new int;
        int* p2 = new int;
        int* p3 = new int;
        int* p4 = new int;
        std::cout << "堆空间地址 p1=" << p1 << std::endl;
        std::cout << "堆空间地址 p2=" << p2 << std::endl;
        std::cout << "堆空间地址 p3=" << p3 << std::endl;
        std::cout << "堆空间地址 p4=" << p4 << std::endl;
        std::cout << "指针变量的栈空间地址 &p1=" << &p1 << std::endl;
        std::cout << "指针变量的栈空间地址 &p2=" << &p2 << std::endl;

        delete p1;
        delete p2;
        delete p3;
        delete p4;
        p1 = nullptr;
        p2 = nullptr;
        p3 = nullptr;
        p4 = nullptr;

        int i1 = 100;
        int i2 = 101;
        int i3 = 102;
        int i4 = 103;
        std::cout << "栈空间地址 i1=" << &i1 << std::endl;
        std::cout << "栈空间地址 i2=" << &i2 << std::endl;
        std::cout << "栈空间地址 i3=" << &i3 << std::endl;
        std::cout << "栈空间地址 i4=" << &i4 << std::endl;
    }

    getchar();
    return 0;
}
