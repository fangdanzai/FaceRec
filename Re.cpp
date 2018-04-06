#include "stdafx.h"
#include "Re.h"


Recognition::Recognition()
{
}


Recognition::~Recognition()
{
}
bool SortCompare(FacePairSim1 num1, FacePairSim1 num2)
{
	return num1.Similarity > num2.Similarity;
}

int Recognition::SetFaceRecord(ProFiles &profiles)
{

	FaceRecord1 record;
	while (profiles.ReadOneRecord(record))
	{
		faceRecord.push_back(record);//c++中的vector头文件里面就有这个push_back函数，在vector类中作用为在vector尾部加入一个数据。
		std::cout << "zaicun" << record.ID << std::endl;
	}
	return 0;
}

std::string  Recognition::SeachFeature(float*Feature, seeta::FaceIdentification &face_recognizer)
{
	std::string name = "Unknows ";
	FacePairSim1 facepairsim;//声明的结构体存储比对的相关信息
	for (int i = 0; i < faceRecord.size(); i++)
	{
		facepairsim.Similarity =
			face_recognizer.CalcSimilarity(Feature, faceRecord[i].Feature);
		facepairsim.ID = faceRecord[i].ID;
		facepairsim.name = faceRecord[i].name;
		facePairSim.push_back(facepairsim);//把比对的相关信息赋值给facePairSim
	}
	//nth_element对给定范围进行"排序"
	std::nth_element(facePairSim.begin(), facePairSim.begin() + 1, facePairSim.end(), SortCompare);//对cosdis排序，求得最高的[需要对比阈值]返回SRClabel

	if (facePairSim[0].Similarity < 0.6)
	{
		facePairSim.clear();
		return name;
	}
	else
	{
		name = facePairSim[0].name;
		facePairSim.clear();
		return name;
	}
}