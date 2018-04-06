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
	std::string fileListPath = path + "file_list.txt";//�����ļ�
	std::string feaBasePath = path + "feature_base.bat";//�����ļ�

	if (mode == 0)//ֻ��
	{
		fileList.open(fileListPath.c_str(), std::ios::in);//��ȡ
		feaBase = fopen(feaBasePath.c_str(), "rb");//��rt�� ֻ����һ���ı��ļ���ֻ���������
	}
	else
	{
		fileList.open(fileListPath.c_str(), std::ios::out);//д��
		feaBase = fopen(feaBasePath.c_str(), "wb");//����wb�� ֻд�򿪻���һ���������ļ���ֻ����д����
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
	fileList << record.ID << "  " << record.name << std::endl;//<<������(<<),�����������
	fwrite(record.Feature, sizeof(float)*FeaLength, 1, feaBase);//feaBaseĿ���ļ�ָ��
	std::cout << "well" << std::endl;
	return 1;
}
int ProFiles::ReadOneRecord(FaceRecord1 &record)//��ȡ���ݽ����ڴ�
{
	record.name = "";
	if (mode == 1)
	{
		std::cout << "mode is not read";
		return 0;
	}
	fileList >> record.ID >> record.name;//��ȡ��(>>)��������������,����˵ϵͳ��һ��Ĭ�ϵı�׼������(cin)��һ������¾���ָ�ļ��̣����ԣ�cin>>x;�ͱ�ʾ�ӱ�׼�������ж�ȡһ��ָ������(������x������)������
	if (record.name != "")//
	{
		int i;
		record.Feature = new float[FeaLength];                       //fread������óɹ�����ʵ�ʶ�ȡ�����������С�ڻ����count��
		i = fread(record.Feature, sizeof(float)*FeaLength, 1, feaBase);//size_t fread ( void *buffer,���ڽ������ݵ��ڴ��ַ, size_t size, size_t count, FILE *stream) ;
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