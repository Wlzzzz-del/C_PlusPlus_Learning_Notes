# 习惯C++
## 条款01 将C++视为语言联邦
C++ 由C语言 + oop + STL + Template编程组成.

## 条款02 尽量以const、enum、inline代替#define

## 条款03 尽可能使用const

## 条款04 :确定对象被使用前就已经被初始化
### 为内置型对象进行手工初始化，因为C++不保证初始化他们

例如char*，可能会指向未知的内存地址，导致无法设想的结果。

### 构造函数最好使用初始化列表(initial)，而不要在函数体内进行赋值操作(assignment).且初值列出的成员变量，其排序要和在class中首次声明的次序相同。

***由于后面声明的成员变量可能依赖于前面声明的成员变量，因此需要按照顺序来初始化。***
初始化列表对成员变量进行的操作是初始化，但在函数体内进行赋值就浪费了初始化阶段，先默认初始化然后进行赋值，效率较低。

### 为免除"跨编译单元之初始化次序问题", 建议以local static对象代替non-local static对象
non-local static对象是指在单个编译单元内的全局静态对象。
当另一个编译单元include了该编译单元，并且，试图利用该静态对象声明另一个全局静态对象。
那么问题来了，由于后者的初始化需要前者的初始化，但**C++无法保证前者会被优先初始化**，因此会产生错误！
如何解决这个问题？
将该静态对象声明放在一个函数体内，该函数返回一个该对象的引用。每次试图使用时，调用该函数！
```C++
class FileSystem
{
    FileSystem& tfs()
    {
        static Filesystem fs;
        return fs;
    }
}
class Direction{
    public:
    Direction()
    {
        size_t size = tfs().size();// 调用静态函数以分配对象
    }
    Direction& dir()
    {
        static Direction dd;
        return dd;
    }
}
```

## 条款05 了解C++默认写了哪些函数
编译器暗自为class创建default构造函数、copy构造函数、copy assignment操作符，以及析构函数

## 条款06 若不想明确使用编译器自动生成的函数，就该明确拒绝
为驳回编译器自动提供的机能，可以将相应的成员函数声明为private并且不予实现。
```C++
class FileSystem
{
    private:
    ~FileSystem();
};
FileSystem:: ~FileSystem(){};
```

## 条款07 为多态基类声明virtual析构函数
当一个基类指针指向派生类的对象，调用delete删除这一对象时会发生什么？  
很明显，该派生类对象的基类部分会被析构，但是派生部分不会被析构，即会造成内存泄漏！  
如何避免这一问题产生？
将基类的函数声明为virtual，如此之后调用基类指针指向的派生类对象的方法会使用派生类中的方法。

### virtual在底层是如何实现的？
如果一个方法被声明为virtual，那么在该类中会有一个vptr指针，指向内存中的某块内存。在该内存块中存放着
vtable，vtable存放多个函数指针，当一个类调用virtual函数时，会查询vtable调用对应的派生类函数。

### 但也不是将所有的方法声明为virtual
如此做会使得类就像是从未声明过virtual一样有错误。  
因此比较好的做法是：只有当该函数中含有一个virtual函数，才将析构函数声明为virtual。

classes的设计目的如果不是作为base classes使用，如果不是为了具备多态性，就不该声明virtual函数。


## 条款08： 别让异常逃离析构函数
当析构函数可能出现异常时，应该编写一个新的函数接口，该接口包含可能产生异常的操作。在析构函数内调用该接口。
如果在析构函数内出现异常，会导致内存泄漏（例如一个vector数组未被完全释放）等问题。
所以记住两点原则
1. 析构函数不要产生异常。如果一个被析构函数调用的函数可能产生异常，应该使用catch捕捉他，吞下或者记录这一异常。
2. 为可能产生异常的操作编写一个新的接口

## 条款10： Operator=的常规协议
operator=通常返回自身的引用
```C++
class widget{
    public:
    widget& operator=(const widget& rhs){
        /// some codes
        return *this;
    }
}
```

## 条款11: operator=需处理自身引用问题

### 自我赋值安全性
考虑一下代码，Widget类内有一个用于管理Bitmap类的指针。Widget类用于管理Bitmap类的资源。
```C++
class Bitmap{};
class Widget{
    private:Bitmap* bt;
};
```
接下来为Widget类重载运算符等号。该写法表面上看合理，但是自我赋值操作不安全。
```C++
class Widget{
    Widget& operator=(Widget& rhs){
        delete bt;
        bt = new Bitmap(*rhs.bt);
        return *this;// 条款10内容
    }
};
```
若两个Widget类进行自我赋值操作，会造成bt指针指向未被成功初始化的Bitmap类，因为此处的delete删除的不仅是自身的
bt，而且是变量rhs的bt。
***自我赋值安全性指的就是类自我赋值操作是否安全。***  
该运算符函数可以通过证同测试过滤掉自我赋值操作.如以下代码：
```C++
class Widget{
    Widget& operator=(Widget& rhs){
        if(rhs == *this){return *this;}// 证同测试
        delete bt;
        bt = new Bitmap(*rhs.bt);
        return *this;// 条款10内容
    }
};
```
### 异常安全性
异常安全性是指当new操作发生异常时，代码能否检测出来。否则指针将指向一块未被初始化的内存。
实际上，在处理异常安全性时，自我赋值安全性往往也会被一起处理。
```C++
Widget& Widget::operator=(const Widget& rhs){
    Bitmap* pOrig = pb;
    pb = new Bitmap(*rhs.bt);
    delete pOrig;
    return *this;
}
```
如果***new Bitmap(*rhs.bt)*** 发生异常，pb还是等于原来的pb。
但是在之前的代码中，如果new操作发生异常，pb可能指向未知的内存块。
同时该代码还能很好地保证自我赋值安全性。

### 利用swap and copy技术
另一个解决异常安全性的方案是采用swap and copy技术。
```C++
class Widget{
    void swap(Widget& rhs){}// 交换rhs和*this的值
    Widget& operator=(Widget& rhs){
        Widget temp(rhs);// 将自身的引用转为一个实体副本
        swap(temp);// 数据交换
        return *this;
    }
};
```
```C++
class Widget{
    void swap(Widget& rhs){}
    Widget& operator=(Widget rhs){// 注意这边不是引用，直接创造了另一个临时的实体副本
        swap(rhs);
        return *this;
    }
}
```

## 条款12：复制对象时不要忘记每个成分
条款5中明确地写到，编译器会为每个类创建默认的引用赋值函数、拷贝构造函数。当程序员想要自定义这两类函数时，编译器创建的
默认函数便会失效。  
***然而，当程序员为该类添加新的成员变量时，必须记得更新引用赋值函数和拷贝构造函数。***
因为编译器不会提醒你忘记了在这些函数中为新的成员变量赋值。
更进一步的，当该类发生继承行为时，基类中的成员变量也是需要被赋值的。
因此:
+ Copying函数应该确保复制“对象内的所有成员变量”及所有基类成分
+ 不要尝试以某个Copying函数调用另一个Copying函数，应该将其共同的语句放在另一个函数中，一起调用这个函数,不仅能够使程序更加简洁明了，也能够在后期增加新的成员变量时更加方便。

