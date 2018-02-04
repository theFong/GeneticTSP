#include "SrcMain.h"
#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>

void ProcessCommandArgs(int argc, const char* argv[])
{
    std::string inputFile = argv[1];
    int popSize = atoi(argv[2]);
    int gens = atoi(argv[3]);
    int mutationChance = atoi(argv[4]);
    int seed = atoi(argv[5]);
    
    std::mt19937 randomGen = std::mt19937(seed);
    
    auto locations = ReadLocations(inputFile);
    
    auto initPop = GenerateInitPop(locations.size(), popSize ,randomGen);
    
    WriteGen(initPop,0);
    
    auto fitnesses = CalcFitness(initPop, locations);
    
    WriteFit(fitnesses);
//    WritePair(initPop,0);
    
}
