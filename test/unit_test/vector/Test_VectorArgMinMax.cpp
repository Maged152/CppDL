#include "test.h"
#include "shakhbat_dl.h"


// Define the test parameters types
struct VectorArgMinMax : ::testing::TestWithParam<std::tuple<
    int,   // length
    unsigned int,   // number of threads
    float, // min value
    float  // max value
    >>
{};


// Define a parameterized test case
TEST_P(VectorArgMinMax, Test_VectorArgMinMax)
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

    unsigned int dst_min_ref;
    unsigned int dst_max_ref;

    unsigned int dst_min_lib;
    unsigned int dst_max_lib;

    // random initialization
    src.RandomInit(min_val, max_val);

    // run test code
    ref.Start();
    test::TestVectorArgMinMax(src, dst_min_ref, dst_max_ref);
    ref.End();

    // run lib code
    lib.Start();
    auto status = src.ArgMinMax(dst_min_lib, dst_max_lib, pool);
    lib.End();

    // print time
    test::PrintTime(ref, lib);

    // compare the results
    EXPECT_EQ(dst_min_ref, dst_min_lib);
    EXPECT_EQ(dst_max_ref, dst_max_lib);
}


// Instantiate the test case with combinations of values
INSTANTIATE_TEST_CASE_P(
    Test_VectorArgMinMax, VectorArgMinMax,
    ::testing::Combine(
        ::testing::Values(7, 100, 5000, 20000, 200000, 2000000),
        ::testing::Values(1, 3, 8, 16),
        ::testing::Values(0.0f, -100.0f),
        ::testing::Values(1.0f, 100.0f)
    ));