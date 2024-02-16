#include "test.h"
#include "shakhbat_dl.h"


// Define the test parameters types
struct VectorUnit : ::testing::TestWithParam<std::tuple<
    int,   // length
    size_t,   // number of threads
    float, // min value
    float  // max value
    >>
{};


// Define a parameterized test case
TEST_P(VectorUnit, Test_VectorUnit)
{
    // extract the parameters
    auto& [length, num_threads, min_val, max_val] = GetParam();

    // threshold
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

    qlm::Vector dst_ref{ length };
    qlm::Vector dst_lib{ length };

    // random initialization
    src.RandomInit(min_val, max_val);

    // run test code
    ref.Start();
    test::Unit(src, dst_ref);
    ref.End();

    // run lib code
    lib.Start();
    auto status = src.Unit(dst_lib, pool);
    lib.End();

    // print time
    test::PrintTime(ref, lib);

    // compare the results
    bool res = test::TestCompare_Percentage(dst_lib, dst_ref, percentage_threshold);
    
    EXPECT_EQ(res, true);
}


// Instantiate the test case with combinations of values
INSTANTIATE_TEST_CASE_P(
    Test_VectorUnit, VectorUnit,
    ::testing::Combine(
        ::testing::Values(7, 100, 5000, 20000, 200000, 2000000),
        ::testing::Values(1, 3, 8, 16),
        ::testing::Values(0.0f, -100.0f),
        ::testing::Values(1.0f, 100.0f)
    ));