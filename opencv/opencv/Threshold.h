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
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void DisplayZoom(Mat image, Size window);

	Size m_window;
	Mat m_image;
	int m_type;
	CEdit m_path;
	CEdit m_threshold;
	afx_msg void OnChangeEdit2();
	CSliderCtrl m_slider;
	afx_msg void OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	CButton m_button;
};

extern Threshold g_Threshold;
