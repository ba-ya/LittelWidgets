[TOC]

# C++

#### std::shared_ptr与make_shared

[参考1](https://blog.csdn.net/q2519008/article/details/100560402)

[std::shard_ptr与vtkSmartPoint的区别](./add2/shard_ptr与vtkSmartPoint的区别.md)

```c++
struct A;
std::shared_ptr<A> p1 = std::make_shared<A>();
std::shared_ptr<A> p2(new A);
```

上面两者有什么区别呢？ 区别是：std::shared_ptr构造函数会执行两次内存申请，而std::make_shared则执行一次。

std::shared_ptr在实现的时候使用的refcount技术，因此内部会有一个计数器（控制块，用来管理数据）和一个指针，指向数据。因此在执行`std::shared_ptr<A> p2(new A)`的时候，首先会申请数据的内存，然后申请内控制块，因此是两次内存申请，而`std::make_shared<A>()`则是只执行一次内存申请

`构造函数是保护或私有时,无法使用 make_shared`

`make_shared` 虽好, 但也存在一些问题, 比如, 当我想要创建的对象没有公有的构造函数时, `make_shared` 就无法使用了, 这时可以使用shared_ptr

![image-20240313113315450](..\add\C++.assets\image-20240313113315450.png)

#### c++11[for的新用法](https://en.cppreference.com/w/cpp/language/range-for)

```c++
//在某个范围内执行 for 循环，把后面的全部遍历一遍，不输出也没事
	std::vector<int> v = {0, 1, 2, 3, 4, 5};
    for (auto&& i : v) // access by forwarding reference, the type of i is int&
    std::cout << i << ' ';
    std::cout << '\n';
    for ([[maybe_unused]] int n : a)  
        std::cout << 1 << ' '; // the loop variable need not be used
    std::cout << '\n';
    for (auto n = v.size(); auto i : v) // the init-statement (C++20)
        std::cout << --n + i << ' ';
    std::cout << '\n';
 
//输出
//0 1 2 3 4 5 
//1 1 1 1 1 1
//5 5 5 5 5 5
/*（n = 6 ,i = 0
      5 + 0
      4 + 1
      3 + 2
      2 + 3
      1 + 4
      0 + 5
）*/
```

#### ~~&a与&&a右值引用~~



