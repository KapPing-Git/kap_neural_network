#ifndef NETWORK_MAIN_H
#define NETWORK_MAIN_H

#include<vector>

using uint = unsigned int;
using AFeatures = std::vector<double>;

struct ADataset
{
  std::vector<AFeatures> features;
  std::vector<uint> num_class;
};

#endif // NETWORK_MAIN_H
