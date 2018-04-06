#pragma once
#include <fstream>
#include <cstdlib>
#include<string>
#include<iostream>
#include "StructDef.h"
class ProFiles
{
private:
	int FeaLength;
	int mode;
	int ReadIndex;
	std::fstream fileList;//�������ļ�
	FILE *feaBase;//�������ļ�
public:
	ProFiles(int length);
	~ProFiles();
	int OpenFiles(std::string &path, int mode);//����·����,���ļ�
	void CloseFiles();//�ر��ļ�
	int InsertOneRecord(FaceRecord1 &record);//�����¼
	int ReadOneRecord(FaceRecord1 &record);
};

