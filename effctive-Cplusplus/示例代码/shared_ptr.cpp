// shared_ptr 示例代码
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Solution {
public:
    std::string optimalDivision(std::vector<int>& nums) {
        int len = nums.size();
        if(len == 1)return std::to_string(nums[0]);
        if(len==2)return std::to_string(nums[0])+"/"+std::to_string(nums[1]);
        std::string str;
        for(int i=0;i<len; i++)
        {
            if(i == 0)
            {
                str+=(std::to_string(nums[i])+"/(");
                continue;
            }
            if(i == len-1)
            {
                str += (std::to_string(nums[i])+")");
            }
            str+=(std::to_string(nums[i])+"/");
        }
        return str;
    }
};

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
        int get_val(){return val;}
        operator int()const{return val;}
    private: int val;
};

int func(test* t){
    std::cout<<t->get_val()<<std::endl;
    return t->get_val();
}
int fff(int t){
    return t;
}
int main(){
    std::shared_ptr<test> ptr(new test(1));
    std::cout<<"successful convert"<<fff<<std::endl;
    system("pause");
    return 0;
}