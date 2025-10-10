#include <iostream>

/// 虚函数表分析
class B
{
public:
    virtual void Test1()
    {
        std::cout << "B test1" << std::endl;
    }
    virtual void Test2()
    {
        std::cout << "B test2" << std::endl;
    }
};

class A : public B
{
public:
    void Test1() override
    {
        std::cout << "A test1" << std::endl;
    }
    void Test2() override
    {
        std::cout << "A test1" << std::endl;
    }
};

class C : public B
{
    void Test1() override
    {
        std::cout << "C test1" << std::endl;
    }
    void Test2() override
    {
        std::cout << "C test1" << std::endl;
    }
};

void TestClass(B *b)
{
    std::cout << "in TestClass" << std::endl;
    b->Test1();
}

int main(int argc, char *argv[])
{
    std::cout << "cppds.com" << std::endl;

    {
        B b;
        b.Test1();

        A a;
        a.Test1();

        TestClass(&a);

        C c;
        TestClass(&c);
    }
    getchar();
    return 0;
}
