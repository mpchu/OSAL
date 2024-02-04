#include <chrono>
#include <iostream>
#include <osal/task>
#include <osal/semaphore>

using namespace std::chrono_literals;

void f1(int n)
{
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Task 1 executing\n";
        ++n;
        osal::this_task::sleep_for(100ms);
    }
}

void f2(int& n)
{
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Task 2 executing\n";
        ++n;
        osal::this_task::sleep_for(1200ms);
    }
}

class foo
{
public:
    void bar()
    {
        for (int i = 0; i < 5; ++i)
        {
            std::cout << "Task 3 executing\n";
            ++n;
            osal::this_task::sleep_for(1000ms);
        }
    }
    int n = 0;
};

class baz
{
public:
    void operator()()
    {
        for (int i = 0; i < 5; ++i)
        {
            std::cout << "Task 4 executing\n";
            ++n;
            osal::this_task::sleep_for(500ms);
        }
    }
    int n = 0;
};

void osal_task_example()
{
    int n = 0;
    foo f;
    baz b;

    osal::task t1(
        osal::task_attributes()
            .set_name("f1")
            .set_priority(2)
            .set_stack_size(1024),
        f1, n + 1); // pass by value

    osal::task t2(
        osal::task_attributes()
            .set_name("f2")
            .set_priority(2)
            .set_stack_size(1024),
        f2, std::ref(n)); // pass by reference

    osal::task t3(
        osal::task_attributes()
            .set_name("foo::bar")
            .set_priority(2)
            .set_stack_size(1024),
        &foo::bar, &f); // t3 runs foo::bar() on object f

    osal::task t4(
        osal::task_attributes()
            .set_name("baz")
            .set_priority(2)
            .set_stack_size(1024),
        b); // t4 runs baz::operator() on a copy of object b

    osal::this_task::sleep_for(10ms); // Block the init task so that the underlying OS can create the new tasks

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "Final value of n is " << n << '\n';
    std::cout << "Final value of f.n (foo::n) is " << f.n << '\n';
    std::cout << "Final value of b.n (baz::n) is " << b.n << '\n';
}
