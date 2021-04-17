#ifndef NETWORK_MAIN_H
#define NETWORK_MAIN_H

//#include<vector>
#include<QVector>

using uint = unsigned int;
using AFeatures = QVector<double>;

struct ADataset
{
  QVector<AFeatures> features;
  QVector<uint> num_class;
};


#endif // NETWORK_MAIN_H
