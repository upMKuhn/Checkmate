#include "stdafx.h"
#include "IncludeTests.h"

TEST(GoogleMockWorks, TestForLinkerErrors) {
	MockTurtle turtle;                          // #2
	EXPECT_CALL(turtle, PenDown())              // #3
		.Times(AtLeast(2));
	EXPECT_TRUE(testfunction(turtle));
}


TEST(Boost, TestForLinkerErrors)
{
	using namespace boost; using namespace boost::algorithm;

	string line("Hello World !");
	std::vector<std::string> strs;
	boost::split(strs, line, boost::is_any_of(" "));
	EXPECT_EQ(3, strs.size());
	EXPECT_EQ("Hello",strs.at(0));
}

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;




TEST(BoostLog, TestForLinkerErrors)
{
	logging::add_file_log("Boostfile.log");
	keywords::auto_flush = true;
	keywords::target = "C:\\";
	BOOST_LOG_TRIVIAL(trace) << "Hello World!!";
	BOOST_LOG_TRIVIAL(info) << "A trace severity message";
	EXPECT_TRUE(true);
}





