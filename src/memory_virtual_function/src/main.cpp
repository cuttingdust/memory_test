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
};

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
    {
        typedef void (**vptr)();/// 指向虚指针数组的指针
        D d1;

        vptr *vptr1 = (vptr *)(&d1);
        vptr *vptr2 = (vptr *)(&d1) + 1;
        (**vptr1)();
        (*(*vptr1 + 1))();

        (**vptr2)();
        (*(*vptr2 + 1))();
    }
    getchar();
    return 0;
}
