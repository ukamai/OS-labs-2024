#include <gtest/gtest.h>
#include <dlfcn.h>

std::string get_dlerror() {
    char* error = dlerror();
    if (error) {
        std::string err(error);
        dlerror(); 
        return err;
    }
    return "";
}

// Тесты для функции GCF
TEST(GCFTest, EuclidAlgorithm) {
    void* handle = dlopen("../../../lw4/build/libgcf_euclid.so", RTLD_LAZY);
    ASSERT_TRUE(handle != NULL) << "Failed to open library: " << get_dlerror();

    typedef int (*GCF_func)(int, int);
    GCF_func GCF = (GCF_func)dlsym(handle, "GCF");
    ASSERT_TRUE(GCF != NULL) << "Failed to find symbol: " << get_dlerror();

    EXPECT_EQ(GCF(10, 20), 10);
    EXPECT_EQ(GCF(15, 25), 5);
    EXPECT_EQ(GCF(35, 49), 7);

    dlclose(handle);
}

TEST(GCFTest, NaiveAlgorithm) {
    void* handle = dlopen("../../../lw4/build/libgcf_naive.so", RTLD_LAZY);
    ASSERT_TRUE(handle != NULL) << "Failed to open library: " << get_dlerror();

    typedef int (*GCF_func)(int, int);
    GCF_func GCF = (GCF_func)dlsym(handle, "GCF");
    ASSERT_TRUE(GCF != NULL) << "Failed to find symbol: " << get_dlerror();

    EXPECT_EQ(GCF(10, 20), 10);
    EXPECT_EQ(GCF(15, 25), 5);
    EXPECT_EQ(GCF(35, 49), 7);

    dlclose(handle);
}

TEST(SquareTest, Rectangle) {
    void* handle = dlopen("../../../lw4/build/libsquare_rectangle.so", RTLD_LAZY);
    ASSERT_TRUE(handle != NULL) << "Failed to open library: " << get_dlerror();

    typedef float (*Square_func)(float, float);
    Square_func Square = (Square_func)dlsym(handle, "Square");
    ASSERT_TRUE(Square != NULL) << "Failed to find symbol: " << get_dlerror();

    EXPECT_FLOAT_EQ(Square(10.0f, 20.0f), 200.0f);
    EXPECT_FLOAT_EQ(Square(15.0f, 25.0f), 375.0f);

    dlclose(handle);
}

TEST(SquareTest, Triangle) {
    void* handle = dlopen("../../../lw4/build/libsquare_triangle.so", RTLD_LAZY);
    ASSERT_TRUE(handle != NULL) << "Failed to open library: " << get_dlerror();

    typedef float (*Square_func)(float, float);
    Square_func Square = (Square_func)dlsym(handle, "Square");
    ASSERT_TRUE(Square != NULL) << "Failed to find symbol: " << get_dlerror();

    EXPECT_FLOAT_EQ(Square(10.0f, 20.0f), 100.0f);
    EXPECT_FLOAT_EQ(Square(15.0f, 25.0f), 187.5f);

    dlclose(handle);
}