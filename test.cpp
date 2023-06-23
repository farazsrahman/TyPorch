#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>
#include <math.h>

#include "matrix.h"
#include "matrix.cpp"
#include "activation.h"
#include "activation.cpp"

using namespace std;

int main() {

    Matrix test(3, 3);
    

    return 0;
}

// int main(){

//     // TEST APPLY
//     // Matrix m1(2, 2);

//     // m1.setEntry(0, 0, 4);
//     // m1.setEntry(0, 1, 9);
//     // m1.setEntry(1, 0, 16);
//     // m1.setEntry(1, 1, 25);

//     // m1.print();
    
//     // m1.apply(sqrt).print(); // make sure to #include <math.h>

//     // TEST transpose
//     // Matrix m1(3, 2);

//     // m1.setEntry(0, 0, 2);
//     // m1.setEntry(0, 1, 3);
//     // m1.setEntry(1, 0, 1);
//     // m1.setEntry(1, 1, 5);

//     // m1.setEntry(2, 0, 23);

//     // m1.print();
//     // m1.tranpose().print();



//     // TEST ELEMENTWISEMULTIPLY
//     // Matrix m1(2, 2);
//     // Matrix m2(2, 2);
//     // Matrix m3(3, 3);

//     // m1.setEntry(0, 0, 2);
//     // m2.setEntry(0, 0, 4);

//     // m1.setEntry(1, 1, 3);
//     // m2.setEntry(1, 1, 3);

//     // m1.elementWiseMultiply(m2).print();





//     // TEST SCALAR OPERATOR OVERLOAD
//     // Matrix test(4, 4);

//     // (2*(1+test)).print();


//     // OPERATOR * TEST (MATRIX MULTIPLICATION)

//     // Matrix m1(2, 2);
//     // m1.setEntry(0, 0, 1);
//     // m1.setEntry(0, 1, 2);

//     // m1.setEntry(1, 0, 3);
//     // m1.setEntry(1, 1, 4);

//     // m1.print();


//     // Matrix rotate90(2, 2);

//     // rotate90.setEntry(0, 1, -1);
//     // rotate90.setEntry(1, 0, 1);

//     // rotate90.print();

//     // Matrix rotatem90(2, 2);

//     // rotatem90.setEntry(0, 1, 1);
//     // rotatem90.setEntry(1, 0, -1);

//     // rotatem90.print();


//     // (rotate90 * m1 * rotatem90).print();
//     // (rotate90 * rotatem90 * m1).print();
//     // m1.print();

//     // Matrix m2(3, 3);
//     // Matrix m3(2, 2);
//     // Matrix v(3, 1);

//     // m2.setEntry(0, 0, 1);
//     // m2.setEntry(1, 0, 2);
//     // m2.setEntry(2, 0, 1);

//     // m2.setEntry(0, 1, 3);
//     // m2.setEntry(1, 1, 1);
//     // m2.setEntry(2, 1, 2);

//     // m2.setEntry(0, 2, 1);
//     // m2.setEntry(1, 2, 2);
//     // m2.setEntry(2, 2, 1);

//     // v.setEntry(0, 0, 1);
//     // v.setEntry(1, 0, 2);
//     // v.setEntry(2, 0, -1);

//     // (m2*v).print(); // [6, 2, 4]^T expected

//     // m2*m3; // should throw error and exit

    





//     // OPERATOR +, -, =, == TEST // BE SURE TO RIGOUSOULY TEST THIS LATER

//     // Matrix m1(3, 2);
//     // Matrix m2(3, 2);
//     // Matrix m3(2, 2);
//     // Matrix empty(3, 2);

//     // m1.setEntry(0, 0, 1); // [[1, 0],
//     // m1.setEntry(1, 1, 2); //  [0, 2],
//     // m1.setEntry(2, 0, 3); //  [3, 0]]

//     // m2.setEntry(0, 0, 3); // [[3, 0], 
//     // m2.setEntry(1, 1, 6); //  [0, 6],
//     // m2.setEntry(2, 0, 2); //  [2, 0]]

//     // Matrix result = m1;
//     // result = m2;

//     // result.setEntry(0, 1, 10);
//     // result.print();
//     // m2.print(); // these should be different

//     // (result = m1 + m1 + m1).print();

//     // printf("%d\n\n", empty == (m1 + m1));


//     // double x = 1.0;
//     // double y = 20.0;
//     // printf("%1.3f\n\n", x - y);

//     // Matrix m23 = m2-m3; // should give an error
    

//     // FLATTEN MATRIX TEST
//     // Matrix m1(3, 2);

//     // m1.setEntry(0, 0, 1);
//     // m1.setEntry(1, 0, 2);
//     // m1.setEntry(2, 0, 3);

//     // m1.setEntry(0, 1, 4);
//     // m1.setEntry(1, 1, 5);
//     // m1.setEntry(2, 1, 6);

//     // m1.flattenMatrix().print(); // expected [1, 2, 3, 4, 5, 6]^T

//     // m1.setEntry(0, 0, 1);
//     // m1.setEntry(1, 0, 3);
//     // m1.setEntry(2, 0, 5);

//     // m1.setEntry(0, 1, 2);
//     // m1.setEntry(1, 1, 4);
//     // m1.setEntry(2, 1, 6);

//     // m1.flattenMatrix(0).print(); // expected [1, 2, 3, 4, 5, 6]


//     // ARGMAX TEST

//     // Matrix m1(3, 3);

//     // m1.setEntry(0, 0, 3);
//     // m1.setEntry(1, 0, 2);
//     // m1.setEntry(2, 0, 1);

//     // m1.setEntry(0, 1, 4);
//     // m1.setEntry(1, 1, 6);
//     // m1.setEntry(2, 1, 5);

//     // m1.setEntry(0, 2, 7);
//     // m1.setEntry(1, 2, 8);
//     // m1.setEntry(2, 2, 9);

//     // m1.print();
    

//     // Matrix m1argmax = m1.argmax_matrix();

//     // m1argmax.print(); // :)



//     // SET ENTRY OUT OF BOUNDS TEST

//     // Matrix m1(3, 3);

//     // m1.setEntry(-1, 0, 3);
//     // m1.setEntry(1, 5, 2);
//     // m1.setEntry(2, 0, 1);

//     // m1.print();




//     // SAME DIMS TEST

//     // Matrix m1(1, 4);
//     // Matrix m2(2, 4);
//     // Matrix m3(2, 3);


//     // printf("\n%d\n", m1.sameDims(m2));
//     // printf("%d\n\n", m3.sameDims(m2));


    
// }



#endif