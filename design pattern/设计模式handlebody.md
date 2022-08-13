# 封装概念
C++的封装是把一个对象的外观接口同实现方式分离开来，但是C++的实现并不完全。编译器必须知道一个对象的所有声明，才能够创建和管理他。
这对于想要封装的部分有两个影响：
+ 程序员不能访问，但能够看到它
+ 造成不必要的重复编译

# Handle-Body模式
handle-body模式是除去实现类，另外声明了一个类作为调用类。
在调用类中有一个指针成员指向一个实现类，调用类的其他成员函数都是对该实现类的方法的调用。
例如，使用handle-body思想重写vector
```C++
class Vector_imp{
    public:
    void push_back(int val);
    void pop_back();
    void clear();
    const size_t size();
};

class Vector{
    public:
    void push_back(int val){
        vec_ptr->push_back(val);
    }
    void pop_back(){
        vec_ptr->pop_back();
    }
    void clear(){
        vec_ptr->clear();
    }
    const size_t size(){
        return vec_ptr->size();
    }
    private:
    Vector_imp* vec_ptr;
}
```
这样子只要接口部分不变，头文件不需要改变，而实现部分可以按需任意改动，完成后只需要对实现部分重新编译即可。

# 缺点
1. 接口类必须把参数传给实现类，这在只有一个析构和构造函数的类不是负担，但是如果是一个大类包含了很多函数，往往增加了出错的可能
2. 对于关注性能，该模型嵌套了两层调用，开销不理想
