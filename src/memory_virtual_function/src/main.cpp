#include <iostream>

/// 虚函数表分析
class B1
{
public:
    virtual void Test1()
    {
        std::cout << "B1 test1" << std::endl;
    }
    virtual void Test2()
    {
        std::cout << "B1 test2" << std::endl;
    }

    int b1 = 100;
};
class B2
{
public:
    virtual void test3()
    {
        std::cout << "B2 test3" << std::endl;
    }

    virtual void test4()
    {
        std::cout << "B2 test4" << std::endl;
    }
    int b2 = 200;
};

class A : public B1
{
public:
    void Test1() override
    {
        std::cout << "A test1" << std::endl;
    }
    void Test2() override
    {
        std::cout << "A test2" << std::endl;
    }
};

class C : public B1
{
    void Test1() override
    {
        std::cout << "C test1" << std::endl;
    }
    void Test2() override
    {
        std::cout << "C test2" << std::endl;
    }
};

void TestClass(B1 *b)
{
    std::cout << "in TestClass" << std::endl;
    b->Test1();
    b->Test2();
}

class D : public B1, public B2
{
public:
    void Test1() override
    {
        std::cout << "D Test1" << std::endl;
    }
    void Test2() override
    {
        std::cout << "D Test2" << std::endl;
    }
    void test3() override
    {
        std::cout << "D test3" << std::endl;
    }
    void test4() override
    {
        std::cout << "D test4" << std::endl;
    }
    int a = 100;
};


template <typename dst_type, typename src_type>
static dst_type union_cast(src_type src)
{
    union
    {
        src_type s;
        dst_type d;
    } u;
    u.s = src;
    return u.d;
}

typedef void (**vptr)(); /// 指向虚指针数组的指针

int main(int argc, char *argv[])
{
    std::cout << "cppds.com" << std::endl;

    {
        B1 b;
        b.Test1();
        b.Test2();

        A a;
        a.Test1();
        a.Test2();

        TestClass(&a);

        C c;
        TestClass(&c);
    }
    std::cout << "============================" << std::endl;
    /// 多继承各个虚表的获取
    {
        D d1;
        std::cout << "&d1 = " << reinterpret_cast<unsigned long long>(&d1) << std::endl;
        vptr *vptr1 = reinterpret_cast<vptr *>(&d1); ///
        std::cout << "&(d1.b1) = " << reinterpret_cast<unsigned long long>(&d1.b1)
                  << std::endl; /// 首地址 + 8， 虚表指针就是8字节的 所以加8
        std::cout << "vptr1 = " << reinterpret_cast<unsigned long long>(vptr1) << std::endl; /// 首地址

        std::cout << "&D::Test1 = " << union_cast<unsigned long long>(&D::Test1) << std::endl;
        std::cout << "&D::Test2 = " << union_cast<unsigned long long>(&D::Test2) << std::endl;

        auto test1 = **vptr1;
        test1();
        std::cout << " test1 = " << reinterpret_cast<unsigned long long>(test1) << std::endl;

        auto test2 = *(*vptr1 + 1);
        test2();
        std::cout << " test2 = " << reinterpret_cast<unsigned long long>(test2) << std::endl;

        vptr *vptr2 = reinterpret_cast<vptr *>(&d1) + 2; /// 中间隔开了一个变量 8B一格

        std::cout << "vptr2 = " << reinterpret_cast<unsigned long long>(vptr2)
                  << std::endl; /// B2的虚表指针 /// 内存对齐 + 8

        std::cout << "&D::test3 = " << union_cast<unsigned long long>(&D::test3) << std::endl;
        std::cout << "&D::test4 = " << union_cast<unsigned long long>(&D::test4) << std::endl;
        /// 实际函数地址与 虚表里面的指针不一致

        (**vptr2)();
        (*(*vptr2 + 1))();

        std::cout << "&(d1.b2) = " << reinterpret_cast<unsigned long long>(&d1.b2) << std::endl;

        std::cout << "end = " << reinterpret_cast<unsigned long long>(&d1.b2) + 8 << std::endl;

        /// + 8 多一个变量

        std::cout << "sizeof(D) = " << sizeof(D) << std::endl; /// 40

        std::cout << "============================" << std::endl;
        /// 父类与子类的虚表不一致
        B1    b1     = static_cast<B1>(d1);
        vptr *vptrB1 = reinterpret_cast<vptr *>(&b1);
        std::cout << "vptrB1 = " << reinterpret_cast<unsigned long long>(vptrB1) << std::endl;
        (**vptrB1)();
        (*(*vptrB1 + 1))();

        B2    b2     = static_cast<B2>(d1);
        vptr *vptrB2 = reinterpret_cast<vptr *>(&b2);
        std::cout << "vptrB2 = " << reinterpret_cast<unsigned long long>(vptrB2) << std::endl;
        (**vptrB2)();
        (*(*vptrB2 + 1))();
    }
    std::cout << "============================" << std::endl;
    {
        A     a1;
        A     a2;
        vptr *vptrA1 = reinterpret_cast<vptr *>(&a1); /// 指向虚表(指针数组)的指针(三级指针)
        vptr *vptrA2 = reinterpret_cast<vptr *>(&a2);
        std::cout << "vptrA1 = " << reinterpret_cast<unsigned long long>(vptrA1) << std::endl; /// +48 中间父子都有变量
        std::cout << "vptrA2 = " << reinterpret_cast<unsigned long long>(vptrA2) << std::endl;
        std::cout << "*vptrA1 = " << reinterpret_cast<unsigned long long>(*vptrA1) << std::endl;
        std::cout << "*vptrA2 = " << reinterpret_cast<unsigned long long>(*vptrA2) << std::endl; ///指定同一个地址
        /// ***虚指针不一样 但是指向同一个虚表
    }
    getchar();
    return 0;
}
