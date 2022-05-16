# shared_ptr
为了保证内存块被使用完之后能及时地被回收，人们经常使用shared_ptr管理内存。shared_ptr会为指向的内存块提供引用计数，当引用计数为0时，会自动回收该块内存。
shared_ptr被包含在<memory>头文件中，使用std命名空间调用。
# 产生的问题
首先我们声明一个test类用做试验对象。
```C++
class test{
    public:
        test():val(0){
            std::cout<<"construct"<<val<<std::endl;
        }
        test(int v):val(v){
            std::cout<<"construct"<<val<<std::endl;
        }
        ~test(){
            std::cout<<"deleted"<<std::endl;
        }
    private: int val;
};
```
当test类被创建时，会往输出流中输出construct信息；当test类被析构时，会往输出流中输出deconstruct信息。
经常地，shared_ptr被这样正确使用：
```C++
void fun(){
    shared<test> ptr(new test);
}
int main(){
    fun();
    return 0;
}
```
控制台输出以下信息：
```
construct
deleted
```
问题是，当ptr被指向动态分配的数组时，由于shared_ptr在析构函数中调用的是 ***delete*** 而非 ***delete[]*** ,这会导致数组仅首个元素被析构，其他元素成了内存垃圾，考虑以下代码。
```C++
void fun(){
    shared<test> ptr(new test[5]);
}
int main(){
    fun();
    return 0;
}
```
代码在堆上尝试分配长度为5的数组，但是在shared_ptr析构时，仅析构了第一个元素。控制台输出如下：
```
construct
construct
construct
construct
construct
deleted
```
# 为shared_ptr指定删除器
为shared_ptr指定删除器有多种方法
1. 使用自带的std::default_delete()
```C++
void fun(){
    std::shared_ptr<test> ptr(new test[5],std::default_delete<test[]>());// default_delete<数组类型>
}
int main(){
    fun();
    return 0;
}
```
2. 自己编写删除器
```C++
void fun(){
    std::shared_ptr<test> ptr(new test[5],[](test* t){
        delete []t;
    });
}
int main(){
    fun();
    return 0;
}
```
# 为shared_ptr指定正确的类型
在新版的C++标准中，允许直接将test[]作为指针的类型。
```C++
void fun(){
    std::shared_ptr<test[]> ptr(new test[5]);
}
int main(){
    fun();
    return 0;
}
```