#include "face_identification.h"
#include "recognizer.h"
#include"ProFiles.h"
#include<io.h> 
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include"StructDef.h"
class Recognition
{
public:
	Recognition();
	~Recognition();
	int SetFaceRecord(ProFiles &profiles);
	std::string SeachFeature(float*Feature, seeta::FaceIdentification &face_recognizer);
private:
	std::vector<FacePairSim1> facePairSim;//vector里面存储着所有的姓名，ID，相似度
	std::vector<FaceRecord1> faceRecord;//把信息读取到内存中
};

