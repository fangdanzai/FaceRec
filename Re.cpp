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
		faceRecord.push_back(record);//c++�е�vectorͷ�ļ�����������push_back��������vector��������Ϊ��vectorβ������һ�����ݡ�
		std::cout << "zaicun" << record.ID << std::endl;
	}
	return 0;
}

std::string  Recognition::SeachFeature(float*Feature, seeta::FaceIdentification &face_recognizer)
{
	std::string name = "Unknows ";
	FacePairSim1 facepairsim;//�����Ľṹ��洢�ȶԵ������Ϣ
	for (int i = 0; i < faceRecord.size(); i++)
	{
		facepairsim.Similarity =
			face_recognizer.CalcSimilarity(Feature, faceRecord[i].Feature);
		facepairsim.ID = faceRecord[i].ID;
		facepairsim.name = faceRecord[i].name;
		facePairSim.push_back(facepairsim);//�ѱȶԵ������Ϣ��ֵ��facePairSim
	}
	//nth_element�Ը�����Χ����"����"
	std::nth_element(facePairSim.begin(), facePairSim.begin() + 1, facePairSim.end(), SortCompare);//��cosdis���������ߵ�[��Ҫ�Ա���ֵ]����SRClabel

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