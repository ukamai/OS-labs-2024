#include <gtest/gtest.h>
#include "../../lw2/include/point.h"
#include "../../lw2/include/triangle.h"

// Тест для функции point_distance
TEST(PointTest, Distance) {
    Point p1 = {0, 0, 0};
    Point p2 = {1, 1, 1};
    EXPECT_NEAR(point_distance(p1, p2), 1.732, 0.001);
}

// Тест для функции triangle_area
TEST(TriangleTest, Area) {
    Point p1 = {0, 0, 0};
    Point p2 = {1, 0, 0};
    Point p3 = {0, 1, 0};
    EXPECT_NEAR(triangle_area(p1, p2, p3), 0.5, 0.001);
}

// Тест для функции triangle_area с ненулевой площадью
TEST(TriangleTest, NonZeroArea) {
    Point p1 = {0, 0, 0};
    Point p2 = {1, 0, 0};
    Point p3 = {0, 1, 0};
    EXPECT_NEAR(triangle_area(p1, p2, p3), 0.5, 0.001);
}

// Тест для функции triangle_area с нулевой площадью
TEST(TriangleTest, ZeroArea) {
    Point p1 = {0, 0, 0};
    Point p2 = {1, 0, 0};
    Point p3 = {2, 0, 0};
    EXPECT_NEAR(triangle_area(p1, p2, p3), 0.0, 0.001);
}

// Тест для функции triangle_area с треугольником, лежащим в плоскости XY
TEST(TriangleTest, XYPlane) {
    Point p1 = {0, 0, 0};
    Point p2 = {1, 0, 0};
    Point p3 = {0, 1, 0};
    EXPECT_NEAR(triangle_area(p1, p2, p3), 0.5, 0.001);
}

// Тест для функции triangle_area с треугольником, лежащим в плоскости XZ
TEST(TriangleTest, XZPlane) {
    Point p1 = {0, 0, 0};
    Point p2 = {1, 0, 0};
    Point p3 = {0, 0, 1};
    EXPECT_NEAR(triangle_area(p1, p2, p3), 0.5, 0.001);
}

// Тест для функции triangle_area с треугольником, лежащим в плоскости YZ
TEST(TriangleTest, YZPlane) {
    Point p1 = {0, 0, 0};
    Point p2 = {0, 1, 0};
    Point p3 = {0, 0, 1};
    EXPECT_NEAR(triangle_area(p1, p2, p3), 0.5, 0.001);
}

// Тест для функции triangle_area с треугольником, лежащим в произвольной плоскости
TEST(TriangleTest, ArbitraryPlane) {
    Point p1 = {1, 2, 3};
    Point p2 = {4, 5, 6};
    Point p3 = {7, 8, 9};
    EXPECT_NEAR(triangle_area(p1, p2, p3), 0.0, 0.001);
}

// Тест для функции triangle_area с треугольником, лежащим в плоскости XY с отрицательными координатами
TEST(TriangleTest, NegativeXYPlane) {
    Point p1 = {-1, -1, 0};
    Point p2 = {1, -1, 0};
    Point p3 = {-1, 1, 0};
    EXPECT_NEAR(triangle_area(p1, p2, p3), 2.0, 0.001);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}