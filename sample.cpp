#include <bits/stdc++.h>
#include "rtx.h"

class obj{
public:
    //nothing
    virtual int trace()=0;
    virtual obj* get_pt() = 0;
};

class concA : public obj {
    int x;
public:
    concA(int w): x(w) {}
    concA(int decine,int unita){ x = 10*decine+(unita%10);}
    int trace() {return x;}
    obj* get_pt() {return this;}
};

class concB : public obj {
    int x;
public:
    concB(int w): x(w) {}
    int trace() {return __builtin_popcount(x);}
    obj* get_pt() {return x&1?this:nullptr;}
};

template<typename T>
class decA : public obj {
static_assert(std::is_base_of<obj, T>::value, "T must derive from obj");

T internal;
int add;
public:
template <typename... Ts>
decA(int a,Ts&&... args) : internal(std::forward<Ts>(args)...),add(a) {}

int trace() {return internal.trace()+add;}
obj* get_pt() {return internal.get_pt();}

};

template<typename T>
class decB : public obj {
static_assert(std::is_base_of<obj, T>::value, "T must derive from obj");

T internal;
int mul;
public:
template <typename... Ts>
decB(int m,Ts&&... args) : internal(std::forward<Ts>(args)...),mul(m) {}

int trace() {return internal.trace()*mul;}
obj* get_pt() {return internal.get_pt();}

};



int main() {
    std::vector<obj*> s;
    s.push_back(new concA(3));
    s.push_back(new decA<concA>(10,666));
    s.push_back(new decB<decA<concB>>(2,1,7));
    s.push_back(new decB<concB>(2,6));
    s.push_back(new decB<concB>(2,6));
    for(obj* p:s)std::cout<<p<<"  "<<p->get_pt()<<std::endl;
    volatile int x=0;
    
}
