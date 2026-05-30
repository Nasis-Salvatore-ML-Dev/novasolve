#include<iostream>
#include<iomanip>
#include<string>

#include "../include/novasolve/Vector.h"


int main(){

    Vector v1(4);
    v1[0]=1;v1[1]=8;
    v1[2]=4;v1[3]=6;
    
    Vector v2(4);
    v2[0]=3;v2[1]=4;
    v2[2]=5;v2[3]=7;

    Vector w(4);
    Vector z(4);
    w=v1+v2;
    z=3*w;


    std::cout<<"v1="<<std::endl;
    std::cout<<v1<<std::endl; // plots vector v1 using overloaded operator
    
    std::cout<<"v2="<<std::endl;
    std::cout<<v2<<std::endl; // plots vector v2 using overloaded operator

    std::cout<<"w = v1 + v2:"<<std::endl;
    w.to_string(); // plots vector w using to_string()function
    std::cout << w.to_string() << std::endl;


    return 0;
}