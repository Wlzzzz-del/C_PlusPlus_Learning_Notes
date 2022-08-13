#include <iostream>

class Manager{
    public:
    static Manager& get_Instance();
    void plus();

    private:
    int count;
    Manager();
};

Manager::Manager()
:count(0)
{
}
void Manager::plus(){
    ++count;
    std::cout<<count<<std::endl;
}
Manager& Manager::get_Instance(){
    static Manager mg;
    return mg;
}

int main(){
    Manager::get_Instance().plus();
    Manager::get_Instance().plus();
    system("pause");
    return 0;
}