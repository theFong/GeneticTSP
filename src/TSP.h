#pragma once
#include <string>
#include <vector>
#include <random>

struct Location
{
	std::string mName;
	double mLatitude;
	double mLongitude;
};

struct Population
{
	std::vector<std::vector<int>> mMembers;
};

std::vector<Location> ReadLocations(std::string &fileName);

Population GenerateInitPop(int locationSize, int popSize, std::mt19937 &randomGen );

void WriteGen(Population pop, int gen);

std::vector<std::pair<int, double>> CalcFitness(Population pop, std::vector<Location> locs);

double Distance(double lat1d, double lon1d, double lat2d, double lon2d);

void WriteFit(std::vector<std::pair<int, double>> fitnesses);

double Deg2rad(double deg);
