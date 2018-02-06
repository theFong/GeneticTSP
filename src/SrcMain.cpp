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
    
    //
    auto pairs = SelectPairs(fitnesses, randomGen);
    WritePair(pairs);
    
    auto nexGen = CrossOver(pairs, initPop, randomGen);
    
    auto mutated = Mutate(nexGen, mutationChance, randomGen);
    
    auto solution = GetSolution(fitnesses);
    
    WriteSolution(solution, initPop, locations);
    
    
//    std::vector<Population*> generations(gens,nullptr);
//    auto indices = CreateIndices(gens);
//    
//    auto initPop = GenerateInitPop(locations.size(), popSize ,randomGen);
//    Population* initPopH = new Population;
//    initPopH->mMembers = initPop.mMembers;
//    generations[0] = initPopH;
//    
//    std::transform(generations.begin(), generations.end()-1, indices.begin(), generations.begin(),
//                   [locations, &randomGen, generations, mutationChance] (Population *curGen, int i) -> Population*
//                    {
//                        WriteGen(*curGen,i);
//                        
//                        auto fitnesses = CalcFitness(*curGen, locations);
//                        WriteFit(fitnesses);
//                        
//                        auto pairs = SelectPairs(fitnesses, randomGen);
//                        WritePair(pairs);
//                        
//                        auto nexGen = CrossOver(pairs, *curGen, randomGen);
//                        auto mutated = Mutate(nexGen, mutationChance, randomGen);
//                        Population* newPop = new Population;
//                        newPop->mMembers = mutated.mMembers;
//
//                        delete generations[i];
//                        return newPop;
//                    });
//    WriteGen(*generations[generations.size()-1], gens-1);
//    auto fitnesses = CalcFitness(*generations[generations.size()-1], locations);
//    auto solution = GetSolution(fitnesses);
//    WriteSolution(solution, *generations[generations.size()-1], locations);
    
}
