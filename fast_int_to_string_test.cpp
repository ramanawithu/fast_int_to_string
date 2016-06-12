// Command to compile
// clang++ -o fast_int_to_string_test fast_int_to_string_test.cpp -std=c++11 -lpthread -lgtest -m64
// g++ -o fast_int_to_string_test fast_int_to_string_test.cpp -std=c++11 -pthread -lgtest -m64

#include <cstdlib>
#include <vector>
#include <chrono>
#include <iostream>
#include <ctime>
#include <cstring>

// Unit testing
#include <gtest/gtest.h>

#include "fast_int_to_string.hpp"

TEST(FastUnsignedIntToString, ContinuousRange) {
    for (int i = 0; i <= (1 << 15); i++)
        EXPECT_TRUE(fast_uint64_to_string(uint64_t(i)) == std::to_string(uint64_t(i)));
}

TEST(FastUnsignedIntToString, PowerOfTwoBoundaries) {
    for (int i = 0; i < 64; i++) {
        uint64_t val = 1 << i;
        EXPECT_TRUE(fast_uint64_to_string(val-1) == std::to_string(val-1));
        EXPECT_TRUE(fast_uint64_to_string(val) == std::to_string(val));
        EXPECT_TRUE(fast_uint64_to_string(val+1) == std::to_string(val+1));
    }
}

TEST(FastUnsignedIntToString, PowerOfTenBoundaries) {
    uint64_t val = 1;
    for (int i = 1; i < 20; i++) {
        val *= 10;
        EXPECT_TRUE(fast_uint64_to_string(val-1) == std::to_string(val-1));
        EXPECT_TRUE(fast_uint64_to_string(val) == std::to_string(val));
        EXPECT_TRUE(fast_uint64_to_string(val+1) == std::to_string(val+1));
    }
}

TEST(FastUnsignedIntToSlice, ContinuousRange) {
    char buffer1[32];
    char buffer2[32];
    for (int i = 0; i <= (1 << 15); i++) {
        fast_uint64_to_string(uint64_t(i)-1, buffer1);
        std::sprintf(buffer2, "%lu", uint64_t(i)-1);
        EXPECT_TRUE(std::strcmp(buffer1, buffer2) == 0);

        fast_uint64_to_string(uint64_t(i), buffer1);
        std::sprintf(buffer2, "%lu", uint64_t(i));
        EXPECT_TRUE(std::strcmp(buffer1, buffer2) == 0);

        fast_uint64_to_string(uint64_t(i)+1, buffer1);
        std::sprintf(buffer2, "%lu", uint64_t(i)+1);
        EXPECT_TRUE(std::strcmp(buffer1, buffer2) == 0);
    }
}

TEST(FastUnsignedIntToSlice, PowerOfTwoBoundaries) {
    char buffer1[32];
    char buffer2[32];
    for (int i = 0; i < 64; i++) {
        uint64_t val = 1 << i;

        fast_uint64_to_string(val-1, buffer1);
        std::sprintf(buffer2, "%lu", val-1);
        EXPECT_TRUE(std::strcmp(buffer1, buffer2) == 0);

        fast_uint64_to_string(val, buffer1);
        std::sprintf(buffer2, "%lu", val);
        EXPECT_TRUE(std::strcmp(buffer1, buffer2) == 0);

        fast_uint64_to_string(val+1, buffer1);
        std::sprintf(buffer2, "%lu", val+1);
        EXPECT_TRUE(std::strcmp(buffer1, buffer2) == 0);
    }
}

TEST(FastUnsignedIntToSlice, PowerOfTenBoundaries) {
    uint64_t val = 1;

    char buffer1[32];
    char buffer2[32];
    for (int i = 1; i < 20; i++) {
        val *= 10;

        fast_uint64_to_string(val-1, buffer1);
        std::sprintf(buffer2, "%lu", val-1);
        EXPECT_TRUE(std::strcmp(buffer1, buffer2) == 0);

        fast_uint64_to_string(val, buffer1);
        std::sprintf(buffer2, "%lu", val);
        EXPECT_TRUE(std::strcmp(buffer1, buffer2) == 0);

        fast_uint64_to_string(val+1, buffer1);
        std::sprintf(buffer2, "%lu", val+1);
        EXPECT_TRUE(std::strcmp(buffer1, buffer2) == 0);
    }
}

void PerformanceTest_1() {
    const uint64_t test_input = 18446744073709551557UL;
    char buffer_1[32] = {'\0'};
    char buffer_2[32] = {'\0'};

    auto start_2 = clock();
    sprintf(buffer_2, "%lu", test_input);
    auto end_2 = clock();

    auto start_1 = clock();
    fast_uint64_to_string(test_input, buffer_1);
    auto end_1 = clock();

    auto d_1 = double(end_1 - start_1)/CLOCKS_PER_SEC;
    auto d_2 = double(end_2 - start_2)/CLOCKS_PER_SEC;

    assert(0 == std::strcmp(buffer_1, buffer_2));

    std::cout << "fast_uint64_to_string time " << d_1 << '\n';
    std::cout << "sprintf time " << d_2 << '\n';
    std::cout << "% speed increase " << (((d_2 - d_1)/d_2)*100) << '\n';
}

void PerformanceTest_2() {
    // Ensure the dataset fits in few pages of RAM. We are not interested in testing OS overhead.
    const int test_size = 1000;
    std::vector<uint64_t> v;
    for (int i = 0; i < test_size; i++)
        v.push_back(uint64_t(i));

    char buffer[32] = {'\0'};

    auto start_2 = clock();
    for (int i = 0; i < test_size; i++)
        sprintf(buffer, "%lu", v[i]);
    auto end_2 = clock();

    auto start_1 = clock();
    for (int i = 0; i < test_size; i++)
        fast_uint64_to_string(v[i], buffer);
    auto end_1 = clock();

    auto d_1 = double(end_1 - start_1)/CLOCKS_PER_SEC;
    auto d_2 = double(end_2 - start_2)/CLOCKS_PER_SEC;

    std::cout << "fast_uint64_to_string time " << d_1 << '\n';
    std::cout << "sprintf time " << d_2 << '\n';
    std::cout << "% speed increase " << (((d_2 - d_1)/d_2)*100) << '\n';
}

int main(int argc, char *argv[]) {
    PerformanceTest_1();
    PerformanceTest_2();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
