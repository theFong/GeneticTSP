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

std::vector<Location> ReadLocations(const std::string &fileName);

Population GenerateInitPop(int locationSize, int popSize, std::mt19937 &randomGen );

void WriteGen(const Population &pop, int gen);

std::vector<std::pair<int, double>> CalcFitness(const Population &pop,const std::vector<Location> &locs);

double Distance(double lat1d, double lon1d, double lat2d, double lon2d);

void WriteFit(const std::vector<std::pair<int, double>> &fitnesses);

double Deg2rad(double deg);

std::vector<std::pair<int, int>> SelectPairs(const std::vector<std::pair<int, double>> &fitnesses, std::mt19937 &gen);

std::vector<int> CreateIndices(int n);

void WritePair(const std::vector<std::pair<int, int>> &pairs);

Population CrossOver(const std::vector<std::pair<int, int>> &pairs,const Population &pop, int mutationChance, std::mt19937 &gen);

std::vector<int> Mutate(const std::vector<int> &mem, int mutationChance, std::mt19937 &gen);

std::pair<int, double> GetSolution(const std::vector<std::pair<int, double>> &fitnesses);

void WriteSolution(const std::pair<int, double> &solution,const Population &pop,const std::vector<Location> &locs);
