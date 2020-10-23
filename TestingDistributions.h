//
// Created by Celly on 21/10/2020.
//

#ifndef MARKOVNET_TESTINGDISTRIBUTIONS_H
#define MARKOVNET_TESTINGDISTRIBUTIONS_H

#include "Distribution.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <tuple>

bool testDist(NormalDistribution norm);
bool testDist(ExponentialDistribution exp);
bool testDist(UniformDistribution uni);
std::tuple<float, float> getMoments (std::vector <float> results);

#endif //MARKOVNET_TESTINGDISTRIBUTIONS_H
