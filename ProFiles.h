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
	std::fstream fileList;//读索引文件
	FILE *feaBase;//读特征文件
public:
	ProFiles(int length);
	~ProFiles();
	int OpenFiles(std::string &path, int mode);//传入路径名,打开文件
	void CloseFiles();//关闭文件
	int InsertOneRecord(FaceRecord1 &record);//存入记录
	int ReadOneRecord(FaceRecord1 &record);
};

