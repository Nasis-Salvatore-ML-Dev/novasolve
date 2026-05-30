#include<iostream>
#include<iomanip>
#include<string>
#include "../include/novasolve/Matrix.h"
#include "../include/novasolve/Vector.h"
#include "../include/novasolve/MatrixGen.h"


int main(){

    
    // 5x5 symmetric Matrix
    Matrix A=MatrixGen::make_symmetric(5,true,-10,10,42);
    std::cout<<A; // plots A

    return 0;
}