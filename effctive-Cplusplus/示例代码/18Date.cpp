// 条款18实例代码
#include <iostream>
class YEAR{
private:
    int val;
public:
    explicit YEAR(int val):val(val){}// 加explicit防止类型转换，比如日可能转化成年？
};

class MONTH{
private:
    int val;
    explicit MONTH(int m):val(m){};
//public:
//    MONTH(int m):val(m){}
public:
    static MONTH Jan(){return MONTH(1);}// 函数，并非类
    static MONTH Feb(){return MONTH(2);}
    /// ……
    static MONTH Dec(){return MONTH(12);}
};

class DAY{
private:
    int val;
public:
    explicit DAY(int val):val(val){}
};

class Date{
public:
    Date(YEAR year, MONTH month, DAY day):year(year),month(month),day(day){}
private:
    YEAR year;
    MONTH month;
    DAY day;
};

int main(){
    Date d(YEAR(1999),MONTH::Dec(),DAY(20));
    system("pause");
    return 0;
}