#include "./../matrix.h"
#include "./../tensor.h"

#include <./gtest/gtest.h> # this shows up as an error but it shouldnt...

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}


// using std::cout;

// int main() {


//     Matrix test(3, 3);
//     test.print();

//     cout << "\n\ntest ran succesfully \n";

//     return 0;
// }