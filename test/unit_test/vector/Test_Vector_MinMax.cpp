#include "test.h"
#include "shakhbat_dl.h"


// Define the test parameters types
struct VectorMinMax : ::testing::TestWithParam<std::tuple<
    int,   // length
    size_t,   // number of threads
    float, // min value
    float  // max value
    >>
{};


// Define a parameterized test case
TEST_P(VectorMinMax, Test_VectorMinMax)
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

    float dst_min_ref;
    float dst_max_ref;

    float dst_min_lib;
    float dst_max_lib;

    // random initialization
    src.RandomInit(min_val, max_val);

    // run test code
    ref.Start();
    test::MinMax(src, dst_min_ref, dst_max_ref);
    ref.End();

    // run lib code
    lib.Start();
    auto status = src.MinMax(dst_min_lib, dst_max_lib, pool);
    lib.End();

    // print time
    test::PrintTime(ref, lib);

    // compare the results
    bool res = test::TestCompare(dst_min_ref, dst_min_lib, threshold);
    res &= test::TestCompare(dst_max_ref, dst_max_lib, threshold);

    EXPECT_EQ(res, true);
}


// Instantiate the test case with combinations of values
INSTANTIATE_TEST_CASE_P(
    Test_VectorMinMax, VectorMinMax,
    ::testing::Combine(
        ::testing::Values(7, 100, 5000, 20000, 200000, 2000000),
        ::testing::Values(1, 3, 8, 16),
        ::testing::Values(0.0f, -100.0f),
        ::testing::Values(1.0f, 100.0f)
    ));