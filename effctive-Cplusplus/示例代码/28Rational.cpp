#include <iostream>

class Rational{
    private:
    int fenzi;
    int fenmu;
    public:
    // 为什么？
    // const int rfenzi(){return this->fenzi;}错误
    /// 函数前加const是表示返回的是const类型
    // 类内的非静态成员函数后加const表示传入的this指针是const类型，
    // 在该函数内不能对this指向的对象进行修改
    int rfenzi() const {return this->fenzi;};
    int rfenmu() const {return this->fenmu;};
    const void output(){std::cout<<this->fenzi<<'\\'<<this->fenmu<<std::endl;}
    Rational(int zi=0, int mu=1):fenzi(zi),fenmu(mu){}
/*
    // 为什么是return by value，参考条款21
    const Rational operator*(const Rational& rhs){
        int r1 = rhs.rfenzi();
        int r2 = rhs.rfenmu();
        int l1 = this->rfenzi();
        int l2 = this->rfenmu();
        return Rational(r1*l1,r2*l2);
    }
    // 假设这个有理数类需要一个单个int的operator*方法：
    const Rational operator*(const int& x){
        return Rational(this->rfenzi()*x,this->rfenmu());
    }
*/
};
/// 通过在non-memberfunction定义operator*
// 可以使Rational a*2;或者Rational 2*a;中的2能自动完成隐式转换
// operator*的参数不再需要考虑顺序
const Rational operator*(const Rational& rhs, const Rational& lhs){
    return Rational(rhs.rfenzi()*lhs.rfenzi(),rhs.rfenmu()*rhs.rfenmu());
}

int main(){
    Rational a(1,2);
    Rational b(1,3);
    Rational c=a*b;
    /* Rational d=2*a;//这是错误的调用 */
    /* 因为调用的是2的operator*,而2并没有对应的operator* */

    // 这是可以通过的调用,正确调用Rational的operator*
    Rational d = a*2;
    // d.output();
    // c.output();
    system("pause");
}