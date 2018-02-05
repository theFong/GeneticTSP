#include "TSP.h"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <sstream>
#include <iostream>
#include <fstream>
#include <random>

//#include <unistd.h>
//#define GetCurrentDir getcwd

std::vector<Location> ReadLocations(std::string &fileName)
{
    // working dir pa2-theFong/tests/Debug
    
    std::ifstream file (fileName);
    std::vector<Location> locations;
    while(file)
    {
//        LAX Airport,33.941845,-118.408635
        std::string line;
        if (std::getline( file, line ))
        {
            std::istringstream ss(line);
            std::string token;
            
            std::getline(ss, token, ',');
            std::string name = token;
            
            std::getline(ss, token, ',');
            double latitude = stod(token);
            
            std::getline(ss, token, ',');
            double longitude =  stod(token);
            
            Location loc = {
                name,
                latitude,
                longitude
            };
            locations.push_back(loc);
        }
        else
        {
            break;
        }
    }
    if (locations.size() == 0) {
        std::cout << "No locations read" << std::endl;
    }
    return locations;
}

Population GenerateInitPop(int locationSize, int popSize, std::mt19937 &randomGen)
{
    // create population to populate
    Population p;
    std::vector<std::vector<int>> pops(popSize);
    p.mMembers = pops;

    std::vector<int> indices(locationSize, 0);
    std::transform(indices.begin()+1, indices.end(), indices.begin(),indices.begin()+1,
                   [](int current, int behind) -> int{ return behind+1;} );

    // nested transform -> iterate over mMembers passing in size nested transform creates indicee vector and shuffles it
    std::transform(p.mMembers.begin(),p.mMembers.end(),p.mMembers.begin(),
                   [&randomGen, indices](std::vector<int> x) -> std::vector<int>
    {
        auto indicesCopy = indices;
        std::shuffle(indicesCopy.begin()+1, indicesCopy.end(), randomGen);
        return indicesCopy;
    });
    return p;
}

void WriteGen(Population pop, int gen)
{
    std::ofstream oFile;
    std::string genString = "GENERATION: "+ std::to_string(gen);
    if (gen == 0)
    {
        genString = "INITIAL POPULATION:";
        oFile = std::ofstream("log.txt", std::ofstream::out|std::ofstream::trunc);
    } else
    {
        oFile = std::ofstream("log.txt", std::ofstream::out|std::ofstream::app);
    }

    
    if (oFile.is_open())
    {
        oFile << genString << "\n";
        for (int i = 0; i < pop.mMembers.size(); ++i) {
            for(size_t j = 0; j < pop.mMembers[0].size(); ++j)
            {
                if(j != 0)
                {
                    oFile << ",";
                }
                oFile << pop.mMembers[i][j];
            }
            oFile << "\n";
        }
    }
    
}

std::vector<std::pair<int, double>> CalcFitness(Population pop, std::vector<Location> locs)
{
    std::vector<std::pair<int, double>> fitness(pop.mMembers.size());
    
    std::vector<int> indices = CreateIndices(pop.mMembers.size());
    // iterate over members of pop calculate each haversine distance
    std::transform(pop.mMembers.begin(), pop.mMembers.end(), indices.begin(), fitness.begin(), [locs](std::vector<int> mem, int i) -> std::pair<int, double>
    {
        // add first dest to last because must loop back
        mem.push_back(mem[0]);
        // create matrix of adj distances
        std::vector<double> adj_diff(mem.size(),0);
        // calc adj dist
        std::adjacent_difference(mem.begin(), mem.end(), adj_diff.begin(), [locs](int loc2, int loc1) -> double
        {
                                     return Distance(locs[loc1].mLatitude,locs[loc1].mLongitude, locs[loc2].mLatitude, locs[loc2].mLongitude);
        });
        // sum the distances
        double sumDist = std::accumulate(adj_diff.begin(), adj_diff.end(), 0.0);
        return std::pair<int, double>(i,sumDist);
    });
    return fitness;
}

void WriteFit(std::vector<std::pair<int, double>> fitnesses)
{
    std::ofstream oFile;
    std::string fitnessStr = "FITNESS: ";
    
    oFile = std::ofstream("log.txt", std::ofstream::out|std::ofstream::app);
    
    
    if (oFile.is_open())
    {
        oFile << fitnessStr << "\n";
        for (int i = 0; i < fitnesses.size(); ++i)
        {
            
            oFile << fitnesses[i].first << ":" << fitnesses[i].second << "\n";
        }
    }
}

double Deg2rad(double deg) {
    // deg to rad const
    return (deg * .0174533);
}

double Distance(double lat1d, double lon1d, double lat2d, double lon2d) {
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = Deg2rad(lat1d);
    lon1r = Deg2rad(lon1d);
    lat2r = Deg2rad(lat2d);
    lon2r = Deg2rad(lon2d);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    // km 6371
    return 2.0 * 3961 * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

std::vector<std::pair<int, int>> SelectPairs(std::vector<std::pair<int, double>> fitnesses, std::mt19937 &gen)
{
    std::vector<std::pair<int, int>> selectedPairs(fitnesses.size());
    // sort fitness vec ascending
    std::vector<std::pair<int, double>> ascFitness = fitnesses;
    std::sort(ascFitness.begin(), ascFitness.end(),
              [](std::pair<int, int> a,std::pair<int, int> b) -> bool
    {
        return a.second < b.second;
    });
    
    // create vector of uniform probability
    std::vector<double> prob(fitnesses.size(), 1.0/double(fitnesses.size()));
    
    // two individs with best fitness prob 6x, remainder of top half 3x, rest remain
    std::vector<int> indices = CreateIndices(fitnesses.size());
    std::transform(ascFitness.begin(), ascFitness.end(), indices.begin(), prob.begin(),
                   [prob](std::pair<int, int> p,int i) -> double
                      {
                          if (i < 2) {
                              return 6.0*prob[i];
                          } else if(i < prob.size()/2)
                          {
                              return 3.0*prob[i];
                          } else {
                              return 1.0*prob[i];
                          }
                      });
    
    // normalize probs
    double sumToNorm = std::accumulate(prob.begin(), prob.end(), 0.0);
    std::transform(prob.begin(), prob.end(), prob.begin(), [sumToNorm](double p) -> double
                   {
                       return p/sumToNorm;
                   });
    
    // select pairs:
        // create uniform dist
    std::uniform_real_distribution<double> uDis(0.0,1.0);
        // Generate a random double for the first parent, and use it to select an individual, from the
        // probability vector
    // memoize sums by creating vec with sum up to
    std::vector<double> iSums(prob.size(),0);
    iSums[0] = prob[0];
    std::transform(prob.begin()+1, prob.end(), iSums.begin(), iSums.begin()+1,
                   [](double curr, double prev) -> double
    {
                       return prev+curr;
    });
    
    std::transform(indices.begin(), indices.end(), selectedPairs.begin(),
                   [&uDis, &gen, iSums, indices](int i)-> std::pair<int, int>
                   {
                       double sumThresh = uDis(gen);
                       auto iSumsCopy = iSums;
                       auto indicesCopy = indices;
                       int mate = *std::find_if(indicesCopy.begin(), indicesCopy.end(), [sumThresh, iSumsCopy](int i) -> bool
                       {
                           return (sumThresh <= iSumsCopy[i]);
                       });
                       std::pair<int, int> pair(i, mate);
                       return pair;
                   });
    return selectedPairs;
}

std::vector<int> CreateIndices(int n)
{
    std::vector<int> indices(n, 0);
    std::transform(indices.begin()+1, indices.end(), indices.begin(),indices.begin()+1,
                   [](int current, int behind) -> int
                    {
                        return behind+1;
                    } );
    return indices;
}
