#include <string>

#define ANSI_TXT_GRN "\033[0;32m"
#define ANSI_TXT_MGT "\033[0;35m" //Magenta
#define ANSI_TXT_DFT "\033[0;0m" //Console default
#define GTEST_BOX "[Parameters] "
#define COUT_GTEST ANSI_TXT_GRN << GTEST_BOX //You could add the Default
#define COUT_GTEST_MGT COUT_GTEST << ANSI_TXT_MGT

namespace test
{
	template<typename T>
	void PrintParameter(T parameter, const std::string& para_name)
	{
		std::cout << COUT_GTEST_MGT << para_name 
			                        << " = " 
			                        << parameter 
			                        << ANSI_TXT_DFT << std::endl;
	}
}