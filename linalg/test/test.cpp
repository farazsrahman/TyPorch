#include "./../matrix.h"
#include "./../tensor.h"
#include <vector>
#include <./gtest/gtest.h> // this shows up as an error but it shouldnt...
                            // avoid clicking here..

using std::cout;
using std::vector;

// SETTERS / GETTERS + IN-PLACE METHODS
TEST(CoordHelpers, testGetIndexOfCoord) {

  Tensor m({2, 2, 2});
  m.fill(0);

  // these shoul essentiallby increasing
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

  for(int i = 0; i < b.size(); i++) {
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

  for(int i = 0; i < b.size(); i++) {
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

  for(int i = 0; i < b.size(); i++) {
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

  for(int i = 0; i < b.size(); i++) {
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

  for(int i = 0; i < b.size(); i++) {
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
TEST(BinaryTensorOperators, testApply) {

  Tensor a({2, 2});
  Tensor b({2, 2});
  Tensor c({2, 2}); 


  for(int i = 0; i < b.size(); i++) {
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

