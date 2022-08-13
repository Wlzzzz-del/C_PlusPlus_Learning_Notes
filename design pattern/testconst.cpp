#include <iostream>
class Number{
public:
Number(int _img, int _real):
img(_img),real(_real){
}
int get_img()const{
    return img;
}
int get_real()const{
    return real;
}

private:
    int img;
    int real;
};
int main(){
    const Number n(1,2);
    n.get_img();
    n.get_real();
    return 0;
}

