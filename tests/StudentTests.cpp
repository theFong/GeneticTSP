#include "catch.hpp"
#include "SrcMain.h"
#include "TSP.h"
#include <string>

// Don't change this
#ifdef _MSC_VER
const std::string SamplePath("sampleoutput/win/");
#elif __APPLE__
const std::string SamplePath("sampleoutput/mac/");
#else
const std::string SamplePath("sampleoutput/linux/");
#endif
// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// TODO:
// Add test cases for your functions here!!
// (You will want to make multiple test cases with different sections)
TEST_CASE("Function test cases", "[student]")
{
//    GenerateInitPop
    SECTION("GenerateInitPop")
    {
        std::mt19937 randomGen = std::mt19937(1337);
        Population testP = GenerateInitPop(20, 8 ,randomGen);
        bool test = (testP.mMembers.size() == 8) && (testP.mMembers[0].size() == 20);
        REQUIRE(test);
    }
//    CalcFitness
    SECTION("CalcFitness")
    {
//        std::vector<std::pair<int, double>> CalcFitness(const Population &pop,const std::vector<Location> &locs)
        Population p;
        p.mMembers = std::vector<std::vector<int>>(2);
        p.mMembers[0] = {0,9,1,10,14,7,8,11,16,18,6,17,4,13,15,2,5,19,12,3};
        p.mMembers[1] = {0,19,4,12,3,16,7,1,15,2,11,14,13,5,18,8,9,10,6,17};
        auto locations = ReadLocations("input/locations.txt");
        auto testRes = CalcFitness(p,locations);
        bool result = testRes[0].second == 370.9142064216266 && testRes[1].second == 356.92062328756896;
        REQUIRE(result);
    }
//    Deg2rad
    SECTION("Deg2rad")
    {
        double rad = Deg2rad(57.2958);
        bool test = int(rad) == 1;
        REQUIRE(test);
    }
//    Distance
    SECTION("Distance")
    {
        auto locations = ReadLocations("input/locations.txt");
        double dist = Distance(locations[0].mLatitude, locations[0].mLongitude,locations[1].mLatitude, locations[1].mLongitude);
        bool test = (dist == 8.9170522271021948);
        REQUIRE(test);
    }
//    SelectPairs
    SECTION("SelectPairs")
    {
        std::mt19937 randomGen = std::mt19937(1337);
        std::vector<std::pair<int, double>> pairs(3);
        pairs[0] = std::pair<int, double>(0,100.0);
        pairs[1] = std::pair<int, double>(1,150.0);
        pairs[2] = std::pair<int, double>(2,125.0);
        auto sel = SelectPairs(pairs, randomGen);
        bool test = sel[0].first == 2 && sel[1].second == 0 && sel[2].first == 0;
        REQUIRE(test);
    }
//    CrossOver
    SECTION("CrossOver")
    {
        std::vector<std::pair<int,int>>pairs(3);
        pairs[0] = std::pair<int,int>(0,1);
        pairs[1] = std::pair<int,int>(1,2);
        pairs[2] = std::pair<int,int>(1,0);
        bool test = true;
        Population inPop;
        inPop.mMembers = std::vector<std::vector<int>>(3);
        inPop.mMembers[0] = {0,1,2};
        inPop.mMembers[0] = {2,1,0};
        inPop.mMembers[0] = {2,0,1};
        std::mt19937 randomGen = std::mt19937(1337);
        
//        Population p = CrossOver(pairs, inPop, 80, randomGen);
        REQUIRE(test);
        
    }
//    Mutate
    SECTION("Mutate")
    {
        std::mt19937 randomGen = std::mt19937(2);
        std::vector<int> mem = {0,1,2,3,4,5,6,7,8,9,10};
        mem = Mutate(mem, 100, randomGen);
        bool test = mem[7] == 9 && mem[9] == 7;
        REQUIRE(test);
    }
//    GetSolution
    SECTION("GetSolution")
    {
        std::vector<std::pair<int, double>> testFit(3);
        testFit[0] = std::pair<int, double>(0,150.0);
        testFit[1] = std::pair<int, double>(0,100.0);
        testFit[2] = std::pair<int, double>(0,200.0);
        auto sol = GetSolution(testFit);
        bool test = sol.second == 100.0;
        REQUIRE(test);
    }
//    CreateIndices
    SECTION("CreateIndices")
    {
        auto inds = CreateIndices(5);
        bool result = inds.size() == 5;
        REQUIRE(result);
    }
}

// Provided Complete file test cases
// (Once you pass the first section, comment out the other)
TEST_CASE("Full file test cases", "[student]")
{
	SECTION("input/locations.txt 8 5 10 1337")
	{
        //"/Users/AlecFong/GoogleDrive/WorkStation/itp435/pa2-theFong/input/locations.txt"
//        "input/locations.txt"
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"8",
			"5",
			"10",
			"1337"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_8_5_10_1337.txt");
		REQUIRE(result);
	}
	SECTION("input/locations.txt 32 20 5 5741328")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"32",
			"20",
			"5",
			"5741328"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_32_20_5_5741328.txt");
		REQUIRE(result);
	}
	SECTION("input/locations.txt 64 500 30 12165465")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"64",
			"500",
			"30",
			"12165465"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_64_500_30_12165465.txt");
		REQUIRE(result);
	}
	SECTION("input/locations.txt 128 200 10 331988960")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"128",
			"200",
			"10",
			"331988960"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_128_200_10_331988960.txt");
		REQUIRE(result);
	}
	SECTION("input/locations.txt 256 200 20 7410785")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"256",
			"200",
			"20",
			"7410785"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_256_200_20_7410785.txt");
		REQUIRE(result);
	}
	SECTION("input/locations2.txt 64 200 25 49328573")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations2.txt",
			"64",
			"200",
			"25",
			"49328573"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_64_200_25_49328573.txt");
		REQUIRE(result);
	}
}
