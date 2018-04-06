
// 人脸识别Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "人脸识别.h"
#include "人脸识别Dlg.h"
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
#include <direct.h>//创建文件夹
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FeatureLength 2048
static int NUM = 1;

using namespace std;
using namespace cv;
using namespace seeta;

//在MFC下模型只能加载一次，不然程序运行一段时间就会死掉
seeta::FaceDetection detector("..\\FaceDetectionModel\\seeta_fd_frontal_v1.0.bin");//人脸检测

// Initialize face alignment model 
seeta::FaceAlignment point_detector("..\\FaceAlignmentModel\\seeta_fa_v1.1.bin");//特征点检测

// Initialize face Identification model 
FaceIdentification face_recognizer("..\\FaceRecognizeModel\\seeta_fr_v1.0.bin");

Recognition recognition;//初始化的时候读取数据


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// C人脸识别Dlg 对话框



C人脸识别Dlg::C人脸识别Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C人脸识别Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C人脸识别Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C人脸识别Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenVideo, &C人脸识别Dlg::OnBnClickedOpenvideo)
	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Save, &C人脸识别Dlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_Collection, &C人脸识别Dlg::OnBnClickedCollection)
	ON_BN_CLICKED(IDC_Snap, &C人脸识别Dlg::OnBnClickedSnap)
	ON_BN_CLICKED(IDC_CloseCam, &C人脸识别Dlg::OnBnClickedClosecam)
	ON_BN_CLICKED(IDC_CloseSystem, &C人脸识别Dlg::OnBnClickedClosesystem)
	ON_BN_CLICKED(IDC_OpenImage, &C人脸识别Dlg::OnBnClickedOpenimage)
	ON_BN_CLICKED(IDC_FaceRec, &C人脸识别Dlg::OnBnClickedFacerec)
END_MESSAGE_MAP()


// C人脸识别Dlg 消息处理程序

BOOL C人脸识别Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C人脸识别Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C人脸识别Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C人脸识别Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void C人脸识别Dlg::DrawMatToHDC(cv::Mat img, UINT ID)//------对应新版Mat 的显示函数
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
void C人脸识别Dlg::OnInitDetect()//------初始化内存和检测器
{
	std::string files_dir = "..\\seetafile\\";  //初始化的读取信息至内存
	ProFiles profiles(FeatureLength);
	profiles.OpenFiles(files_dir, 0);
	recognition.SetFaceRecord(profiles);

	detector.SetMinFaceSize(100);
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);
}
void C人脸识别Dlg::OnBnClickedOpenvideo()//dakaishexingtou
{
	
	if (m_cam.isOpened())
	{
		m_cam.release();
	}
	if (!(m_cam.open(0)))
	{
		MessageBox("无法打开摄像头");
	}
	else
	{
		// 显示图像参数
		m_cam >> m_mat;
		/*char chEdit[10];*/
		/*_itoa(m_mat1.cols, chEdit, 10);
		SetDlgItemText(IDC_EDIT_Realw, chEdit);
		_itoa(m_mat1.rows, chEdit, 10);
		SetDlgItemText(IDC_EDIT_Realh, chEdit);*/
		OnInitDetect();
		// 刷新显示区
		Invalidate(TRUE);
		// 设置定时器
		SetTimer(1, 20, NULL);
	}
}

void C人脸识别Dlg::OnBnClickedClosecam()
{
	KillTimer(1);
	m_cam.release();
	Mat src = imread("..\\images\\Close.jpg");
	DrawMatToHDC(src, IDC_ShowVideo);
}
void C人脸识别Dlg::OnTimer(UINT_PTR nIDEvent)
{
	m_cam >> m_mat;
	if (nIDEvent == 1)
	{
		std::string name;

		float tDestLen = 1000.0; //缩放后的对角线长度
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
		// Detect faces检测人脸
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
				// Detect 5 facial landmarks五点
				seeta::FacialLandmark gallery_points[5];
				point_detector.PointDetectLandmarks(gallery_img_data_gray, gallery_faces[i], gallery_points);

				// Extract face identity feature特征向量
				face_recognizer.ExtractFeatureWithCrop(gallery_img_data_color, gallery_points, feature);//feature待检测人脸

				name = recognition.SeachFeature(feature, face_recognizer);//返回识别的姓名

				//画框
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
		float tDestLen = 1000.0; //缩放后的对角线长度
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
		// Detect faces检测人脸
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
				// Detect 5 facial landmarks五点

				//画框
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


void C人脸识别Dlg::OnBnClickedSave()//特征存储
{
	KillTimer(2);
	Mat src = imread("..\\images\\训练.jpg");
	DrawMatToHDC(src, IDC_ShowVideo);
	m_cam.release();

	std::string Path = "..\\seetafile\\";
	std::string test_dir = "..\\camera\\";
	ProFiles proFiles(2048); //ProFiles是定义的一个类
	proFiles.OpenFiles(Path, 2);//OpenFiles是ProFiles类中的函数

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
			sprintf(numImg_name, "%s%\\\%(%d).jpg", img_name.c_str(), i);//把a,b,a+b以第二个参数的形式写入numImg_name,buffer
			i++;
			//load image
			cv::Mat SrcImage;
			cv::Mat SrcImage0 = cv::imread(test_dir + numImg_name, 1);
			if (!SrcImage0.data)
			{
				cout << "Photos have been  deposited\n";// deposited为储存
				break;
			}
			float tDestLen = 1000.0; //缩放后的对角线长度
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


			//Detect faces检测人脸
			std::vector<seeta::FaceInfo> gallery_faces = detector.Detect(gallery_img_data_gray);
			int32_t gallery_face_num = static_cast<int32_t>(gallery_faces.size());
			//Detect 5 facial landmarks五点
			seeta::FacialLandmark gallery_points[5];
			point_detector.PointDetectLandmarks(gallery_img_data_gray, gallery_faces[0], gallery_points);

			// Extract face identity feature特征向量
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
void C人脸识别Dlg::OnBnClickedCollection()//采集样本
{
	GetDlgItem(IDC_m_Name)->GetWindowText(strTemp);
	if (strTemp != "")
	{
		int status;
		status = _mkdir("..\\camera\\" + strTemp); //这是在程序所在当前文件夹下创建bookstatus == 0，表示创建成gong
		if (status == 0)
		{
			ofstream outfile("..\\camera\\test_file_list.txt", ios::app);//ios::app表示在原文件末尾追加,写txt
			if (!outfile)
			{
				cout << "Open the file failure...\n";
				exit(0);
			}
			outfile << ' ' << strTemp << std::endl;//在MFC中可以直接声明称CString类型的字符串，实现在末尾加入字符
		}
		if (!(m_cam.open(0)))
		{
			MessageBox("Can not open camera");
		}
		else
		{
			// 显示图像参数
			m_cam >> m_mat;
			detector.SetMinFaceSize(100);
			detector.SetScoreThresh(2.f);
			detector.SetImagePyramidScaleFactor(0.8f);
			detector.SetWindowStep(4, 4);

			// 刷新显示区
			Invalidate(TRUE);
			// 设置定时器
			SetTimer(2, 40, NULL);
		}
	}
	else
		AfxMessageBox("Please enter name");
	
}

void C人脸识别Dlg::OnBnClickedSnap()//截图
{
	CString outfile;
	CString ff;
	ff.Format("%d", count);
	outfile = "..\\camera\\" + strTemp +"\\" + "(" + ff + ")"  ".jpg"; //为了实现 保存的文件不重名  save0.jpg save1.jpg ...
	IplImage *SaveImage;
	SaveImage = &IplImage(m_mat);  //mat->Iplimage
	cvSaveImage(outfile, SaveImage);
	count++;
}

void C人脸识别Dlg::OnBnClickedClosesystem()//关闭系统
{
	KillTimer(1);
	KillTimer(2);
	m_cam.release();
	C人脸识别Dlg::OnCancel();
}

void C人脸识别Dlg::OnBnClickedOpenimage()//打开图片
{
	KillTimer(1);
	KillTimer(2);
	m_cam.release();
	Mat src = imread("..\\images\\Close.jpg");
	DrawMatToHDC(src, IDC_ShowVideo);
	
	// 设置过滤器   
	TCHAR szFilter[] = _T("图像(*.jpg)|*.jpg|图像(*.jpeg)|*.jpeg|图像(*.png)|*.png|图像(*.bmp)|*.bmp|所有文件(*.*)|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("jpg|jpeg|png|bmp|gif"), NULL, 0, szFilter, this);
	CString strFilePath;
	float tDestLen = 1000.0; //缩放后的对角线长度
	//CImage cimg;
	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();
		string tPath = CStringA(strFilePath);
		//showPicByPath(strFilePath, IDC_PIC1); 
		Mat img0 = imread(tPath);
		float scale = float(tDestLen) / (img0.rows + img0.cols);  //对图片进行缩放，与运行时间有关
		float newh = scale * img0.rows;
		float neww = scale * img0.cols;
		resize(img0, m_matImage, Size(neww, newh), 0, 0, CV_INTER_LINEAR);
		DrawMatToHDC(m_matImage, IDC_ShowVideo);//显示图片
	}
	std::string files_dir = "..\\seetafile\\";  //初始化的读取信息至内存
	ProFiles profiles(FeatureLength);
	profiles.OpenFiles(files_dir, 0);
	recognition.SetFaceRecord(profiles);

	detector.SetMinFaceSize(20);//设置的越小检测时间越长，但是小脸也可以检测到
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);
}
void C人脸识别Dlg::OnBnClickedFacerec()//静态图片人脸识别
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
	// Detect faces检测人脸
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
			// Detect 5 facial landmarks五点
			seeta::FacialLandmark gallery_points[5];
			point_detector.PointDetectLandmarks(gallery_img_data_gray, gallery_faces[i], gallery_points);

			// Extract face identity feature特征向量
			face_recognizer.ExtractFeatureWithCrop(gallery_img_data_color, gallery_points, feature);//feature待检测人脸

			name = recognition.SeachFeature(feature, face_recognizer);//返回识别的姓名

			//画框
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
