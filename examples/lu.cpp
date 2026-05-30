#include "../include/novasolve/LU.h"
#include "../include/novasolve/MatrixGen.h"
#include <cmath>
#include <cstddef>

int main(){


    std::cout<<"3x3 Symmetric Matrix, A"<<std::endl;
    std::cout<<std::string(40,'=')<<std::endl;
    
    Matrix A=MatrixGen::make_symmetric(3,true,-10,10);

    std::cout<<"A= "<<A<<std::endl;

    std::cout<<"3x1 Vector, b"<<std::endl;
    std::cout<<std::string(40,'=')<<std::endl;
    Vector b(3);
    b[0]=1.0; b[1]=1.0; b[2]=1.0;
    std::cout<<"b= "<<b<<std::endl;

    std::cout<<"\n\nSolving Ax=b using LU decomposition"<<std::endl;
    std::cout<<std::string(40,'=')<<std::endl;

    Vector x = solve(A, b);

    std::cout<<"\nSolution:"<<std::endl;
    std::cout<<"x= "<<x<<std::endl;

    return 0;
}