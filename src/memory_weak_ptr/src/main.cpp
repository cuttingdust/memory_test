#include <iostream>
/*
## weak_ptr
    use_count
        返回管理该对象的 shared_ptr 对象数量
    lock
        创建管理被引用的对象的shared_ptr
*/

class B;
class A
{
public:
    A()
    {
        std::cout << "Create A" << std::endl;
    }
    ~A()
    {
        std::cout << "Drop A" << std::endl;
    }
    void Do()
    {
        std::cout << "Do b2.use_count() = " << b2.use_count() << std::endl;
        auto b = b2.lock(); /// 复制一个共享智能指针 引用计数+1
        std::cout << "Do b2.use_count() = " << b2.use_count() << std::endl;
    }
    std::shared_ptr<B> b1;
    std::weak_ptr<B>   b2;
};
class B
{
public:
    B()
    {
        std::cout << "Create B" << std::endl;
    }
    ~B()
    {
        std::cout << "Drop B" << std::endl;
    }

    std::shared_ptr<A> a1;
};

int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;
        auto a = std::make_shared<A>(); /// =1
        auto b = std::make_shared<B>(); /// =1
        a->b1  = b;                     ///+1 =2
        std::cout << "a->b1 = b b.use_count()=" << b.use_count() << std::endl;
        b->a1 = a; ///+1 =2
        std::cout << "b->a1 = a a.use_count()=" << a.use_count() << std::endl;
    }
    /// a 出作用域 a.use_count - 1 = 1 a资源不释放 a.b1不释放
    /// ?  b.use_count = 2
    /// b 出作用域 b.use_count - 1 = 1 b资源不释放 b.a1 不释放
    /// ? b.use_count = 1 a.use_count = 1
    // 循环引用
    std::cout << "after AB}" << std::endl;


    {
        auto a = std::make_shared<A>(); /// =1
        auto b = std::make_shared<B>(); /// =1
        a->b2  = b;                     /// =1
        a->Do(); /// 一定保证 a->b1 = b b.use_count()=1
        std::cout << "a->b1 = b b.use_count()=" << b.use_count() << std::endl;
        b->a1 = a; /// +1 =2
        std::cout << "b->a1 = a a.use_count()=" << a.use_count() << std::endl;
    }

    getchar();
    return 0;
}
