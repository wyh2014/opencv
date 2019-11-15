#pragma once
#include "iostream"
#include "string"
using namespace std;

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp"
using namespace cv;


// Threshold 对话框

class Threshold : public CDialogEx
{
	DECLARE_DYNAMIC(Threshold)

public:
	Threshold(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Threshold();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THRESHOLD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPath0();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void DisplayZoom(Mat image, Size window, String window_name);

	Size m_window0, m_window1;
	Mat m_image0, m_image1;
	int m_type;
	CEdit m_threshold0;
	afx_msg void OnChangeThreshold0();
	CSliderCtrl m_threshold1;
	afx_msg void OnCustomdrawThresold1(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_radio1;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	
	
};

extern Threshold g_Threshold;
