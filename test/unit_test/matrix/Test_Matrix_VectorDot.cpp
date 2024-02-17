#include "test.h"
#include "shakhbat_dl.h"


// Define the test parameters types
struct MatrixVectorDot : ::testing::TestWithParam<std::tuple<
    int,   // len
    size_t,   // number of threads
    float, // min value
    float  // max value
    >>
{};


// Define a parameterized test case
TEST_P(MatrixVectorDot, Test_MatrixVectorDot)
{
    constexpr float threshold = 0.0f;
    // extract the parameters
    auto& [len, num_threads, min_val, max_val] = GetParam();

    // print the parameters
    test::PrintParameter(len, "length");
    test::PrintParameter(num_threads, "num_threads");
    test::PrintParameter(min_val, "min_val");
    test::PrintParameter(max_val, "max_val");

    // initialize the function
    qlm::ThreadPool pool{ num_threads };

    qlm::Timer<qlm::usec> ref;
    qlm::Timer<qlm::usec> lib;

    qlm::Matrix src0{ len, len };
    qlm::Vector src1{ len };

    qlm::Vector dst_ref{ len };
    qlm::Vector dst_lib{ len };

    // random initialization
    src0.RandomInit(min_val, max_val);
    src1.RandomInit(min_val, max_val);

    // run test code
    ref.Start();
    test::Dot(src0, src1, dst_ref);
    ref.End();

    // run lib code
    lib.Start();
    auto status = src0.Dot(src1, dst_lib, pool);
    lib.End();

    // print time
    test::PrintTime(ref, lib);

    // compare the results
    bool res = test::TestCompare(dst_ref, dst_lib, threshold);

    EXPECT_EQ(res, true);
}


// Instantiate the test case with combinations of values
INSTANTIATE_TEST_CASE_P(
    Test_MatrixVectorDot, MatrixVectorDot,
    ::testing::Combine(
        ::testing::Values(7, 50, 110, 1),
        ::testing::Values(1, 3, 8, 16),
        ::testing::Values(0.0f, -100.0f),
        ::testing::Values(1.0f, 100.0f)
    ));