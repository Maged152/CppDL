#include "test.h"
#include "shakhbat_dl.h"


// Define the test parameters types
struct VectorArgMax : ::testing::TestWithParam<std::tuple<
    int,   // length
    size_t,   // number of threads
    float, // min value
    float  // max value
    >>
{};


// Define a parameterized test case
TEST_P(VectorArgMax, Test_VectorArgMax)
{
    constexpr float threshold = 0.0f;
    // extract the parameters
    auto& [length, num_threads, min_val, max_val] = GetParam();

    // print the parameters
    test::PrintParameter(length, "length");
    test::PrintParameter(num_threads, "num_threads");
    test::PrintParameter(min_val, "min_val");
    test::PrintParameter(max_val, "max_val");

    // initialize the function
    qlm::ThreadPool pool{ num_threads };

    qlm::Timer<qlm::usec> ref;
    qlm::Timer<qlm::usec> lib;

    qlm::Vector src{ length };

    size_t dst_ref;
    size_t dst_lib;

    // random initialization
    src.RandomInit(min_val, max_val);

    // run test code
    ref.Start();
    test::ArgMax(src, dst_ref);
    ref.End();

    // run lib code
    lib.Start();
    auto status = src.ArgMax(dst_lib, pool);
    lib.End();

    // print time
    test::PrintTime(ref, lib);

    // compare the results
    EXPECT_EQ(dst_lib, dst_ref);
}


// Instantiate the test case with combinations of values
INSTANTIATE_TEST_CASE_P(
    Test_VectorArgMax, VectorArgMax,
    ::testing::Combine(
        ::testing::Values(7, 100, 5000, 20000, 200000, 2000000),
        ::testing::Values(1, 3, 8, 16),
        ::testing::Values(0.0f, -100.0f),
        ::testing::Values(1.0f, 100.0f)
    ));