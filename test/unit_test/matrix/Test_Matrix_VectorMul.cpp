#include "test.h"
#include "shakhbat_dl.h"


// Define the test parameters types
struct MatrixMul : ::testing::TestWithParam<std::tuple<
    int,   // rows
	int,   // cols
	qlm::BroadCast, // Broad Cast
    size_t,   // number of threads
    float, // min value
    float  // max value
    >>
{};


// Define a parameterized test case
TEST_P(MatrixMul, Test_MatrixMul)
{
    constexpr float threshold = 0.0f;
    // extract the parameters
    auto& [rows, cols, broad_cast, num_threads, min_val, max_val] = GetParam();

    // print the parameters
    test::PrintParameter(rows, "rows");
	test::PrintParameter(cols, "cols");
	test::PrintParameter(broad_cast, "broad_cast");
    test::PrintParameter(num_threads, "num_threads");
    test::PrintParameter(min_val, "min_val");
    test::PrintParameter(max_val, "max_val");

    // initialize the function
    qlm::ThreadPool pool{ num_threads };

    qlm::Timer<qlm::usec> ref;
    qlm::Timer<qlm::usec> lib;

	int len = broad_cast == qlm::BroadCast::BROAD_CAST_ROW ? rows : cols;
    qlm::Matrix src1{ rows, cols };
    qlm::Vector src2{ len };

    qlm::Matrix dst_ref{ rows, cols };
    qlm::Matrix dst_lib{ rows, cols };

    // random initialization
    src1.RandomInit(min_val, max_val);
    src2.RandomInit(min_val, max_val);

    // run test code
    ref.Start();
    test::Mul(src1, src2, dst_ref, broad_cast);
    ref.End();

    // run lib code
    lib.Start();
    auto status = src1.Mul(src2, dst_lib, broad_cast, pool);
    lib.End();

    // print time
    test::PrintTime(ref, lib);

    // compare the results
    bool res = test::TestCompare(dst_ref, dst_lib, threshold);

    EXPECT_EQ(res, true);
}


// Instantiate the test case with combinations of values
INSTANTIATE_TEST_CASE_P(
    Test_MatrixMul, MatrixMul,
    ::testing::Combine(
        ::testing::Values(7, 100, 5000, 8578),
		::testing::Values(7, 100, 5000, 457),
		::testing::Values(qlm::BroadCast::BROAD_CAST_ROW, qlm::BroadCast::BROAD_CAST_COLUMN),
        ::testing::Values(1, 3, 8, 16),
        ::testing::Values(0.0f, -100.0f),
        ::testing::Values(1.0f, 100.0f)
    ));