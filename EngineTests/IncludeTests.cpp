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
	std::string str = ";;Hello|world||-foo--bar;yow;baz|";
	typedef boost::tokenizer<boost::char_separator<char> >
		tokenizer;
	EXPECT_TRUE(true);
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



TEST(FormatLib, TestForLinkerErrors)
{
	std::string s = fmt::format("{0}{1}{0}", "abra", "cad");
	EXPECT_EQ("abracadabra", s);
}




