```C++
void test_string(string& str){
    cout<<str<<endl;
};
int main(){
    test_string("abcd");
}
```
test_string的参数为引用参数，但是在main函数中却为其传入了常量"abcd"，
编译器将无法通过这段代码，报错如下：
```C++
error: cannot bind non-const lvalue reference of type 'std::string&' {aka 'std::__cxx11::basic_string<char>&'} to an rvalue of type 'std::string' {aka 'std::__cxx11::basic_string<char>'} 
```
这是因为test_string的参数类型的引用类型的字符串参数，这时候是引用传递，而传递的时候却没有变量(仔细想，引用传递的是地址，但是常量"abcd"只有常量地址，需要声明const才能将常量地址置于常量区)。
因此如需要这样传递，则将声明改为const 引用类型即
```C++
void test_string(const string& str){
    cout<<str<<endl;
}
```