#include "pch.h"
#include "test_helper.h"
#include "../ML/include/shakhbat_ml.h"
//#include "../ML/test/test.h"


// Define the test parameters types
struct Parameters : ::testing::TestWithParam<std::tuple<
    int,
    bool
    >>
{};


// Define a parameterized test case
TEST_P(Parameters, Test_VectorAdd) 
{
    // extract the parameters
    auto& [num, n] = GetParam();

    // print the parameters
    test::PrintParameter(num, "num");
    test::PrintParameter(n, "n");

    //auto x = qlm::Factorial(5);
    //std::cout << x << "\n";
    // do the test
   // EXPECT_EQ(IsEven(num), num % 2 == 0);
}


// Instantiate the test case with combinations of values
INSTANTIATE_TEST_CASE_P(
    Test_VectorAddScenarios, Parameters,
    ::testing::Combine(
        ::testing::Values(2, 4, 6,  3, 5), 
        ::testing::Values(true, false)));