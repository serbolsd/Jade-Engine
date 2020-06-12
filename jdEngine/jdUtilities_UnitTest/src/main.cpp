#define GTEST_HAS_TR1_TUPLE 0
#define GTEST_USE_OWN_TR1_TUPLE 0
 
#include <gtest/gtest.h>
#include <jdPrerequisitesUtil.h>
#include <jdVector2.h>
#include <jdPoint.h>

using namespace jdEngineSDK;

int
main(int argc, char** argv) {
  printf("Running main() from %s\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


TEST(jdUtilities, JDVector2_Test) {
  // ARRENGE - Vector normal initialization 
  JDVector2 A(1.0f, 2.0f);
  // ARRENGE - Vector initialization from vector
  JDVector2 initFromC(4.0f, 5.0f);
  JDVector2 B(initFromC);
  // ARRENGE - General Vectors
  JDVector2 vector1(3.0f, 4.0f);
  JDVector2 vector2(6.0f, 8.0f);

  // ACT - Addition
  JDVector2 C = vector1 + vector2;
  // ACT - Subtraction
  JDVector2 D = vector1 - vector2;
  // ACT - Multiplication
  JDVector2 E = vector1 * 5;
  // ACT - Division
  JDVector2 F = vector2 / 2;
  // ACT - Vector Dot Product
  float dotProduct1 = vector1.dot(vector2);
  JDVector2 mulProduct = vector1 * vector2;
  // ACT - Vector Cross Product
  float Vector1CrossVector2 = vector1.cross(vector2);
  // ACT - Vector magnitude
  float Magnitude = vector1.magnitude();
  // ACT - Vector normalized
  vector1.normalize();

  // ASSERT - Vector normal initialization 
  GTEST_ASSERT_EQ(A.x, 1.0f);
  GTEST_ASSERT_EQ(A.y, 2.0f);
  // ASSERT - Vector initialization from vector
  GTEST_ASSERT_EQ(B.x, 4.0f);
  GTEST_ASSERT_EQ(B.y, 5.0f);
  // ASSERT - Vector Addition
  GTEST_ASSERT_EQ(C.x, 9.0f);
  GTEST_ASSERT_EQ(C.y, 12.0f);
  // ASSERT - Vector subtraction
  GTEST_ASSERT_EQ(D.x, -3.0f);
  GTEST_ASSERT_EQ(D.y, -4.0f);
  // ASSERT - Vector Multiplication
  GTEST_ASSERT_EQ(E.x, 15.0f);
  GTEST_ASSERT_EQ(E.y, 20.0f);
  // ASSERT - Vector Division
  GTEST_ASSERT_EQ(F.x, 3.0f);
  GTEST_ASSERT_EQ(F.y, 4.0f);
  // ASSERT - Vector Dot Product
  GTEST_ASSERT_EQ(dotProduct1, 50.0f);
  // ASSERT - Vector Cross Product
  GTEST_ASSERT_EQ(Vector1CrossVector2, 0.0f);
  // ASSERT - Vector magnitude
  GTEST_ASSERT_EQ(Magnitude, 5.0f);
  // ASSERT - Vector
  GTEST_ASSERT_EQ(vector1.x, 0.6f);
  GTEST_ASSERT_EQ(vector1.y, 0.8f);
}

TEST(jdUtilities, JDPoint_Test) {
  // ARRENGE - Vector normal initialization 
  JDPoint A(2, 2);
  // ARRENGE - Vector initialization from vector
  JDPoint coC(4, 5);
  JDPoint B(coC);
  // ARRENGE - General Vectors
  JDPoint vector1(3, 4);
  JDPoint vector2(6, 8);

  // ACT - Addition
  JDPoint C = vector1 + vector2;
  // ACT - Subtraction
  JDPoint D = vector1 - vector2;
  // ACT - Multiplication
  JDPoint E = vector1 * 5;
  // ACT - Division
  JDPoint F = vector2 / 2;
  // ACT - Vector Dot Product
  int32 dotProduct1 = vector1.dot(vector2);
  // ACT - Vector Cross Product
  int32 Vector1CrossVector2 = vector1.cross(vector2);

  // ASSERT - Vector normal initialization 
  GTEST_ASSERT_EQ(A.x, 2);
  GTEST_ASSERT_EQ(A.y, 2);
  // ASSERT - Vector initialization from vector
  GTEST_ASSERT_EQ(B.x, 4);
  GTEST_ASSERT_EQ(B.y, 5);
  // ASSERT - Vector Addition
  GTEST_ASSERT_EQ(C.x, 9);
  GTEST_ASSERT_EQ(C.y, 12);
  // ASSERT - Vector subtraction
  GTEST_ASSERT_EQ(D.x, -3);
  GTEST_ASSERT_EQ(D.y, -4);
  // ASSERT - Vector Multiplication
  GTEST_ASSERT_EQ(E.x, 15);
  GTEST_ASSERT_EQ(E.y, 20);
  // ASSERT - Vector Division
  GTEST_ASSERT_EQ(F.x, 3);
  GTEST_ASSERT_EQ(F.y, 4);
  // ASSERT - Vector Dot Product
  GTEST_ASSERT_EQ(dotProduct1, 50);
  // ASSERT - Vector Cross Product
  GTEST_ASSERT_EQ(Vector1CrossVector2, 0);
  // ASSERT - Vector
  GTEST_ASSERT_EQ(vector1.x, 3);
  GTEST_ASSERT_EQ(vector1.y, 4);
}