#pragma once
class RepresentationTests : public ::testing::Test 
{
public:
	RepresentationTests();
	~RepresentationTests();

	virtual void SetUp() {
		Checkmate::Bitboards::init();
	}

};

