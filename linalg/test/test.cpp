#include "./../linalg.h"
#include <vector>
#include <cmath>
#include <./gtest/gtest.h> // this shows up as an error but it shouldnt...
                            // avoid clicking here..

using std::cout;
using std::vector;

// SETTERS / GETTERS + IN-PLACE METHODS
TEST(CoordHelpers, testGetIndexOfCoord) {

  Tensor m({2, 2, 2});
  m.fill(0);

  // these shoul essentiallby increasingcdcdcdcdcdcdcdcdcddddddddcdcdaws
  // in binary number system
  EXPECT_EQ(m.getIndexOfCoord({0, 0, 0}), 0);
  EXPECT_EQ(m.getIndexOfCoord({0, 0, 1}), 1);
  EXPECT_EQ(m.getIndexOfCoord({0, 1, 0}), 2);
  EXPECT_EQ(m.getIndexOfCoord({0, 1, 1}), 3);

  EXPECT_EQ(m.getIndexOfCoord({1, 0, 0}), 4);
  EXPECT_EQ(m.getIndexOfCoord({1, 0, 1}), 5);
  EXPECT_EQ(m.getIndexOfCoord({1, 1, 0}), 6);
  EXPECT_EQ(m.getIndexOfCoord({1, 1, 1}), 7);
  
}
TEST(CoordHelpers, testGetCoordOfIndex) {

  Tensor m({2, 2, 2});
  m.fill(0);
  vector<int> expected;

  expected = {0, 0, 0};
  EXPECT_EQ(m.getCoordOfIndex(0), expected);
  expected = {0, 0, 1};
  EXPECT_EQ(m.getCoordOfIndex(1), expected);
  expected = {0, 1, 0};
  EXPECT_EQ(m.getCoordOfIndex(2), expected);
  expected = {0, 1, 1};
  EXPECT_EQ(m.getCoordOfIndex(3), expected);

  expected = {1, 0, 0};
  EXPECT_EQ(m.getCoordOfIndex(4), expected);
  expected = {1, 0, 1};
  EXPECT_EQ(m.getCoordOfIndex(5), expected);
  expected = {1, 1, 0};
  EXPECT_EQ(m.getCoordOfIndex(6), expected);
  expected = {1, 1, 1};
  EXPECT_EQ(m.getCoordOfIndex(7), expected);
  
}
TEST(GettersAndSetters, testGetThenSet) {
  Tensor m({2, 2, 2});

  for(int i = 0; i < 8; i++) {
    m.setEntry(m.getCoordOfIndex(i), i);
    EXPECT_EQ(m.getEntry(m.getCoordOfIndex(i)), i);
  }
}
TEST(GettersAndSetters, testFill) {
  Tensor m({2, 2, 2});
  m.fill(3);

  for(int i = 0; i < 8; i++) {
    EXPECT_EQ(m.getEntry(m.getCoordOfIndex(i)), 3);
  }
}
TEST(IO, testPrint) {

  Tensor m({2, 2, 2, 2});
  for(int i = 0; i < 16; i++) {
    // testing that we don't get a seg fault
    m.setEntry(m.getCoordOfIndex(i), i);
  }

  // just tested these manually, and made sure 
  // they looked alright
  // m.print();

}

// EQUALITY OPERATORS
TEST(EqualityOperators, testSameShape) {
  Tensor a({2, 2});
  Tensor b({2, 2});
  Tensor c({2, 2, 2});

  EXPECT_TRUE(a.sameShape(b));
  EXPECT_FALSE(a.sameShape(c));
  
}
TEST(EqualityOperators, testStructuralEquality) {

  
  Tensor a({2, 2}); a.fill(1);
  Tensor b({2, 2}); 
  Tensor c({2, 2}); 

  for(int i = 0; i < b.getSize(); i++) {
    b.setEntry(b.getCoordOfIndex(i), i);
    c.setEntry(c.getCoordOfIndex(i), i);
  }

  EXPECT_TRUE(b == c);
  EXPECT_FALSE(b != c);

  EXPECT_TRUE(a != b);
  EXPECT_FALSE(a == b);

  b.setEntry({0, 0}, 678);
  EXPECT_TRUE(b != c);
  EXPECT_FALSE(b == c);

}

// BINARY OPERATORS - TENSOR
TEST(BinaryTensorOperators, testAdd) {
  
  Tensor a({2, 2});
  Tensor b({2, 2});
  Tensor c({2, 2}); 
  Tensor d({2, 2});
  Tensor e({2, 2});

  for(int i = 0; i < b.getSize(); i++) {
    a.setEntry(c.getCoordOfIndex(i), i);
    b.setEntry(c.getCoordOfIndex(i), i);
    c.setEntry(c.getCoordOfIndex(i), 1);
    d.setEntry(c.getCoordOfIndex(i), i+i);
    e.setEntry(c.getCoordOfIndex(i), i+i+1);
  }

  // Tensor x Tensor
  EXPECT_TRUE(d == a + b);
  EXPECT_TRUE(e != a + b);
  EXPECT_TRUE(e == a + b + c);

  // Tensor x Scalar
  EXPECT_TRUE(e == d + 1);
  EXPECT_TRUE(e == 1 + d);
  
  // Chained operations
  EXPECT_TRUE(e ==  a + 1 + b);

}
TEST(BinaryTensorOperators, testSubtract) {

  Tensor a({2, 2});
  Tensor b({2, 2});
  Tensor c({2, 2}); 
  Tensor d({2, 2});
  Tensor e({2, 2});

  for(int i = 0; i < b.getSize(); i++) {
    a.setEntry(c.getCoordOfIndex(i), i);
    b.setEntry(c.getCoordOfIndex(i), i);
    c.setEntry(c.getCoordOfIndex(i), 1);
    d.setEntry(c.getCoordOfIndex(i), i+i);
    e.setEntry(c.getCoordOfIndex(i), i+i+1);
  }

  // Tensor x Tensor
  EXPECT_TRUE(d - a == b);
  EXPECT_TRUE(e - a != b);
  EXPECT_TRUE(e - a - b == c);

  // Tensor x Scalar
  EXPECT_TRUE(e - 1 == d);
  
  // Chained operations
  EXPECT_TRUE(c ==  a - b + 1);
  

}
TEST(BinaryTensorOperators, testMultiply) {

  Tensor a({2, 2});
  Tensor b({2, 2});
  Tensor c({2, 2}); 
  Tensor d({2, 2});
  Tensor e({2, 2});

  for(int i = 0; i < b.getSize(); i++) {
    a.setEntry(c.getCoordOfIndex(i), i);
    b.setEntry(c.getCoordOfIndex(i), i);
    c.setEntry(c.getCoordOfIndex(i), 2);
    d.setEntry(c.getCoordOfIndex(i), i*i);
    e.setEntry(c.getCoordOfIndex(i), i*i*2);
  }

  // Tensor x Tensor
  EXPECT_TRUE(d == a * b);
  EXPECT_TRUE(e != a * b);
  EXPECT_TRUE(e == a * b * c);

  // Tensor x Scalar
  EXPECT_TRUE(e == 2 * d);
  EXPECT_TRUE(e == d * 2);
  EXPECT_TRUE(d == d * 1);
  
  // Chained operations
  EXPECT_TRUE(e ==  a * 2 * b);

}
TEST(BinaryTensorOperators, testDivide) {

  Tensor a({2, 2});
  Tensor b({2, 2});
  Tensor c({2, 2}); 
  Tensor d({2, 2});
  Tensor e({2, 2});
  Tensor z({2, 2}); // all 1s but 0 in in top left

  for(int i = 0; i < b.getSize(); i++) {
    a.setEntry(c.getCoordOfIndex(i), (i+1));
    b.setEntry(c.getCoordOfIndex(i), (i+1));
    c.setEntry(c.getCoordOfIndex(i), 2);
    d.setEntry(c.getCoordOfIndex(i), (i+1)*(i+1));
    e.setEntry(c.getCoordOfIndex(i), (i+1)*(i+1)*2);
  }
  z.fill(1);
  z.setEntry({0, 0}, 0);

  // Tensor x Tensor
  EXPECT_TRUE(d / a == b);
  EXPECT_TRUE(e / a != b);
  EXPECT_TRUE(e / a / b  == c);
  EXPECT_TRUE((a-1) / (b-1) == z); // testing zero approx

  // Tensor x Scalar
  EXPECT_TRUE(e / 2 == d);
  EXPECT_TRUE(e / 2 / a == b);
    

}
double sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}
TEST(BinaryTensorOperators, testApply) {

  Tensor a({2, 2});
  Tensor b({2, 2});
  Tensor c({2, 2}); 


  for(int i = 0; i < b.getSize(); i++) {
    a.setEntry(c.getCoordOfIndex(i), i);
    b.setEntry(c.getCoordOfIndex(i), i*i / 2.0);
  }
  // factorial
  c.setEntry({0, 0}, 1);
  c.setEntry({0, 1}, 1);
  c.setEntry({1, 0}, 2);
  c.setEntry({1, 1}, 6);

  // math expression
  EXPECT_TRUE(a.apply([](double v){ return v*v/2.0; }) == b);
  
  EXPECT_TRUE(
    a.apply(
      [](double v) {
        int n = static_cast<int>(v);
        int ans = 1;
        while(n > 0) {
          ans *= n;
          n--;
        }
        return ans*1.0;
      }) 
    == c);

  Tensor d({1});
  Tensor e({1});
  d.setEntries({2.3});
  e.setEntries({sigmoid(2.3)});
  EXPECT_TRUE(e == d.apply(sigmoid));

}

// ENCAPSULATION
TEST(EncapsulationTests, testCopyConstructorAndAssignmentOperator) {

  Tensor a({2, 2, 2}); a.fill(1);
  Tensor b(a);
  Tensor c({2, 2, 2}); c = a;

  EXPECT_TRUE(a == b);
  EXPECT_TRUE(a == c);

  b.setEntry({0, 0, 0}, 2);
  EXPECT_TRUE(a != b);

  c.setEntry({0, 0, 0}, 3);
  EXPECT_TRUE(a != c);

}
TEST(EncapsulationTests, testGetEntries) {

  Tensor t({2});
  t.setEntries({1, 2});

  t.getEntries()[1] = 4;

  EXPECT_TRUE(t.getEntries()[1] == 2);

}


// MATRIX TESTS
TEST(MatrixTests, testMatrixMultiply) {
  Matrix a(2, 2);
  Matrix b(2, 2);
  Matrix c(2, 2);
  Matrix d(3, 3);

  for(int i = 0; i < 4; i++) {
    a.setEntry(a.getCoordOfIndex(i), i);
    b.setEntry(b.getCoordOfIndex(i), i-1);
  }

  c.setEntry(c.getCoordOfIndex(0), 1);
  c.setEntry(c.getCoordOfIndex(1), 2);
  c.setEntry(c.getCoordOfIndex(2), 1);
  c.setEntry(c.getCoordOfIndex(3), 6);

  EXPECT_TRUE(a.matMul(b) == c);
  EXPECT_EXIT(a.matMul(d), ::testing::ExitedWithCode(1), ".*");

}
TEST(MatrixTests, testTransposeFail) {

  Matrix a(2, 3);
  EXPECT_EXIT(a.transpose(), ::testing::ExitedWithCode(1), ".*");


}
TEST(MatrixTests, testTranspose) {
  Matrix a(3, 3);
  Matrix b(3, 3);

  int i = 0;
  for(int r = 0; r < 3; r++) {
        for(int c = 0; c < 3; c++) {  
            a.setEntry({r, c}, i);
            b.setEntry({c, r}, i);
            i++;
        }
    }

    EXPECT_TRUE(a.transpose() == b);


}

// FLATTEN TEST
TEST(FlattenTest, testFlatten) {

  Tensor t({3, 3, 3});

  for(int i = 0; i < t.getSize(); i++) {
    t.setEntry(t.getCoordOfIndex(i), i);
  }

  Matrix c = flatten(t, 1);
  Matrix r = flatten(t, 0);


  EXPECT_EQ(c.getDimensionality(), 2);
  EXPECT_EQ(r.getDimensionality(), 2);
  EXPECT_EQ(r.getNumRows(), 1);
  EXPECT_EQ(c.getNumCols(), 1);


for(int i = 0; i < t.getSize(); i++) {
  EXPECT_TRUE(t.getEntry(t.getCoordOfIndex(i)) == 
              c.getEntry(c.getCoordOfIndex(i)));
}

}

// IDK if this constitutes good testing lol but its 2am
TEST(ReshapeTest, testReshape) {

  Tensor t({4, 4, 4}); // ;-;
  t.randomize(1);

  EXPECT_TRUE(t == reshape(t.getShape(), flatten(t)));

}