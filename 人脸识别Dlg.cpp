
// ����ʶ��Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "����ʶ��.h"
#include "����ʶ��Dlg.h"
#include "afxdialogex.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "CvvImage.h"
#include <fstream>
#include <string>
#include <shellapi.h>
#include <windows.h>
#include <io.h> 
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "ProFiles.h"
#include "StructDef.h"
#include "Re.h"
#include <direct.h>//�����ļ���
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FeatureLength 2048
static int NUM = 1;

using namespace std;
using namespace cv;
using namespace seeta;

//��MFC��ģ��ֻ�ܼ���һ�Σ���Ȼ��������һ��ʱ��ͻ�����
seeta::FaceDetection detector("..\\FaceDetectionModel\\seeta_fd_frontal_v1.0.bin");//�������

// Initialize face alignment model 
seeta::FaceAlignment point_detector("..\\FaceAlignmentModel\\seeta_fa_v1.1.bin");//��������

// Initialize face Identification model 
FaceIdentification face_recognizer("..\\FaceRecognizeModel\\seeta_fr_v1.0.bin");

Recognition recognition;//��ʼ����ʱ���ȡ����


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C����ʶ��Dlg �Ի���



C����ʶ��Dlg::C����ʶ��Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C����ʶ��Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C����ʶ��Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C����ʶ��Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenVideo, &C����ʶ��Dlg::OnBnClickedOpenvideo)
	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Save, &C����ʶ��Dlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_Collection, &C����ʶ��Dlg::OnBnClickedCollection)
	ON_BN_CLICKED(IDC_Snap, &C����ʶ��Dlg::OnBnClickedSnap)
	ON_BN_CLICKED(IDC_CloseCam, &C����ʶ��Dlg::OnBnClickedClosecam)
	ON_BN_CLICKED(IDC_CloseSystem, &C����ʶ��Dlg::OnBnClickedClosesystem)
	ON_BN_CLICKED(IDC_OpenImage, &C����ʶ��Dlg::OnBnClickedOpenimage)
	ON_BN_CLICKED(IDC_FaceRec, &C����ʶ��Dlg::OnBnClickedFacerec)
END_MESSAGE_MAP()


// C����ʶ��Dlg ��Ϣ�������

BOOL C����ʶ��Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C����ʶ��Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C����ʶ��Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C����ʶ��Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void C����ʶ��Dlg::DrawMatToHDC(cv::Mat img, UINT ID)//------��Ӧ�°�Mat ����ʾ����
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	IplImage ipm = img;
	cimg.CopyOf(&ipm);
	cimg.DrawToHDC_SCALE(hDC, &rect);
	ReleaseDC(pDC);
}
void C����ʶ��Dlg::OnInitDetect()//------��ʼ���ڴ�ͼ����
{
	std::string files_dir = "..\\seetafile\\";  //��ʼ���Ķ�ȡ��Ϣ���ڴ�
	ProFiles profiles(FeatureLength);
	profiles.OpenFiles(files_dir, 0);
	recognition.SetFaceRecord(profiles);

	detector.SetMinFaceSize(100);
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);
}
void C����ʶ��Dlg::OnBnClickedOpenvideo()//dakaishexingtou
{
	
	if (m_cam.isOpened())
	{
		m_cam.release();
	}
	if (!(m_cam.open(0)))
	{
		MessageBox("�޷�������ͷ");
	}
	else
	{
		// ��ʾͼ�����
		m_cam >> m_mat;
		/*char chEdit[10];*/
		/*_itoa(m_mat1.cols, chEdit, 10);
		SetDlgItemText(IDC_EDIT_Realw, chEdit);
		_itoa(m_mat1.rows, chEdit, 10);
		SetDlgItemText(IDC_EDIT_Realh, chEdit);*/
		OnInitDetect();
		// ˢ����ʾ��
		Invalidate(TRUE);
		// ���ö�ʱ��
		SetTimer(1, 20, NULL);
	}
}

void C����ʶ��Dlg::OnBnClickedClosecam()
{
	KillTimer(1);
	m_cam.release();
	Mat src = imread("..\\images\\Close.jpg");
	DrawMatToHDC(src, IDC_ShowVideo);
}
void C����ʶ��Dlg::OnTimer(UINT_PTR nIDEvent)
{
	m_cam >> m_mat;
	if (nIDEvent == 1)
	{
		std::string name;

		float tDestLen = 1000.0; //���ź�ĶԽ��߳���
		float scale = float(tDestLen) / (m_mat.rows + m_mat.cols);
		float newh = scale * m_mat.rows;
		float neww = scale * m_mat.cols;

		cv::Mat img;
		resize(m_mat, img, cv::Size(neww, newh), 0, 0, CV_INTER_LINEAR);
		float *feature;
		feature = new float[FeatureLength];
		cv::Mat gallery_img_gray;
		cv::cvtColor(img, gallery_img_gray, CV_BGR2GRAY);

		seeta::ImageData gallery_img_data_color(img.cols, img.rows, img.channels());
		gallery_img_data_color.data = img.data;

		seeta::ImageData gallery_img_data_gray(gallery_img_gray.cols, gallery_img_gray.rows, gallery_img_gray.channels());
		gallery_img_data_gray.data = gallery_img_gray.data;
		// Detect faces�������
		std::vector<seeta::FaceInfo> gallery_faces = detector.Detect(gallery_img_data_gray);
		int32_t gallery_face_num = static_cast<int32_t>(gallery_faces.size());

		if (gallery_face_num == 0)
		{
			std::cout << "Faces are not detected.\n";
		}
		else
		{
			for (int i = 0; i < gallery_face_num; i++)
			{
				// Detect 5 facial landmarks���
				seeta::FacialLandmark gallery_points[5];
				point_detector.PointDetectLandmarks(gallery_img_data_gray, gallery_faces[i], gallery_points);

				// Extract face identity feature��������
				face_recognizer.ExtractFeatureWithCrop(gallery_img_data_color, gallery_points, feature);//feature���������

				name = recognition.SeachFeature(feature, face_recognizer);//����ʶ�������

				//����
				cv::Rect face_rect;
				face_rect.x = gallery_faces[i].bbox.x;
				face_rect.y = gallery_faces[i].bbox.y;
				face_rect.width = gallery_faces[i].bbox.width;
				face_rect.height = gallery_faces[i].bbox.height;

				cv::rectangle(img, face_rect, cvScalar((0), (0), (255)), 4, 8, 0);
				putText(img, name, cvPoint(gallery_faces[i].bbox.x, gallery_faces[i].bbox.y), 4, 1, cv::Scalar(0, 255, 0));
			}
		}
		DrawMatToHDC(img, IDC_ShowVideo);
	}
	if (nIDEvent == 2)
	{
		float tDestLen = 1000.0; //���ź�ĶԽ��߳���
		float scale = float(tDestLen) / (m_mat.rows + m_mat.cols);
		float newh = scale * m_mat.rows;
		float neww = scale * m_mat.cols;

		cv::Mat img;
		resize(m_mat, img, cv::Size(neww, newh), 0, 0, CV_INTER_LINEAR);
		float *feature;
		feature = new float[FeatureLength];
		cv::Mat gallery_img_gray;
		cv::cvtColor(img, gallery_img_gray, CV_BGR2GRAY);

		seeta::ImageData gallery_img_data_color(img.cols, img.rows, img.channels());
		gallery_img_data_color.data = img.data;

		seeta::ImageData gallery_img_data_gray(gallery_img_gray.cols, gallery_img_gray.rows, gallery_img_gray.channels());
		gallery_img_data_gray.data = gallery_img_gray.data;
		// Detect faces�������
		std::vector<seeta::FaceInfo> gallery_faces = detector.Detect(gallery_img_data_gray);
		int32_t gallery_face_num = static_cast<int32_t>(gallery_faces.size());

		if (gallery_face_num == 0)
		{
			std::cout << "Faces are not detected.\n";
		}
		else
		{
			for (int i = 0; i < gallery_face_num; i++)
			{
				// Detect 5 facial landmarks���

				//����
				cv::Rect face_rect;
				face_rect.x = gallery_faces[i].bbox.x;
				face_rect.y = gallery_faces[i].bbox.y;
				face_rect.width = gallery_faces[i].bbox.width;
				face_rect.height = gallery_faces[i].bbox.height;
				cv::rectangle(img, face_rect, cvScalar((0), (0), (255)), 4, 8, 0);
			}
		}
		DrawMatToHDC(img, IDC_ShowVideo);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void C����ʶ��Dlg::OnBnClickedSave()//�����洢
{
	KillTimer(2);
	Mat src = imread("..\\images\\ѵ��.jpg");
	DrawMatToHDC(src, IDC_ShowVideo);
	m_cam.release();

	std::string Path = "..\\seetafile\\";
	std::string test_dir = "..\\camera\\";
	ProFiles proFiles(2048); //ProFiles�Ƕ����һ����
	proFiles.OpenFiles(Path, 2);//OpenFiles��ProFiles���еĺ���

	std::ifstream ifs;
	std::string img_name;
	ifs.open(test_dir + "test_file_list.txt", std::ifstream::in);
	while (ifs >> img_name)
	{
		cout << img_name + "\n";
		char numImg_name[60];
		//read image
		int i = 1;
		while (1)
		{
			sprintf(numImg_name, "%s%\\\%(%d).jpg", img_name.c_str(), i);//��a,b,a+b�Եڶ�����������ʽд��numImg_name,buffer
			i++;
			//load image
			cv::Mat SrcImage;
			cv::Mat SrcImage0 = cv::imread(test_dir + numImg_name, 1);
			if (!SrcImage0.data)
			{
				cout << "Photos have been  deposited\n";// depositedΪ����
				break;
			}
			float tDestLen = 1000.0; //���ź�ĶԽ��߳���
			float scale = float(tDestLen) / (SrcImage0.rows + SrcImage0.cols);
			float newh = scale * SrcImage0.rows;
			float neww = scale * SrcImage0.cols;

			resize(SrcImage0, SrcImage, cv::Size(neww, newh), 0, 0, CV_INTER_LINEAR);
			cout << test_dir + numImg_name << "!!!";


			cv::Mat gallery_img_gray;
			cv::cvtColor(SrcImage, gallery_img_gray, CV_BGR2GRAY);

			seeta::ImageData gallery_img_data_color(SrcImage.cols, SrcImage.rows, SrcImage.channels());
			gallery_img_data_color.data = SrcImage.data;

			seeta::ImageData gallery_img_data_gray(gallery_img_gray.cols, gallery_img_gray.rows, gallery_img_gray.channels());
			gallery_img_data_gray.data = gallery_img_gray.data;


			//Detect faces�������
			std::vector<seeta::FaceInfo> gallery_faces = detector.Detect(gallery_img_data_gray);
			int32_t gallery_face_num = static_cast<int32_t>(gallery_faces.size());
			//Detect 5 facial landmarks���
			seeta::FacialLandmark gallery_points[5];
			point_detector.PointDetectLandmarks(gallery_img_data_gray, gallery_faces[0], gallery_points);

			// Extract face identity feature��������
			float gallery_fea[2048];
			face_recognizer.ExtractFeatureWithCrop(gallery_img_data_color, gallery_points, gallery_fea);

			FaceRecord1 faceRecord;
			faceRecord.ID = NUM;
			faceRecord.name = img_name;
			faceRecord.Feature = gallery_fea;
			NUM++;
			proFiles.InsertOneRecord(faceRecord);
		}
	}
	ifs.close();
	proFiles.CloseFiles();
	AfxMessageBox("Train finished");
}
void C����ʶ��Dlg::OnBnClickedCollection()//�ɼ�����
{
	GetDlgItem(IDC_m_Name)->GetWindowText(strTemp);
	if (strTemp != "")
	{
		int status;
		status = _mkdir("..\\camera\\" + strTemp); //�����ڳ������ڵ�ǰ�ļ����´���bookstatus == 0����ʾ������gong
		if (status == 0)
		{
			ofstream outfile("..\\camera\\test_file_list.txt", ios::app);//ios::app��ʾ��ԭ�ļ�ĩβ׷��,дtxt
			if (!outfile)
			{
				cout << "Open the file failure...\n";
				exit(0);
			}
			outfile << ' ' << strTemp << std::endl;//��MFC�п���ֱ��������CString���͵��ַ�����ʵ����ĩβ�����ַ�
		}
		if (!(m_cam.open(0)))
		{
			MessageBox("Can not open camera");
		}
		else
		{
			// ��ʾͼ�����
			m_cam >> m_mat;
			detector.SetMinFaceSize(100);
			detector.SetScoreThresh(2.f);
			detector.SetImagePyramidScaleFactor(0.8f);
			detector.SetWindowStep(4, 4);

			// ˢ����ʾ��
			Invalidate(TRUE);
			// ���ö�ʱ��
			SetTimer(2, 40, NULL);
		}
	}
	else
		AfxMessageBox("Please enter name");
	
}

void C����ʶ��Dlg::OnBnClickedSnap()//��ͼ
{
	CString outfile;
	CString ff;
	ff.Format("%d", count);
	outfile = "..\\camera\\" + strTemp +"\\" + "(" + ff + ")"  ".jpg"; //Ϊ��ʵ�� ������ļ�������  save0.jpg save1.jpg ...
	IplImage *SaveImage;
	SaveImage = &IplImage(m_mat);  //mat->Iplimage
	cvSaveImage(outfile, SaveImage);
	count++;
}

void C����ʶ��Dlg::OnBnClickedClosesystem()//�ر�ϵͳ
{
	KillTimer(1);
	KillTimer(2);
	m_cam.release();
	C����ʶ��Dlg::OnCancel();
}

void C����ʶ��Dlg::OnBnClickedOpenimage()//��ͼƬ
{
	KillTimer(1);
	KillTimer(2);
	m_cam.release();
	Mat src = imread("..\\images\\Close.jpg");
	DrawMatToHDC(src, IDC_ShowVideo);
	
	// ���ù�����   
	TCHAR szFilter[] = _T("ͼ��(*.jpg)|*.jpg|ͼ��(*.jpeg)|*.jpeg|ͼ��(*.png)|*.png|ͼ��(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||");
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("jpg|jpeg|png|bmp|gif"), NULL, 0, szFilter, this);
	CString strFilePath;
	float tDestLen = 1000.0; //���ź�ĶԽ��߳���
	//CImage cimg;
	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		strFilePath = fileDlg.GetPathName();
		string tPath = CStringA(strFilePath);
		//showPicByPath(strFilePath, IDC_PIC1); 
		Mat img0 = imread(tPath);
		float scale = float(tDestLen) / (img0.rows + img0.cols);  //��ͼƬ�������ţ�������ʱ���й�
		float newh = scale * img0.rows;
		float neww = scale * img0.cols;
		resize(img0, m_matImage, Size(neww, newh), 0, 0, CV_INTER_LINEAR);
		DrawMatToHDC(m_matImage, IDC_ShowVideo);//��ʾͼƬ
	}
	std::string files_dir = "..\\seetafile\\";  //��ʼ���Ķ�ȡ��Ϣ���ڴ�
	ProFiles profiles(FeatureLength);
	profiles.OpenFiles(files_dir, 0);
	recognition.SetFaceRecord(profiles);

	detector.SetMinFaceSize(20);//���õ�ԽС���ʱ��Խ��������С��Ҳ���Լ�⵽
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);
}
void C����ʶ��Dlg::OnBnClickedFacerec()//��̬ͼƬ����ʶ��
{
	float *feature;
	std::string name;
	feature = new float[FeatureLength];
	cv::Mat gallery_img_gray;
	cv::cvtColor(m_matImage, gallery_img_gray, CV_BGR2GRAY);

	seeta::ImageData gallery_img_data_color(m_matImage.cols, m_matImage.rows, m_matImage.channels());
	gallery_img_data_color.data = m_matImage.data;

	seeta::ImageData gallery_img_data_gray(gallery_img_gray.cols, gallery_img_gray.rows, gallery_img_gray.channels());
	gallery_img_data_gray.data = gallery_img_gray.data;
	// Detect faces�������
	std::vector<seeta::FaceInfo> gallery_faces = detector.Detect(gallery_img_data_gray);
	int32_t gallery_face_num = static_cast<int32_t>(gallery_faces.size());

	if (gallery_face_num == 0)
	{
		AfxMessageBox("No face detected");
	}
	else
	{
		for (int i = 0; i < gallery_face_num; i++)
		{
			// Detect 5 facial landmarks���
			seeta::FacialLandmark gallery_points[5];
			point_detector.PointDetectLandmarks(gallery_img_data_gray, gallery_faces[i], gallery_points);

			// Extract face identity feature��������
			face_recognizer.ExtractFeatureWithCrop(gallery_img_data_color, gallery_points, feature);//feature���������

			name = recognition.SeachFeature(feature, face_recognizer);//����ʶ�������

			//����
			cv::Rect face_rect;
			face_rect.x = gallery_faces[i].bbox.x;
			face_rect.y = gallery_faces[i].bbox.y;
			face_rect.width = gallery_faces[i].bbox.width;
			face_rect.height = gallery_faces[i].bbox.height;

			cv::rectangle(m_matImage, face_rect, cvScalar((0), (0), (255)), 4, 8, 0);
			putText(m_matImage, name, cvPoint(gallery_faces[i].bbox.x, gallery_faces[i].bbox.y), 4, 1, cv::Scalar(0, 255, 0));
		}
	}
	DrawMatToHDC(m_matImage, IDC_ShowVideo);
}
