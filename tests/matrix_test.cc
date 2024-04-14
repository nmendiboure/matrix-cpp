#include "gtest/gtest.h"
#include "../core/component/matrix.h"

#include <vector>

TEST(MatrixTest, DefaultConstructor) {
    Matrix<int> m;
    EXPECT_EQ(m.getHeight(), 0);
    EXPECT_EQ(m.getWidth(), 0);
}

TEST(MatrixTest, ConstructorWithSize) {
    Matrix<int> m(2, 3);
    EXPECT_EQ(m.getHeight(), 2);
    EXPECT_EQ(m.getWidth(), 3);
}

TEST(MatrixTest, ConstructorWithSizeAndDefault) {
    Matrix<int> m(2, 3, 5);
    EXPECT_EQ(m.getHeight(), 2);
    EXPECT_EQ(m.getWidth(), 3);
    EXPECT_EQ(m.get(0, 0), 5);
    EXPECT_EQ(m.get(1, 2), 5);
}

TEST(MatrixTest, MoveConstructor) {
    Matrix<int> m1(2, 2, 5);
    Matrix<int> m2(std::move(m1));
    EXPECT_EQ(m2.getHeight(), 2);
    EXPECT_EQ(m2.getWidth(), 2);
    EXPECT_EQ(m2.get(0, 0), 5);
}

TEST(MatrixTest, ConstructorWithVector) {
    std::vector<std::vector<int>> vec = {{1, 2}, {3, 4}};
    Matrix<int> m(vec);
    EXPECT_EQ(m.getHeight(), 2);
    EXPECT_EQ(m.getWidth(), 2);
    EXPECT_EQ(m.get(0, 0), 1);
    EXPECT_EQ(m.get(1, 1), 4);
}

TEST(MatrixTest, MoveConstructorWithVector) {
    std::vector<std::vector<int>> vec = {{1, 2}, {3, 4}};
    Matrix<int> m(std::move(vec));
    EXPECT_EQ(m.getHeight(), 2);
    EXPECT_EQ(m.getWidth(), 2);
    EXPECT_EQ(m.get(0, 0), 1);
    EXPECT_EQ(m.get(1, 1), 4);
}

TEST(MatrixAccessTest, ElementAccess) {
    // test () operator and get()
    Matrix<int> m(2, 3, 7);
    EXPECT_EQ(m(0, 0), 7);
    EXPECT_EQ(m.get(0, 0), 7);

    m(0, 0) = 5;
    EXPECT_EQ(m.get(0, 0), 5);

    std::vector<int> line = {1, 2, 3};
    m(1) = line;
    EXPECT_EQ(m.get(1, 1), 2);
}


TEST(MatrixAccessTest, DimensionAccess) {
    // test getHeight(), getWidth() and getShape()
    Matrix<int> m(4, 5, 0);
    EXPECT_EQ(m.getHeight(), 4);
    EXPECT_EQ(m.getWidth(), 5);
    std::pair<int, int> shape = m.getShape();
    EXPECT_EQ(shape.first, 4);
    EXPECT_EQ(shape.second, 5);
}

TEST(MatrixMethodTest, Clear) {
    Matrix<int> m(2, 2, 1);
    m.clear();
    EXPECT_EQ(m.getHeight(), 0);
    EXPECT_EQ(m.getWidth(), 0);
}

TEST(MatrixMethodTest, Duplicate) {
    Matrix<int> m(2, 2, 1);
    Matrix<int> m2 = m.duplicate();
    EXPECT_EQ(m2.getHeight(), 2);
    EXPECT_EQ(m2.getWidth(), 2);
    EXPECT_EQ(m2.get(0, 0), 1);
}

TEST(MatrixMethodTest, Erase) {
    Matrix<int> m(3, 3, 1);
    m.erase(1, 0);
    EXPECT_EQ(m.getHeight(), 2);
}

TEST(MatrixMethodTest, Fill) {
    Matrix<int> m(2, 2);
    m.fill(5);
    EXPECT_EQ(m.get(0, 0), 5);
    EXPECT_EQ(m.get(1, 1), 5);
}

TEST(MatrixMethodTest, GetCol) {
    Matrix<int> m(2, 3);
    m.put(0, 1, 5);
    auto col = m.getCol(1);
    EXPECT_EQ(col[0], 5);
}

TEST(MatrixMethodTest, Insert) {
    Matrix<int> m(2, 2, 1);
    std::vector<int> newRow = {3, 3};
    m.insert(1, newRow, 0);
    EXPECT_EQ(m.getHeight(), 3);
    EXPECT_EQ(m.get(1, 0), 3);
}

TEST(MatrixMethodTest, PopBack) {
    Matrix<int> m(3, 3, 1);
    m.pop_back(0);
    EXPECT_EQ(m.getHeight(), 2);
}

TEST(MatrixMethodTest, PushBack) {
    Matrix<int> m(2, 2, 1);
    std::vector<int> newRow = {3, 3};
    m.push_back(newRow, 0);
    EXPECT_EQ(m.getHeight(), 3);
    EXPECT_EQ(m.get(2, 0), 3);
}

TEST(MatrixMethodTest, Put) {
    Matrix<int> m(2, 2, 1);
    m.put(0, 0, 5);
    EXPECT_EQ(m.get(0, 0), 5);
}

TEST(MatrixMethodTest, ReserveAndResize) {
    Matrix<int> m;
    m.reserve(2, 2);
    m.resize(2, 2);
    EXPECT_EQ(m.getHeight(), 2);
    EXPECT_EQ(m.getWidth(), 2);
}

TEST(MatrixMethodTest, SubMat) {
    Matrix<int> m(4, 4, 1);
    Matrix<int> sub = m.subMat(1, 1, 2, 2);
    EXPECT_EQ(sub.getHeight(), 2);
    EXPECT_EQ(sub.getWidth(), 2);
}

TEST(MatrixMathOperations, Add) {
    Matrix<int> m1(2, 2, 1);
    Matrix<int> m2(2, 2, 2);
    Matrix<int> result = m1.add(m2);
    EXPECT_EQ(result.get(0, 0), 3);
    EXPECT_EQ(result.get(1, 1), 3);
}


TEST(MatrixMathOperations, Subtract) {
    Matrix<int> m1(2, 2, 3);
    Matrix<int> m2(2, 2, 2);
    Matrix<int> result = m1.subtract(m2);
    EXPECT_EQ(result.get(0, 0), 1);
}

TEST(MatrixMathOperations, Multiply) {
    Matrix<int> m1(2, 2, 2);
    Matrix<int> result = m1.multiply(2);
    EXPECT_EQ(result.get(0, 0), 4);

    Matrix<int> m2(2, 2, 3);
    std::vector<int> v = {2, 2};
    Matrix<int> result2 = m2.multiply(v);

    Matrix<int> m3(2, 2, 3);
    Matrix<int> result3 = m1.multiply(m3);
    EXPECT_EQ(result3.get(0, 0), 6);
}

TEST(MatrixMathOperations, Dot) {
    Matrix<int> m1(2, 3, 1);
    Matrix<int> m2(3, 2, 2);
    Matrix<int> result = m1.dot(m2);
    EXPECT_EQ(result.get(0, 0), 6);
}

TEST(MatrixMathOperations, Transpose) {
    Matrix<int> m1(2, 3, 1);
    Matrix<int> result = m1.transpose();
    EXPECT_EQ(result.getHeight(), 3);
    EXPECT_EQ(result.getWidth(), 2);
}

TEST(MatrixMathOperations, Divide) {
    Matrix<int> m1(2, 2, 4);
    Matrix<int> result = m1.divide(2);
    EXPECT_EQ(result.get(0, 0), 2);

    Matrix<int> m2(2, 2, 4);
    std::vector<int> v = {2, 2};
    Matrix<int> result2 = m2.divide(v);
    EXPECT_EQ(result2.get(0, 0), 2);

    Matrix<int> m3(2, 2, 4);
    Matrix<int> result3 = m3.divide(m2);
    EXPECT_EQ(result3.get(0, 0), 1);
}


TEST(MatrixMathTest, MaxMinSum) {
    Matrix<int> m(2, 2);
    m.put(0, 0, 1);
    m.put(0, 1, 2);
    m.put(1, 0, 3);
    m.put(1, 1, 4);

    EXPECT_EQ(m.max(), 4);
    EXPECT_EQ(m.min(), 1);
    EXPECT_EQ(m.sum(), 10);

    auto maxRow = m.max(0);
    EXPECT_EQ(maxRow[0], 2);
    EXPECT_EQ(maxRow[1], 4);

    auto maxCol = m.max(1);
    EXPECT_EQ(maxCol[0], 3);
    EXPECT_EQ(maxCol[1], 4);

    auto minRow = m.min(0);
    EXPECT_EQ(minRow[0], 1);
    EXPECT_EQ(minRow[1], 3);

    auto minCol = m.min(1);
    EXPECT_EQ(minCol[0], 1);
    EXPECT_EQ(minCol[1], 2);

    auto sumRow = m.sum(0);
    EXPECT_EQ(sumRow[0], 3);
    EXPECT_EQ(sumRow[1], 7);

    auto sumCol = m.sum(1);
    EXPECT_EQ(sumCol[0], 4);
    EXPECT_EQ(sumCol[1], 6);
}

TEST(MatrixMathTest, CumulativeSum) {
    Matrix<int> m(2, 2);
    m.put(0, 0, 1);
    m.put(0, 1, 2);
    m.put(1, 0, 3);
    m.put(1, 1, 4);

    auto cumuSumResult = m.cumuSum(0);
    EXPECT_EQ(cumuSumResult.get(0, 0), 1);
    EXPECT_EQ(cumuSumResult.get(1, 0), 4);
}


TEST(MatrixOperatorTest, EqualityAndInequality) {
    Matrix<int> m1(2, 2, 1);
    Matrix<int> m2(2, 2, 1);
    Matrix<int> m3(2, 2, 2);

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 != m2);
    EXPECT_FALSE(m1 == m3);
    EXPECT_TRUE(m1 != m3);
}


TEST(MatrixOperatorTest, AdditionAndSubtraction) {
    Matrix<int> m1(2, 2, 1);
    Matrix<int> m2(2, 2, 2);
    m1 += m2;
    EXPECT_EQ(m1.get(0, 0), 3);
    m1 -= m2;
    EXPECT_EQ(m1.get(0, 0), 1);
}


TEST(MatrixOperatorTest, Multiplication) {
    Matrix<int> m1(2, 2, 1);
    Matrix<int> m2(2, 2, 2);
    std::vector<int> v = {2, 2};

    m1 *= m2;
    EXPECT_EQ(m1.get(0, 0), 2);

    m1 = Matrix<int>(2, 2, 1);
    m1 *= v;
    EXPECT_EQ(m1.get(0, 0), 2);

    m1 = Matrix<int>(2, 2, 1);
    m1 *= 3;
    EXPECT_EQ(m1.get(0, 0), 3);
}


TEST(MatrixOperatorTest, Division) {
    Matrix<int> m1(2, 2, 4);
    Matrix<int> m2(2, 2, 2);
    std::vector<int> v = {2, 2};

    m1 /= 2;
    EXPECT_EQ(m1.get(0, 0), 2);

    m1 = Matrix<int>(2, 2, 4);
    m1 /= v;
    EXPECT_EQ(m1.get(0, 0), 2);

    m1 = Matrix<int>(2, 2, 4);
    m1 /= m2;
    EXPECT_EQ(m1.get(0, 0), 2);
}


TEST(MatrixOperatorTest, AssignmentFromVector) {
    std::vector<std::vector<int>> vec = {{1, 2}, {3, 4}};
    Matrix<int> m;
    m = vec;
    EXPECT_EQ(m.get(0, 0), 1);
    EXPECT_EQ(m.get(1, 1), 4);
}



TEST(MatrixOperatorTest, MoveAssignment) {
    Matrix<int> m1(2, 2, 1);
    Matrix<int> m2;
    m2 = std::move(m1);
    EXPECT_EQ(m2.get(0, 0), 1);
}


TEST(MatrixOperatorTest, MoveAssignmentFromVector) {
    std::vector<std::vector<int>> vec = {{1, 2}, {3, 4}};
    Matrix<int> m;
    m = std::move(vec);
    EXPECT_EQ(m.get(0, 0), 1);
    EXPECT_EQ(m.get(1, 1), 4);
}

TEST(MatrixOperatorTest, OperatorAdd) {
    Matrix<int> m1(2, 2, 1);
    Matrix<int> m2(2, 2, 2);
    Matrix<int> result = m1 + m2;
    EXPECT_EQ(result.get(0, 0), 3);
    EXPECT_EQ(result.get(1, 1), 3);
}

TEST(MatrixOperatorTest, OperatorSubtract) {
    Matrix<int> m1(2, 2, 3);
    Matrix<int> m2(2, 2, 2);
    Matrix<int> result = m1 - m2;
    EXPECT_EQ(result.get(0, 0), 1);
    EXPECT_EQ(result.get(1, 1), 1);
}

TEST(MatrixOperatorTest, OperatorMultiplyMatrix) {
    Matrix<int> m1(2, 2, 2);
    Matrix<int> m2(2, 2, 3);
    Matrix<int> result = m1 * m2;
    EXPECT_EQ(result.get(0, 0), 6);
    EXPECT_EQ(result.get(1, 1), 6);
}

TEST(MatrixOperatorTest, OperatorMultiplyVector) {
    Matrix<int> m(2, 2, 2);
    std::vector<int> v = {3, 3};
    Matrix<int> result = m * v;
    EXPECT_EQ(result.get(0, 0), 6);
    EXPECT_EQ(result.get(1, 1), 6);
}


TEST(MatrixOperatorTest, OperatorMultiplyScalar) {
    Matrix<int> m(2, 2, 2);
    Matrix<int> result = m * 3;
    EXPECT_EQ(result.get(0, 0), 6);
    EXPECT_EQ(result.get(1, 1), 6);
}


TEST(MatrixOperatorTest, OperatorDivide) {
    Matrix<int> m(2, 2, 4);
    Matrix<int> result = m / 2;
    EXPECT_EQ(result.get(0, 0), 2);
    EXPECT_EQ(result.get(1, 1), 2);
}

TEST(MatrixOperatorTest, OperatorDivideVector) {
    Matrix<int> m(2, 2, 4);
    std::vector<int> v = {2, 2};
    Matrix<int> result = m / v;
    EXPECT_EQ(result.get(0, 0), 2);
    EXPECT_EQ(result.get(1, 1), 2);
}

TEST(MatrixOperatorTest, OperatorDivideMatrix) {
    Matrix<int> m1(2, 2, 4);
    Matrix<int> m2(2, 2, 2);
    Matrix<int> result = m1 / m2;
    EXPECT_EQ(result.get(0, 0), 2);
    EXPECT_EQ(result.get(1, 1), 2);
}


TEST(MatrixOperatorTest, OperatorStream) {
    Matrix<int> m(2, 2, 1);
    std::stringstream buffer;
    buffer << m;
    std::string output = buffer.str();
    std::string expectedOutput = "1 1 \n1 1 \n";
    EXPECT_EQ(output, expectedOutput);
}
