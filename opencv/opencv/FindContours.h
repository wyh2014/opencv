#pragma once
#include "iostream"
#include "string"
using namespace std;

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

// FindContours 对话框

class FindContours : public CDialogEx
{
	DECLARE_DYNAMIC(FindContours)

public:
	FindContours(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~FindContours();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_FINDCONTOURS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedFcTh0();

	Size m_window0, m_window1;
	Mat m_image0, m_image1, m_image2;
	int m_threshold, m_type, m_mode, m_method;
	void DisplayZoom(Mat image, Size window, String window_name);
	
	
	afx_msg void OnEnChangeFcTh1();
	CComboBox m_th2;
	CButton m_mode0;
	CButton m_method0;
	afx_msg void OnCbnSelchangeFcTh2();
	afx_msg void OnBnClickedFcMode0();
	afx_msg void OnBnClickedFcMode1();
	afx_msg void OnBnClickedFcMode2();
	afx_msg void OnBnClickedFcMode3();
	afx_msg void OnBnClickedFcMode4();
	afx_msg void OnBnClickedFcMethod0();
	afx_msg void OnBnClickedFcMethod1();
	afx_msg void OnBnClickedFcMethod2();
	afx_msg void OnBnClickedFcMethod3();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_slider1;
	CEdit m_edit_th1;
};

extern FindContours g_FindContours;
