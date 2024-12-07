#include <gtest/gtest.h> 
#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <cstring>

extern "C" {
    #include <utils.h>
    #include <parent.h>
}

TEST(test_reverse_string, test_simple_string) 
{ 
    char str[] = "abcdef";
    char expected_str[] = "fedcba";
    ReverseString(str);
    ASSERT_TRUE(strcmp(str, expected_str) == 0);
} 

TEST(test_reverse_string, test_empty_string) 
{ 
    char str[] = "";
    char expected_str[] = "";
    ReverseString(str);
    ASSERT_TRUE(strcmp(str, expected_str) == 0);
} 

TEST(test_reverse_string, test_single_character) 
{ 
    char str[] = "a";
    char expected_str[] = "a";
    ReverseString(str);
    ASSERT_TRUE(strcmp(str, expected_str) == 0);
} 

TEST(test_reverse_string, test_special_characters) 
{ 
    char str[] = "!@#$%^&*()";
    char expected_str[] = ")(*&^%$#@!";
    ReverseString(str);
    ASSERT_TRUE(strcmp(str, expected_str) == 0);
} 

TEST(test_reverse_string, test_mixed_characters) 
{ 
    char str[] = "a1b2c3d4e5f6";
    char expected_str[] = "6f5e4d3c2b1a";
    ReverseString(str);
    ASSERT_TRUE(strcmp(str, expected_str) == 0);
} 

int main(int argc, char **argv) { 
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}