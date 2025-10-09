#include <iostream>


/// 指针与面向对象
///  限制栈中创建对象，限制调用delete销毁对象
class TestMem
{
public:
    static TestMem* Create()
    {
        std::cout << "static Create " << std::endl;
        return new TestMem();
    }
    static void Drop(TestMem* tm)
    {
        std::cout << "static Drop" << std::endl;
        delete tm;
    }

protected:
    TestMem()
    {
        std::cout << "Create TestMem" << std::endl;
    }
    virtual ~TestMem()
    {
        std::cout << "Drop TestMem" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    std::cout << "cppds.com" << std::endl;

    {
        ///  限制栈中创建对象，限制调用delete销毁对象
        // TestMem tm1;
        // auto tm2 = new TestMem;
        TestMem* tm3 = TestMem::Create();
        // delete tm3;
        TestMem::Drop(tm3);
    }
    getchar();
    return 0;
}
