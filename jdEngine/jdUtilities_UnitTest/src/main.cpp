#define GTEST_HAS_TR1_TUPLE 0
#define GTEST_USE_OWN_TR1_TUPLE 0
 
#include <gtest/gtest.h>
#include <jdPrerequisitesUtil.h>
#include <math.h>
#include <jdVector2.h>
#include <jdVector3.h>
#include <jdVector4.h>
#include <jdPoint.h>
#include <jdRadian.h>
#include <jdDegree.h>
#include <jdMatrix3.h>
#include <jdMatrix4.h>

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

  // ACT - Vectors to compare
  JDVector2 cmp1 = { 0.f,0.f };
  JDVector2 cmp2 = { -1.f, -1.f };
  JDVector2 cmp3 = { 1.f, 1.f };
  JDVector2 cmp4 = { 1.f, 1.f };

  // ASSERT - Vector to compare
  GTEST_ASSERT_EQ(cmp1 < cmp2, false);
  GTEST_ASSERT_EQ(cmp1 > cmp2, true);
  GTEST_ASSERT_EQ(cmp1 != cmp2, true);
  GTEST_ASSERT_EQ(cmp3 != cmp4, false);
  GTEST_ASSERT_EQ(cmp3 == cmp4, true);
}

TEST(jdUtilities, JDPoint_Test) {
  // ARRENGE - Point normal initialization 
  JDPoint A(2, 2);
  // ARRENGE - Point initialization from vector
  JDPoint coC(4, 5);
  JDPoint B(coC);
  // ARRENGE - General Point
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
  // ACT - Point Dot Product
  int32 dotProduct1 = vector1.dot(vector2);
  // ACT - Point Cross Product
  int32 Vector1CrossVector2 = vector1.cross(vector2);

  // ASSERT - Point normal initialization 
  GTEST_ASSERT_EQ(A.x, 2);
  GTEST_ASSERT_EQ(A.y, 2);
  // ASSERT - Point initialization from vector
  GTEST_ASSERT_EQ(B.x, 4);
  GTEST_ASSERT_EQ(B.y, 5);
  // ASSERT - Point Addition
  GTEST_ASSERT_EQ(C.x, 9);
  GTEST_ASSERT_EQ(C.y, 12);
  // ASSERT - Point subtraction
  GTEST_ASSERT_EQ(D.x, -3);
  GTEST_ASSERT_EQ(D.y, -4);
  // ASSERT - Point Multiplication
  GTEST_ASSERT_EQ(E.x, 15);
  GTEST_ASSERT_EQ(E.y, 20);
  // ASSERT - Point Division
  GTEST_ASSERT_EQ(F.x, 3);
  GTEST_ASSERT_EQ(F.y, 4);
  // ASSERT - Point Dot Product
  GTEST_ASSERT_EQ(dotProduct1, 50);
  // ASSERT - Point Cross Product
  GTEST_ASSERT_EQ(Vector1CrossVector2, 0);
  // ASSERT - Point
  GTEST_ASSERT_EQ(vector1.x, 3);
  GTEST_ASSERT_EQ(vector1.y, 4);

  // ACT - Point to compare
  JDPoint cmp1 = { 0,0 };
  JDPoint cmp2 = { -1, -1 };
  JDPoint cmp3 = { 1, 1 };
  JDPoint cmp4 = { 1, 1 };

  // ASSERT - Point to compare
  GTEST_ASSERT_EQ(cmp1 < cmp2, false);
  GTEST_ASSERT_EQ(cmp1 > cmp2, true);
  GTEST_ASSERT_EQ(cmp1 != cmp2, true);
  GTEST_ASSERT_EQ(cmp3 != cmp4, false);
  GTEST_ASSERT_EQ(cmp3 == cmp4, true);
}

TEST(jdUtilities, JDVector3_Test) {
  // ARRENGE - Vector normal initialization 
  JDVector3 A(1.0f, 2.0f, 3.f);
  // ASSERT - Vector normal initialization 
  EXPECT_FLOAT_EQ(A.x, 1.0f);
  EXPECT_FLOAT_EQ(A.y, 2.0f);
  EXPECT_FLOAT_EQ(A.z, 3.f);

  // ARRENGE - Vector initialization from vector
  JDVector3 initFromC(4.0f, 5.0f, 6.f);
  JDVector3 B(initFromC);
  // ASSERT - Vector initialization from vector
  EXPECT_FLOAT_EQ(B.x, 4.0f);
  EXPECT_FLOAT_EQ(B.y, 5.0f);
  EXPECT_FLOAT_EQ(B.z, 6.f);
  
  // ARRENGE - General Vectors
  JDVector3 vector1(3.0f, 4.0f, 5.f);
  JDVector3 vector2(1.0f, 2.0f, 3.f);
  // ACT - Addition
  JDVector3 C = vector1 + vector2;
  // ASSERT - Vector Addition
  EXPECT_FLOAT_EQ(C.x, 4.f);
  EXPECT_FLOAT_EQ(C.y, 6.f);
  EXPECT_FLOAT_EQ(C.z, 8.f);

  // ACT - Subtraction
  JDVector3 D = vector1 - vector2;
  // ASSERT - Vector subtraction
  EXPECT_FLOAT_EQ(D.x, 2.f);
  EXPECT_FLOAT_EQ(D.y, 2.f);
  EXPECT_FLOAT_EQ(D.z, 2.f);

  // ACT - Multiplication
  JDVector3 E = vector1 * 5;
  // ASSERT - Vector Multiplication
  EXPECT_FLOAT_EQ(E.x, 15.0f);
  EXPECT_FLOAT_EQ(E.y, 20.0f);
  EXPECT_FLOAT_EQ(E.z, 25.f);

  // ACT - Division
  JDVector3 F = vector2 / 2;
  // ASSERT - Vector Division
  EXPECT_FLOAT_EQ(F.x, 0.5f);
  EXPECT_FLOAT_EQ(F.y, 1.f);
  EXPECT_FLOAT_EQ(F.z, 1.5f);

  // ACT - Vector Dot Product
  float dotProduct1 = vector1.dot(vector2);
  JDVector3 mulProduct = vector1 * vector2;
  // ASSERT - Vector Dot Product
  EXPECT_FLOAT_EQ(dotProduct1, 26.f);

  // ACT - Vector Cross Product
  JDVector3 Vector1CrossVector2 = vector1.cross(vector2);
  // ASSERT - Vector Cross Product
  EXPECT_FLOAT_EQ(Vector1CrossVector2.x, 2.f);
  EXPECT_FLOAT_EQ(Vector1CrossVector2.y, -4.f);
  EXPECT_FLOAT_EQ(Vector1CrossVector2.z, 2.f);

  // ACT - Vector magnitude
  float Magnitude = vector1.magnitude();
  // ASSERT - Vector magnitude
  EXPECT_FLOAT_EQ(Magnitude, 7.071067812f);

  // ACT - Vector square magnitude
  float SMagnitude = vector1.squareMagnitude();
  // ASSERT - Vector square magnitude
  EXPECT_FLOAT_EQ(SMagnitude, 50.f);

  // ACT - Vector normalized
  JDVector3 normalized = vector1.getnormalize();
  // ASSERT - Vector
  EXPECT_FLOAT_EQ(normalized.x, vector1.x/Magnitude);
  EXPECT_FLOAT_EQ(normalized.y, vector1.y/Magnitude);
  EXPECT_FLOAT_EQ(normalized.z, vector1.z/Magnitude);

  // ACT - Vector normalized
  float distance = vector1.distance(vector2);
  JDVector3 comparativedistance = vector1-vector2;
  float compDistMagnitude = comparativedistance.magnitude();
  // ASSERT - Vector
  EXPECT_FLOAT_EQ(distance, compDistMagnitude);

  // ACT - Vectors to compare
  JDVector3 cmp1 = {0.f,0.f,0.f};
  JDVector3 cmp2 = {-1.f, -1.f, -1.f};
  JDVector3 cmp3 = {1.f, 1.f, 1.f};
  JDVector3 cmp4 = {1.f, 1.f, 1.f};

  // ASSERT - Vector to compare
  GTEST_ASSERT_EQ(cmp1 < cmp2, false);
  GTEST_ASSERT_EQ(cmp1 > cmp2, true);
  GTEST_ASSERT_EQ(cmp1 != cmp2, true);
  GTEST_ASSERT_EQ(cmp3 != cmp4, false);
  GTEST_ASSERT_EQ(cmp3 == cmp4, true);
}

TEST(jdUtilities, JDVector4_Test) {
  // ARRENGE - Vector normal initialization 
  JDVector4 A(1.0f, 2.0f, 3.f);
  // ASSERT - Vector normal initialization 
  EXPECT_FLOAT_EQ(A.x, 1.0f);
  EXPECT_FLOAT_EQ(A.y, 2.0f);
  EXPECT_FLOAT_EQ(A.z, 3.f);
  EXPECT_FLOAT_EQ(A.w, 0.f);

  // ARRENGE - Vector initialization from vector
  JDVector4 initFromC(4.0f, 5.0f, 6.f,1.f);
  JDVector4 B(initFromC);
  // ASSERT - Vector initialization from vector
  EXPECT_FLOAT_EQ(B.x, 4.0f);
  EXPECT_FLOAT_EQ(B.y, 5.0f);
  EXPECT_FLOAT_EQ(B.z, 6.f);
  EXPECT_FLOAT_EQ(B.w, 1.f);

  // ARRENGE - General Vectors
  JDVector4 vector1(3.0f, 4.0f, 5.f, 1.f);
  JDVector4 vector2(1.0f, 2.0f, 3.f, 1.f);
  // ACT - Addition
  JDVector4 C = vector1 + vector2;
  // ASSERT - Vector Addition
  EXPECT_FLOAT_EQ(C.x, 4.f);
  EXPECT_FLOAT_EQ(C.y, 6.f);
  EXPECT_FLOAT_EQ(C.z, 8.f);
  EXPECT_FLOAT_EQ(C.w, 2.f);

  // ACT - Subtraction
  JDVector4 D = vector1 - vector2;
  // ASSERT - Vector subtraction
  EXPECT_FLOAT_EQ(D.x, 2.f);
  EXPECT_FLOAT_EQ(D.y, 2.f);
  EXPECT_FLOAT_EQ(D.z, 2.f);
  EXPECT_FLOAT_EQ(D.w, 0.f);

  // ACT - Multiplication
  JDVector4 E = vector1 * 5;
  // ASSERT - Vector Multiplication
  EXPECT_FLOAT_EQ(E.x, 15.0f);
  EXPECT_FLOAT_EQ(E.y, 20.0f);
  EXPECT_FLOAT_EQ(E.z, 25.f);
  EXPECT_FLOAT_EQ(E.w, 5.f);

  // ACT - Division
  JDVector4 F = vector2 / 2;
  // ASSERT - Vector Division
  EXPECT_FLOAT_EQ(F.x, 0.5f);
  EXPECT_FLOAT_EQ(F.y, 1.f);
  EXPECT_FLOAT_EQ(F.z, 1.5f);
  EXPECT_FLOAT_EQ(F.w, 0.5f);

  // ACT - Vector Dot Product
  float dotProduct1 = vector1.dot(vector2);
  JDVector4 mulProduct = vector1 * vector2;
  // ASSERT - Vector Dot Product
  EXPECT_FLOAT_EQ(dotProduct1, 27.f);

  // ACT - Vector Cross Product
  JDVector4 Vector1CrossVector2 = vector1.cross(vector2);
  // ASSERT - Vector Cross Product
  EXPECT_FLOAT_EQ(Vector1CrossVector2.x, 2.f);
  EXPECT_FLOAT_EQ(Vector1CrossVector2.y, -4.f);
  EXPECT_FLOAT_EQ(Vector1CrossVector2.z, 2.f);
  EXPECT_FLOAT_EQ(Vector1CrossVector2.w, 0.f);

  // ACT - Vector magnitude
  float Magnitude = vector1.magnitude();
  // ASSERT - Vector magnitude
  EXPECT_FLOAT_EQ(Magnitude, 7.141428429f);

  // ACT - Vector square magnitude
  float SMagnitude = vector1.squareMagnitude();
  // ASSERT - Vector square magnitude
  EXPECT_FLOAT_EQ(SMagnitude, 51.f);

  // ACT - Vector normalized
  JDVector4 normalized = vector1.getNormalize();
  // ASSERT - Vector
  EXPECT_FLOAT_EQ(normalized.x, vector1.x / Magnitude);
  EXPECT_FLOAT_EQ(normalized.y, vector1.y / Magnitude);
  EXPECT_FLOAT_EQ(normalized.z, vector1.z / Magnitude);
  EXPECT_FLOAT_EQ(normalized.w, vector1.w / Magnitude);

  // ACT - distance two vectors
  float distance = vector1.distance(vector2);
  JDVector4 comparativedistance = vector1 - vector2;
  float compDistMagnitude = comparativedistance.magnitude();
  // ASSERT - distance two vectors
  EXPECT_FLOAT_EQ(distance, compDistMagnitude);

  // ACT - Vectors to compare
  JDVector4 cmp1 = { 0.f, 0.f, 0.f, 0.f };
  JDVector4 cmp2 = { -1.f, -1.f, -1.f, -1.f };
  JDVector4 cmp3 = { 1.f, 1.f, 1.f, 1.f };
  JDVector4 cmp4 = { 1.f, 1.f, 1.f, 1.f };

  // ASSERT - Vector to compare
  GTEST_ASSERT_EQ(cmp1 < cmp2, false);
  GTEST_ASSERT_EQ(cmp1 > cmp2, true);
  GTEST_ASSERT_EQ(cmp1 != cmp2, true);
  GTEST_ASSERT_EQ(cmp3 != cmp4, false);
  GTEST_ASSERT_EQ(cmp3 == cmp4, true);
}

TEST(jdUtilities, Radian_Degree_Test) {
  Radian r = Radian(1);
  Degree d = Degree(180);
  EXPECT_FLOAT_EQ(r.valueDegrees(), 57.295776f);
  EXPECT_FLOAT_EQ(r.valueRadians(), 1.f);
  EXPECT_FLOAT_EQ(d.valueDegrees(), 180.f);
  EXPECT_FLOAT_EQ(d.valueRadians(), Math::PI);

  GTEST_ASSERT_EQ(r == d, false);
  GTEST_ASSERT_EQ(r != d, true);
  GTEST_ASSERT_EQ(r < d, true);
  GTEST_ASSERT_EQ(r > d, false);

  r = d;

  GTEST_ASSERT_EQ(r == d, true);
  GTEST_ASSERT_EQ(r != d, false);
  GTEST_ASSERT_EQ(r < d, false);
  GTEST_ASSERT_EQ(r > d, false);

  r = 2.f;
  d = 2.f;

  EXPECT_FLOAT_EQ(r.valueRadians(), 2.f);
  EXPECT_FLOAT_EQ(d.valueDegrees(), 2.f);
  EXPECT_FLOAT_EQ(d.valueDegrees() == r.valueRadians(), true);
  EXPECT_FLOAT_EQ(d.valueDegrees() != r.valueRadians(), false);
}

TEST(jdUtilities, JDMatrix3_Test) {
  JDMatrix3 mat;
  JDMatrix3 mat2 = mat;
  // ASSERT - matrix identiti
  EXPECT_FLOAT_EQ(mat.m[0],1.f);
  EXPECT_FLOAT_EQ(mat.m[1],0.f);
  EXPECT_FLOAT_EQ(mat.m[2],0.f);
  EXPECT_FLOAT_EQ(mat.m[3],0.f);
  EXPECT_FLOAT_EQ(mat.m[4],1.f);
  EXPECT_FLOAT_EQ(mat.m[5],0.f);
  EXPECT_FLOAT_EQ(mat.m[6],0.f);
  EXPECT_FLOAT_EQ(mat.m[7],0.f);
  EXPECT_FLOAT_EQ(mat.m[8],1.f);

  // ASSERT - matrix equalization
  EXPECT_FLOAT_EQ(mat[0]==mat2[0], true);
  EXPECT_FLOAT_EQ(mat[1]==mat2[1], true);
  EXPECT_FLOAT_EQ(mat[2]==mat2[2], true);
  EXPECT_FLOAT_EQ(mat[3]==mat2[3], true);
  EXPECT_FLOAT_EQ(mat[4]==mat2[4], true);
  EXPECT_FLOAT_EQ(mat[5]==mat2[5], true);
  EXPECT_FLOAT_EQ(mat[6]==mat2[6], true);
  EXPECT_FLOAT_EQ(mat[7]==mat2[7], true);
  EXPECT_FLOAT_EQ(mat[8]==mat2[8], true);
  
  mat += mat2;
  // ASSERT - Matrix addition
  EXPECT_FLOAT_EQ(mat.m[0], 2.f);
  EXPECT_FLOAT_EQ(mat.m[1], 0.f);
  EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  EXPECT_FLOAT_EQ(mat.m[3], 0.f);
  EXPECT_FLOAT_EQ(mat.m[4], 2.f);
  EXPECT_FLOAT_EQ(mat.m[5], 0.f);
  EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  EXPECT_FLOAT_EQ(mat.m[8], 2.f);

  // ASSERT - Matrix to compare after addition
  EXPECT_FLOAT_EQ(mat[0] == mat2[0], false);
  EXPECT_FLOAT_EQ(mat[1] == mat2[1], true);
  EXPECT_FLOAT_EQ(mat[2] == mat2[2], true);
  EXPECT_FLOAT_EQ(mat[3] == mat2[3], true);
  EXPECT_FLOAT_EQ(mat[4] == mat2[4], false);
  EXPECT_FLOAT_EQ(mat[5] == mat2[5], true);
  EXPECT_FLOAT_EQ(mat[6] == mat2[6], true);
  EXPECT_FLOAT_EQ(mat[7] == mat2[7], true);
  EXPECT_FLOAT_EQ(mat[8] == mat2[8], false);

  EXPECT_FLOAT_EQ(mat[0] != mat2[0], true);
  EXPECT_FLOAT_EQ(mat[1] != mat2[1], false);
  EXPECT_FLOAT_EQ(mat[2] != mat2[2], false);
  EXPECT_FLOAT_EQ(mat[3] != mat2[3], false);
  EXPECT_FLOAT_EQ(mat[4] != mat2[4], true);
  EXPECT_FLOAT_EQ(mat[5] != mat2[5], false);
  EXPECT_FLOAT_EQ(mat[6] != mat2[6], false);
  EXPECT_FLOAT_EQ(mat[7] != mat2[7], false);
  EXPECT_FLOAT_EQ(mat[8] != mat2[8], true);

  mat -= mat2;
  // ASSERT - Matrix subtraction
  EXPECT_FLOAT_EQ(mat.m[0], 1.f);
  EXPECT_FLOAT_EQ(mat.m[1], 0.f);
  EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  EXPECT_FLOAT_EQ(mat.m[3], 0.f);
  EXPECT_FLOAT_EQ(mat.m[4], 1.f);
  EXPECT_FLOAT_EQ(mat.m[5], 0.f);
  EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  EXPECT_FLOAT_EQ(mat.m[8], 1.f);

  mat.scale(5);
  // ASSERT - Matrix Scale
  EXPECT_FLOAT_EQ(mat.m[0], 5.f);
  EXPECT_FLOAT_EQ(mat.m[1], 0.f);
  EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  EXPECT_FLOAT_EQ(mat.m[3], 0.f);
  EXPECT_FLOAT_EQ(mat.m[4], 5.f);
  EXPECT_FLOAT_EQ(mat.m[5], 0.f);
  EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  EXPECT_FLOAT_EQ(mat.m[8], 1.f);

  mat[1]=2;
  // ASSERT - Matrix
  EXPECT_FLOAT_EQ(mat.m[0], 5.f);
  EXPECT_FLOAT_EQ(mat.m[1], 2.f);
  EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  EXPECT_FLOAT_EQ(mat.m[3], 0.f);
  EXPECT_FLOAT_EQ(mat.m[4], 5.f);
  EXPECT_FLOAT_EQ(mat.m[5], 0.f);
  EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  EXPECT_FLOAT_EQ(mat.m[8], 1.f);

  mat.transpose();
  // ASSERT - Matrix Tranpose
  EXPECT_FLOAT_EQ(mat.m[0], 5.f);
  EXPECT_FLOAT_EQ(mat.m[1], 0.f);
  EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  EXPECT_FLOAT_EQ(mat.m[3], 2.f);
  EXPECT_FLOAT_EQ(mat.m[4], 5.f);
  EXPECT_FLOAT_EQ(mat.m[5], 0.f);
  EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  EXPECT_FLOAT_EQ(mat.m[8], 1.f);
}

TEST(jdUtilities, JDMatrix4_Test) {
  JDMatrix4 mat;
  JDMatrix4 mat2 = mat;
  // ASSERT - matrix identiti
  EXPECT_FLOAT_EQ(mat.m[0], 1.f);
  EXPECT_FLOAT_EQ(mat.m[1], 0.f);
  EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  EXPECT_FLOAT_EQ(mat.m[3], 0.f);
  EXPECT_FLOAT_EQ(mat.m[4], 0.f);
  EXPECT_FLOAT_EQ(mat.m[5], 1.f);
  EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  EXPECT_FLOAT_EQ(mat.m[8], 0.f);
  EXPECT_FLOAT_EQ(mat.m[9], 0.f);
  EXPECT_FLOAT_EQ(mat.m[10], 1.f);
  EXPECT_FLOAT_EQ(mat.m[11], 0.f);
  EXPECT_FLOAT_EQ(mat.m[12], 0.f);
  EXPECT_FLOAT_EQ(mat.m[13], 0.f);
  EXPECT_FLOAT_EQ(mat.m[14], 0.f);
  EXPECT_FLOAT_EQ(mat.m[15], 1.f);

  //// ASSERT - matrix equalization
  //EXPECT_FLOAT_EQ(mat[0] == mat2[0], true);
  //EXPECT_FLOAT_EQ(mat[1] == mat2[1], true);
  //EXPECT_FLOAT_EQ(mat[2] == mat2[2], true);
  //EXPECT_FLOAT_EQ(mat[3] == mat2[3], true);
  //EXPECT_FLOAT_EQ(mat[4] == mat2[4], true);
  //EXPECT_FLOAT_EQ(mat[5] == mat2[5], true);
  //EXPECT_FLOAT_EQ(mat[6] == mat2[6], true);
  //EXPECT_FLOAT_EQ(mat[7] == mat2[7], true);
  //EXPECT_FLOAT_EQ(mat[8] == mat2[8], true);
  //
  //mat += mat2;
  //// ASSERT - Matrix addition
  //EXPECT_FLOAT_EQ(mat.m[0], 2.f);
  //EXPECT_FLOAT_EQ(mat.m[1], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[3], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[4], 2.f);
  //EXPECT_FLOAT_EQ(mat.m[5], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[8], 2.f);
  //
  //// ASSERT - Matrix to compare after addition
  //EXPECT_FLOAT_EQ(mat[0] == mat2[0], false);
  //EXPECT_FLOAT_EQ(mat[1] == mat2[1], true);
  //EXPECT_FLOAT_EQ(mat[2] == mat2[2], true);
  //EXPECT_FLOAT_EQ(mat[3] == mat2[3], true);
  //EXPECT_FLOAT_EQ(mat[4] == mat2[4], false);
  //EXPECT_FLOAT_EQ(mat[5] == mat2[5], true);
  //EXPECT_FLOAT_EQ(mat[6] == mat2[6], true);
  //EXPECT_FLOAT_EQ(mat[7] == mat2[7], true);
  //EXPECT_FLOAT_EQ(mat[8] == mat2[8], false);
  //
  //EXPECT_FLOAT_EQ(mat[0] != mat2[0], true);
  //EXPECT_FLOAT_EQ(mat[1] != mat2[1], false);
  //EXPECT_FLOAT_EQ(mat[2] != mat2[2], false);
  //EXPECT_FLOAT_EQ(mat[3] != mat2[3], false);
  //EXPECT_FLOAT_EQ(mat[4] != mat2[4], true);
  //EXPECT_FLOAT_EQ(mat[5] != mat2[5], false);
  //EXPECT_FLOAT_EQ(mat[6] != mat2[6], false);
  //EXPECT_FLOAT_EQ(mat[7] != mat2[7], false);
  //EXPECT_FLOAT_EQ(mat[8] != mat2[8], true);
  //
  //mat -= mat2;
  //// ASSERT - Matrix subtraction
  //EXPECT_FLOAT_EQ(mat.m[0], 1.f);
  //EXPECT_FLOAT_EQ(mat.m[1], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[3], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[4], 1.f);
  //EXPECT_FLOAT_EQ(mat.m[5], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[8], 1.f);
  //
  //mat.scale(5);
  //// ASSERT - Matrix Scale
  //EXPECT_FLOAT_EQ(mat.m[0], 5.f);
  //EXPECT_FLOAT_EQ(mat.m[1], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[3], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[4], 5.f);
  //EXPECT_FLOAT_EQ(mat.m[5], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[8], 1.f);
  //
  //mat[1] = 2;
  //// ASSERT - Matrix
  //EXPECT_FLOAT_EQ(mat.m[0], 5.f);
  //EXPECT_FLOAT_EQ(mat.m[1], 2.f);
  //EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[3], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[4], 5.f);
  //EXPECT_FLOAT_EQ(mat.m[5], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[8], 1.f);
  //
  //mat.transpose();
  //// ASSERT - Matrix Tranpose
  //EXPECT_FLOAT_EQ(mat.m[0], 5.f);
  //EXPECT_FLOAT_EQ(mat.m[1], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[2], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[3], 2.f);
  //EXPECT_FLOAT_EQ(mat.m[4], 5.f);
  //EXPECT_FLOAT_EQ(mat.m[5], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[6], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[7], 0.f);
  //EXPECT_FLOAT_EQ(mat.m[8], 1.f);
}

TEST(jdUtilities, JDTrigonometry_Test) {
  float normalSin = Math::sin(2.0f);
  float normalCos = Math::cos(2.0f);
  float normalTan = Math::tan(2.0f);
  float taySin = Math::taylorSin(2.0f);
  float tayCos = Math::taylorCos(2.0f);
  float tayTan = Math::taylorTan(2.0f);
  EXPECT_FLOAT_EQ(taySin, normalSin);
  EXPECT_FLOAT_EQ(tayCos, normalCos);
  EXPECT_FLOAT_EQ(tayTan, normalTan);
  Math::PI;
}


TEST(gen_math, rdc) {
  float isq = Math::Q_rsqrt(50.0f);
  float isqw = Math::WinInvSqrt(50.0f);

  float dif = isqw - isq;
}

TEST(gen_math, wis) {

}