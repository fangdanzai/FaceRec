#include "stdafx.h"
#include "ProFiles.h"


ProFiles::ProFiles(int length)
{
	FeaLength = length;
}


ProFiles::~ProFiles()
{
}
int ProFiles::OpenFiles(std::string &path, int mode)
{
	std::string fileListPath = path + "file_list.txt";//索引文件
	std::string feaBasePath = path + "feature_base.bat";//特征文件

	if (mode == 0)//只读
	{
		fileList.open(fileListPath.c_str(), std::ios::in);//读取
		feaBase = fopen(feaBasePath.c_str(), "rb");//“rt” 只读打开一个文本文件，只允许读数据
	}
	else
	{
		fileList.open(fileListPath.c_str(), std::ios::out);//写入
		feaBase = fopen(feaBasePath.c_str(), "wb");//　“wb” 只写打开或建立一个二进制文件，只允许写数据
	}
	if (!fileList || !feaBase)
	{
		std::cout << "failed to open files";
		return -1;
	}
	return 1;
}
void ProFiles::CloseFiles()
{
	if (fileList)
	{
		fileList.close();
	}
	if (feaBase)
	{
		fclose(feaBase);
	}
}
int ProFiles::InsertOneRecord(FaceRecord1 &record)
{
	if (mode == 1)
	{
		return -1;
	}
	fileList << record.ID << "  " << record.name << std::endl;//<<插入器(<<),向流输出数据
	fwrite(record.Feature, sizeof(float)*FeaLength, 1, feaBase);//feaBase目标文件指针
	std::cout << "well" << std::endl;
	return 1;
}
int ProFiles::ReadOneRecord(FaceRecord1 &record)//读取数据进入内存
{
	record.name = "";
	if (mode == 1)
	{
		std::cout << "mode is not read";
		return 0;
	}
	fileList >> record.ID >> record.name;//析取器(>>)从流中输入数据,比如说系统有一个默认的标准输入流(cin)，一般情况下就是指的键盘，所以，cin>>x;就表示从标准输入流中读取一个指定类型(即变量x的类型)的数据
	if (record.name != "")//
	{
		int i;
		record.Feature = new float[FeaLength];                       //fread如果调用成功返回实际读取到的项个数（小于或等于count）
		i = fread(record.Feature, sizeof(float)*FeaLength, 1, feaBase);//size_t fread ( void *buffer,用于接收数据的内存地址, size_t size, size_t count, FILE *stream) ;
		std::cout << "faeture read" << i << std::endl;
		std::cout << "one " << record.Feature[2] << std::endl;
	}
	else
	{
		std::cout << "in the end can not read ";
		return 0;
	}
	return 1;
}