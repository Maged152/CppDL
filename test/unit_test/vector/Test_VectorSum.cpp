#include "test.h"
#include "shakhbat_dl.h"


// Define the test parameters types
struct VectorSum : ::testing::TestWithParam<std::tuple<
    int,   // length
    unsigned int,   // number of threads
    float, // min value
    float  // max value
    >>
{};


// Define a parameterized test case
TEST_P(VectorSum, Test_VectorSum)
{
    // extract the parameters
    auto& [length, num_threads, min_val, max_val] = GetParam();
    
    // threshold
    //const unsigned int thread_len = std::ceil(length / num_threads);
    //const float epslon0 = std::pow(2.0f, -24.0f) * std::max(std::abs(max_val), std::abs(min_val));
    //const float epslon1 = epslon0 * thread_len;

    //const float threshold1_0 = epslon0 * (thread_len * (thread_len - 1) / 2);
    //const float threshold1_1 = epslon1 * (num_threads * (num_threads - 1) / 2);
    //const float threshold = threshold1_1 + threshold1_0;
    const float percentage_threshold = 0.07f;
    
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

    float dst_ref;
    float dst_lib;

    // random initialization
    src.RandomInit(min_val, max_val);

    // run test code
    ref.Start();
    test::TestVectorSum(src, dst_ref);
    ref.End();

    // run lib code
    lib.Start();
    auto status = src.Sum(dst_lib, pool);
    lib.End();

    // print time
    test::PrintTime(ref, lib);

    // compare the results
    bool res = test::TestCompare_Percentage(dst_lib, dst_ref, percentage_threshold);

    EXPECT_EQ(res, true);
}


// Instantiate the test case with combinations of values
INSTANTIATE_TEST_CASE_P(
    Test_VectorSum, VectorSum,
    ::testing::Combine(
        ::testing::Values(7, 100, 5000, 20000, 200000, 2000000),
        ::testing::Values(1, 3, 8, 16),
        ::testing::Values(0.0f, -100.0f),
        ::testing::Values(1.0f, 100.0f)
    ));