#include "test.h"
#include "shakhbat_ml.h"


// Define the test parameters types
struct VectorScalarDiv : ::testing::TestWithParam<std::tuple<
    int,   // length
    unsigned int,   // number of threads
    float, // scalar value
    float, // min value
    float  // max value
    >>
{};


// Define a parameterized test case
TEST_P(VectorScalarDiv, Test_VectorScalarDiv)
{
    constexpr float threshold = 0.0f;
    // extract the parameters
    auto& [length, num_threads, value, min_val, max_val] = GetParam();

    // print the parameters
    test::PrintParameter(length, "length");
    test::PrintParameter(num_threads, "num_threads");
    test::PrintParameter(value, "value");
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
    test::TestVectorScalarDiv(src, value, dst_ref);
    ref.End();

    // run lib code
    lib.Start();
    auto status = src.Div(value, dst_lib, pool);
    lib.End();

    // print time
    test::PrintTime(ref, lib);

    // compare the results
    bool res = test::TestCompare(dst_ref, dst_lib, threshold);

    EXPECT_EQ(res, true);
}


// Instantiate the test case with combinations of values
INSTANTIATE_TEST_CASE_P(
    Test_VectorScalarDiv, VectorScalarDiv,
    ::testing::Combine(
        ::testing::Values(7, 100, 5000, 20000, 200000, 2000000),
        ::testing::Values(1, 3, 8, 16),
        ::testing::Values(100.0f, -100.0f),
        ::testing::Values(0.0f, -100.0f),
        ::testing::Values(1.0f, 100.0f)
    ));