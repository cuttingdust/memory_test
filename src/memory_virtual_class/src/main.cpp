#include <iostream>

///  继承的内存指针分析
class Base
{
public:
    int x1 = 1;
    int x2 = 2;
};

class A : public Base
{
public:
    int a1 = 4;
    int x1 = 5;
};

//////////////////////////////////////////////////////////////////
class Base1
{
public:
    int b11 = 1;
    int b12 = 2;
};
class Base2
{
public:
    int b21 = 3;
    int b22 = 4;
};
/// [Base1][Base2][A2]
class A2 : public Base1, public Base2
{
public:
    int a1 = 5;
    int a2 = 6;
};

//////////////////////////////////////////////////////////////////

/// 多继承二义性问题
class C
{
public:
    int c1;
};

class B1 : public C
{
public:
    int b1;
};

class B2 : public C
{
public:
    int b2;
};
/*
C  C
B1 B2
  A3
*/
class A3 : public B1, public B2
{
public:
    int a1 = 0;
};


//////////////////////////////////////////////////////////////////
class B3 : virtual public C
{
public:
    int b1;
};
class B4 : virtual public C
{
public:
    int b2;
};

/*
  C
B1 B2
  A4
*/
class A4 : public B3, public B4
{
public:
    int a1 = 0;
};

int main(int argc, char* argv[])
{
    std::cout << "cppds.com" << std::endl;

    /// 单继承的内存指针分析
    {
        Base b;
        /// &b
        std::cout << "address b = " << reinterpret_cast<long long>(std::addressof(b)) << std::endl;
        std::cout << "b.x1 address:" << reinterpret_cast<long long>(&b.x1) << std::endl;
        std::cout << "b.x2 address:" << reinterpret_cast<long long>(&b.x2) << std::endl;

        A* a = new A;
        std::cout << "address a = \t" << reinterpret_cast<long long>(a) << std::endl;
        std::cout << "a->x1 = " << a->x1 << std::endl;
        std::cout << "a->Base::x1 = " << a->Base::x1 << std::endl;
        std::cout << "a->Base::x2 = " << a->Base::x2 << std::endl;
        std::cout << "a->x1 address = \t" << reinterpret_cast<long long>(&a->x1) << std::endl;
        std::cout << "a->a1 address = \t" << reinterpret_cast<long long>(&a->a1) << std::endl;
        std::cout << "a->Base::x1 address = \t" << reinterpret_cast<long long>(&a->Base::x1) << std::endl;
        std::cout << "a->Base::x2 address = \t" << reinterpret_cast<long long>(&a->Base::x2) << std::endl;
        /// 子 &a->Base::x1 也是基类的地址 ， 先创基类 然后类  &a->a1 是子类的第一个元素地址,
        /// 没有虚函数 所以没有虚指针和虚表
        /// A类分析
        /// address a =     1567707721104
        /// a->x1 = 5
        /// a->Base::x1 = 1
        /// a->Base::x2 = 2
        /// a->x1 address =         1567707721116
        /// a->a1 address =         1567707721112
        /// a->Base::x1 address =   1567707721104
        /// a->Base::x2 address =   1567707721108
    }

    std::cout << "=============================================" << std::endl;
    /// 多继承内存分析
    {
        /// [Base1][Base2][A2] is a and is b
        A2 a;
        std::cout << "&a = \t\t" << reinterpret_cast<long long>(&a) << std::endl;
        std::cout << "&a.b11 = \t" << reinterpret_cast<long long>(&a.b11) << std::endl;
        std::cout << "&a.b12 = \t" << reinterpret_cast<long long>(&a.b12) << std::endl;
        std::cout << "&a.b21 = \t" << reinterpret_cast<long long>(&a.b21) << std::endl;
        std::cout << "&a.b22 = \t" << reinterpret_cast<long long>(&a.b22) << std::endl;
        std::cout << "&a.a1 = \t" << reinterpret_cast<long long>(&a.a1) << std::endl;
    }
    std::cout << "=============================================" << std::endl;
    /// 多继承二义性问题 (钻石继承 / 菱形继承)
    {
        A3 a;
        // a.C::c1 = 1; /// c1不明确
        a.B1::c1 = 1;
        // std::cout << "&a.C::c1 = " << reinterpret_cast<long long>(&a.C::c1) << std::endl; /// c1不明确
        std::cout << "&a.B1::c1 = " << reinterpret_cast<long long>(&a.B1::c1) << std::endl;
        std::cout << "&a.B2::c1 = " << reinterpret_cast<long long>(&a.B2::c1) << std::endl;
        std::cout << "-----------------------------------------" << std::endl;

        /// 虚继承 专门解决二义性问题
        A4 a4;
        std::cout << "&a4.C::c1 = " << reinterpret_cast<long long>(&a4.C::c1) << std::endl;
        std::cout << "&a4.B3::c1 = " << reinterpret_cast<long long>(&a4.B3::c1) << std::endl;
        std::cout << "&a4.B4::c1 = " << reinterpret_cast<long long>(&a4.B4::c1) << std::endl;
    }

    getchar();
    return 0;
}
