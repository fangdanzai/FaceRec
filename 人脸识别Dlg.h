
// 人脸识别Dlg.h : 头文件
//

#pragma once
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include "cv.h"
#include "CvvImage.h"
#include "stdafx.h"
#include "face_detection.h"
#include "face_alignment.h"
#include "face_identification.h"
// C人脸识别Dlg 对话框
class C人脸识别Dlg : public CDialogEx
{
// 构造
public:
	C人脸识别Dlg(CWnd* pParent = NULL);	// 标准构造函数
	cv::VideoCapture m_cam;
	cv::Mat m_mat, m_mat_process;
	cv::Mat m_matImage;
	int count = 1;//保存的文件名 后的数字
	CString strTemp;//采集样本人的名字
	void DrawMatToHDC(cv::Mat img, UINT ID);//------对应新版Mat 的显示函数
	void OnInitDetect();//------对应新版Mat 的显示函数
// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenvideo();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCollection();
	afx_msg void OnBnClickedSnap();
	afx_msg void OnBnClickedClosecam();
	afx_msg void OnBnClickedClosesystem();
	afx_msg void OnBnClickedOpenimage();
	afx_msg void OnBnClickedFacerec();
};
