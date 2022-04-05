---
title: C++面向对象笔记
date: 2022-01-11 08:33:42
tags: C++
---

# NEW与Delete
以如下的代码为例
```C++
				int *p = new int;
				int *q = new int[10];
				
				Student *pp = new Student();
				Student *qq = new Student[10];
				// 假设一个Student对象占用16字节
```
new首先分配一块地址空间，然后**调用构造函数**,并返回一个指针，该指针指向地址空间的首地址。   
为了记录不同的空间，C++在后台维护一张表，记录了new的对象的首地址和长度。
在执行完上述代码以后，表中的表项为
| 首地址 | 长度 |
| ---    | ---  |
| &p     | 4B   |
| &q     | 40B  |
| &pp    | 16B  |
| &qq    | 160B |

考虑以下代码，通过delete关键字来释放空间
```C++
delete p;

delete[] q;

delete pp;

delete[] qq;
// delete qq;
```
简单地记一个原则：***如果new的时候带[ ],那么delete的时候也要带[ ].***   
delete做的事情，是调用对象的析构函数(不同表项对应的析构函数在编译时就会依靠编译器来写入)，然后将所占用的空间释放掉，并从table中删除表项。    
## 如果new的时候有带[ ],delete的时候不带[ ]，会产生什么后果？  
以代码段中的qq为例，qq的首个Student对象会被析构，但是后面9个Student对象不会被析构，然后这10个对象占用的地址空间会被清除，同时表项会被删掉。这就有可能产生未被回收的内存空间。
注意: C++不允许创建带参数构造函数类的对象数组，  
对象数组中的类必须带有无参数的默认构造函数(且必须有声明和定义)！

# 关键字Class与Struct的区别
区别仅在与类内成员变量和成员函数的默认访问权限。   
若无指定，**Class**的成员变量和函数默认为**private**   
若无指定，**Struct**的成员变量和函数默认为**public**   
# private的问题
private的私有是对于类与类之间的私有，而同一个类的不同对象的成员变量是可以相互访问的。
```C++
				Class a{
				private:
				   int p;
				public:
				   void read(a *n){
				      if (n !=NULL){
				      cout<< n->p << endl;
				         }
					 }
					 a(int value){
					 this -> p = value;
					 }
					 
				int main(){
				   a aa(10);
					 a bb(20);
					 a* p = &aa;
					 bb.read(p);
					 return 0;
				}
				
```
以上面的代码为例，定义了一个类a，包含有int型成员变量p，其中成员函数read接收一个类a的指针，并返回其成员变量p的值。接下来在main函数中，实例化了两个a对象aa和bb，并且试图通过bb访问aa的private成员变量p，结果是访问成功了。
```C++
			Class b{
			   public:
				   void read(a *n){
				      if (n !=NULL){
				      cout<< n->p << endl;// 编译不通过
				         }
					 }
				 
			};
```
接下来试图定义一个b类做相同的事情，结果是编译不通过。
# 友元对象/函数
若要使b类可以访问a类的私有成员变量（以上面的例子），则可以在a类中声明b类为友元类。
```C++
        Class b;//先给出声明，否则编译会不通过
				Class a{
				friend b;//声明友元类
				private:
				   int p;
				public:
				   void read(a *n){
				      if (n !=NULL){
				      cout<< n->p << endl;
				         }
					 }
					 a(int value){
					 this -> p = value;
					 }
				//接下来再给出Class b的定义。
```

# 初始化列表
第一条，构造函数中的赋值，一定要使用初始化列表进行初始化   
即便是基本数据类型（如int、float等）也要如此。  
第二条，父类的构造函数初始化也必须使用初始化列表。  
如果某个对象的成员变量是另一个类，若不使用初始化列表，则当该对象初始化时容易出错。  

```C++
    class a{
		private:
         int value;
    public:
		     a(value):value(value)// 初始化列表
				 {// 函数体
				 };
				 
		};
```

# 继承
一个类继承自另一个类时，如b继承自a，则a的所有public以及protected方法与变量，a都可以访问与使用,但private不可以访问。   
```C++
class A{
public: A(int i);
        A();
				void print();
protected:
        void set();
private:
        int i;
};

class B:public A{//继承的语法，B继承自A, 则B是A的子类
public: void reset(int i){set();}//子类内可以调用父类的protected接口
};

int main(){
B b;
b.set();// 此句编译不通过，子类不能在其他函数直接调用protected
b.reset();// 可以通过
}
```
# 函数重载的隐藏规则
假设父类中有一个方法print()，在该类中还重载了print(int A), 另外有一个子类A继承自该父类，A中有自己的一个方法print()，那么该父类的两个方法不会被继承（即被隐藏了，无法被调用）  
即 子类和父类中出现同名函数，父类的函数被隐藏。  
```C++

class A{
public: A(int i);
        A();
				void print();// 父类不带参数print
				void print(int n);// 父类带参数print
protected:
        void set();
private:
        int i;
};

class B:public A{
public: 
      void reset(int i){set();}
			void print();// 子类不带参数print
       
};

int main(){
   B b;// 实例化一个子类的对象
	 b.print();// 调用子类的print
	 b.print(10);// 错误，父类的两个print被隐藏
	 b.A::print();// 正确，成功调用父类的无参数print
	 b.A::print(10);// 正确，成功调用父类的带参数print
	 
}
```
# 函数定义默认值
1. 函数参数的默认值只能在声明时给出（写在.h文件中）  
2. 带有默认值的参数排在右边
```C++
   void a(int i,int j=0,int h=0);// 正确
	 void b(int i=0, int j, int k);// 错误
```
3. 给参数赋予默认值是编译器进行编译时干的事情,与运行时无关。
4. 建议不使用默认值,会造成阅读困难，且不安全。

# inline函数
## 用法
声明和定义只能放在.h文件。
## 为什么使用内联函数？
在执行主函数main时，当需要调用其他函数时，就必须进行压栈的操作（参数入栈->地址入栈->地址出栈->参数出栈）  
但是有些简单的函数（例如类中用于访问private成员的public接口）这样进行执行就会浪费资源  
因此对这些比较简单的函数，将他们设为内联函数inline 。  
如此做之后，编译器在编译时，不会将其编译成压栈入栈的操作路线，而是直接将所执行的代码直接插入到调用该函数的地方  
## tips
当声明一个类时直接在声明体内定义某些方法，则这些方法会被认为是inline函数(Any function you define inside a class declaration is automatically an inline function.)
## 哪些函数值得使用inline
1. 运算少的小函数，两到三行。
2. 需要被频繁调用的函数，特别是在循环体内的。

## 哪些函数不值得使用inline
1. 非常大的函数，超过20行
2. 递归调用的函数（因为递归本身就需要用到栈)

# const
## const值的使用
```C++
const int class_size =12;
int finalGrad[class_size];// OK

int x;
cin>>x;
const int size = x;
double classAverage[size];// error
```
为什么当const需要cin时不能成为数组长度的变量？  
因为声明一个数组时，编译器需要知道该数组明确的长度，才能够在堆栈区给出足够大的预留空间，该段代码中的x需要cin给出值，因此是不确定的数不能作为数组的长度。
## const指针的含义
```C++ 
char * const q = "abc";// 指针q是const
*q = 'C';// OK,指针所指向的内存块的内容可以改变
q++; // 指针的值不能改变

const char *p = "ABCD";
// (*p) 是一个const的char类型变量
*p = "D";//error
p++;// legal
```
const的含义是指：不能通过该变量改变该内存块的值。  
除非添加extern将变量声明全局变量，否则const变量必须初始化。

## 区分*与const写法
区分的标志是const写在*号的前面还是后面。
```C++
Person p1("Fred",20);
const Person* p = &p1;// const写在*前面，代表对象是const
Person const* p = &p1;
Person *const p = &p1;// const写在*后面，代表指针是const
```


## char指针tips
```C++
const char* s1 = "hello world";
char[] s2 = "hello wordl";

s1[0] = 'b';// error
s2[0] = 'b';// right
```
为什么会有以上的结果？原因在于"hello world"存的位置位于内存中存放代码段的区域，该区域由界地址寄存器所保护。  
s1一个char类型的指针指向了该块内存，但该块内存不能被修改，因此代码s1[0]='b'会报错。  
但s2在堆栈中新建了一个char类型的数组，并且将代码段中的"hello world"字符串内容赋值给堆栈中，由于堆栈是可修改内存，因此代码不会报错。

## 为什么使用指针
传递一整个对象可能会消耗很多内存空间(需要传参，在新的地址空间存放对象)。使用地址指针解决了这一问题，但可能使得程序员修改最初的值。
实际上，当你定义一个指针作为函数的参数时，都应该加上const(使得函数体内无法对该地址的值进行修改).
```C++
int Date::get_day() const{
	day++;// 错误，不能修改
}
```
*在一个成员函数后面声明const，使该成员函数不能对对象的成员变量的值进行修改。*

## 引用(reference)
C++复杂就复杂在提供了太多的内存模型
1. 太多的地方可以放对象————堆栈、堆、全局数据区
2. 太多种访问内存的方式————指针、引用、变量

引用必须在定义时就被初始化。
```C++
int x = 3;
int &y = x;// 可以通过y修改x的值
const int& z = x;// 不能通过z修改x的值
```
## 引用与指针

### 引用:   
1. 不能为NULL
2. 依赖于所指向的变量，可以理解为是变量的别名
3. 一旦初始化就不能再指向新地址   
4. 引用的底层实现就是指针
5. 引用不存在实体，因此不存在类型为引用的数组
### 指针: 
1. 可以被置为NULL
2. 指针独立与现存的对象
3. 即便初始化后也可以指向其他地址


## 向上类型转换(upcasting)
我们可以将基类的*指针*(与内置指针一样，智能指针也支持这一特性)或引用*绑定*到派生类的对象上。  
## 静态类型和动态类型
当我们使用存在继承关系的类型时，必须将一个变量或其他表达式的*静态类型*与该表达式表示对象的*动态类型*区分开。  
例如某个指针是基类类型的指针，而他所指向的是派生类的对象。在这一情况下，该指针的静态类型是基类指针，动态类型的派生类指针.(如果表达式既不是引用也不是指针，则静态类型和动态类型永远一致)
注意的是，之所以能够完成转换，*是因为每个派生类的对象都包含一个基类部分，而基类的引用或指针可以绑定到该基类部分上。*  
同时，*这一类型转换过程只对指针或引用类型有效。*

### 拷贝构造何时被调用
```C++
void p(person p1);// 调用该函数时会调用p1的拷贝构造函数
void p(person& p1);// 调用该函数时候不会调用p1的拷贝构造函数

person p(){
	person q("fred");
	return q;// 此时调用person的拷贝构造
}
```

### 另一种方式的初始化
```C++
int a(2);// 两种初始化相同
int a = 2;
```

### global 全局对象的构造函数何时被调用
当进入程序时，在调用main()函数前会先调用全局对象的构造函数，因此main()函数不再是第一个被调用的函数。
当main()函数执行完后，全局对象的析构函数被调用。


### 声明static意味着Hidden和Persistent
+ Hidden
类内的Hidden关系可以通过public、private关键字保证.
+ Persistent
独立于例子存在，意味着在内存中是持续化存储。
+ class-wide
static成员在类内是被共享的。

### initialize 和 assignment
```C++
Person p = p1;// 初始化在一个变量的生命周期中只有一次
p = p2;// 声明在生命周期中可以有无数次
```

## 运算符重载
+ 只有存在的运算符能够被重载
+ 只能对类或者一个枚举的类型进行运算符重载
+ 保持原有的操作数和运算优先级
```C++
const String String::operator +(const String& that);// 类内重载，自带一个类的this指针。
const String operator +(const String& r, const String& l);// 类外重载，不带有this指针。
```
```C++
Integer x(1),y(5),z;
x + y; // x.operator+(y);
```
以上面代码为例，x + y运算符前面的那个对象x是作为receiver。运算符方法调用的是receiver的方法。

## 运算符重载函数是否应该做成成员函数
tips:
```C++
+ 单元运算符应当被做成成员函数
+ =()[] -> ->* 必须作为成员函数
+ 所有二元运算符应当作成非成员函数
``
### 万能异常捕捉器
```C++
void outer3(){
	try{
		outer2();
	}catch(...){
		// ... means catch all exceptions
		cout<<"The exception stops here";
	}
}
```
### new 和 malloc
+ malloc 在申请空间失败时会返回一个空指针(NULL)
+ new 在申请空间失败时会引起一个bad_alloc()的exception

# 尽量不要在构造函数中抛出异常
```C++
class A{
	public:
	A(){
		//delete this; 在throw0之前需要delete this！
		throw 0;
	}
}

int notmain(){
	A* p = new A;// new 一个A的新对象
	delete p;
	return 0;
}
```
new会做的两件事，首先分配内存，然后调用对象的构造函数，但是在A的构造函数中，构造函数还没完成就抛出了异常。
导致delete无法正常调用析构函数(p无法指向该块分配的内存)。  
出现了无法被找到内存块。

### 什么是流?
流是C++独有的，与此相对的C语言读写文件的时候，文件是以整体的形式(random access)存在，能够找到其中任意的字符。但是通过流读取文件，某个字符一旦错过就无法再读取。
1. 对于外部设备的一维单方向的接口

### creating manipulators
可以定义自己的流操作符用于流中。
```C++
//skeleton for an output stream manipulator
ostream& manip(ostream& out){
	// ...
	return out;// 输出一定要是一个流
}

ostream& tab( ostream& out ){
	return out<<'\t';
}

cout<<"hello "<<tab<<"world"<<endl;
// output: hello
//         world;
```