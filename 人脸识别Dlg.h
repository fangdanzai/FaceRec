
// ����ʶ��Dlg.h : ͷ�ļ�
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
// C����ʶ��Dlg �Ի���
class C����ʶ��Dlg : public CDialogEx
{
// ����
public:
	C����ʶ��Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	cv::VideoCapture m_cam;
	cv::Mat m_mat, m_mat_process;
	cv::Mat m_matImage;
	int count = 1;//������ļ��� �������
	CString strTemp;//�ɼ������˵�����
	void DrawMatToHDC(cv::Mat img, UINT ID);//------��Ӧ�°�Mat ����ʾ����
	void OnInitDetect();//------��Ӧ�°�Mat ����ʾ����
// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
