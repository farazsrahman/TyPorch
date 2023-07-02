#include "./../matrix.h"
#include "./../tensor.h"
#include <vector>
#include <./gtest/gtest.h> // this shows up as an error but it shouldnt...
                            // avoid clicking here..

using std::cout;
using std::vector;

// Demonstrate some basic assertions.
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
TEST(GettersAndSetters, test) {

  Tensor m({2, 2, 2});
  vector<int> coord;

  for(int i = 0; i < 8; i++) {

    // testing that we don't get a seg fault
    m.setEntry(m.getCoordOfIndex(i), i);
    EXPECT_EQ(m.getEntry(m.getCoordOfIndex(i)), i);

  }


}
TEST(IO, testPrint) {

  Tensor m({2, 2, 2, 2});
  vector<int> coord;

  // just tested these manually, and made sure 
  // they looked alright

  for(int i = 0; i < 16; i++) {
    // testing that we don't get a seg fault
    m.setEntry(m.getCoordOfIndex(i), i);
  }

  m.print();

}

