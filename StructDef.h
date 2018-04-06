#ifndef _FRUTILITY_H_
#define _FRUTILITY_H_
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
struct FaceRecord1
{
	int ID;
	std::string name;
	float *Feature;
};

struct FacePairSim1
{
	int ID;
	std::string name;
	float Similarity;
};

#endif
