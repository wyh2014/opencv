// Threshold.cpp: 实现文件
//

#include "stdafx.h"
#include "opencv.h"
#include "Threshold.h"
#include "afxdialogex.h"

Threshold g_Threshold = new Threshold;//阈值界面
// Threshold 对话框

IMPLEMENT_DYNAMIC(Threshold, CDialogEx)

Threshold::Threshold(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THRESHOLD, pParent)
{

}

Threshold::~Threshold()
{
}

void Threshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TH_THRESHOLD0, m_threshold0);
	DDX_Control(pDX, IDC_TH_THRESHOLD1, m_threshold1);
	DDX_Control(pDX, IDC_TH_RADIO1, m_radio1);
}


BEGIN_MESSAGE_MAP(Threshold, CDialogEx)
	ON_BN_CLICKED(IDC_TH_PATH0, &Threshold::OnBnClickedPath0)
	ON_EN_CHANGE(IDC_TH_THRESHOLD0, &Threshold::OnChangeThreshold0)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TH_THRESHOLD1, &Threshold::OnCustomdrawThresold1)
	ON_BN_CLICKED(IDC_TH_RADIO1, &Threshold::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_TH_RADIO2, &Threshold::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_TH_RADIO3, &Threshold::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_TH_RADIO4, &Threshold::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_TH_RADIO5, &Threshold::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_TH_RADIO6, &Threshold::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_TH_RADIO7, &Threshold::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_TH_RADIO8, &Threshold::OnBnClickedRadio8)
END_MESSAGE_MAP()


// Threshold 消息处理程序


BOOL Threshold::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_threshold0.SetWindowTextW(L"0");
	GetDlgItem(IDC_TH_PATH1)->SetWindowText(L"");
	
	m_threshold1.SetRange(0, 255);//设置范围
	m_threshold1.SetTicFreq(1);//设置显示刻度的间隔
	m_threshold1.SetPos(0);//当前停留的位置
	m_type = THRESH_BINARY;
	m_radio1.SetCheck(1);


	HWND hWnd, hParent;
	CRect rect;
	namedWindow("image0");
	hWnd = (HWND)cvGetWindowHandle("image0");// MFC嵌套opencv窗口
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_TH_IMAGE0)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	GetDlgItem(IDC_TH_IMAGE0)->GetClientRect(&rect);
	m_window0 = Size(rect.right - rect.left - 2, rect.bottom - rect.top - 2);//获取图像窗口大小
	m_image0 = imread("res//threshold.PNG", 1);
	DisplayZoom(m_image0, m_window0, "image0");

	namedWindow("image1");
	hWnd = (HWND)cvGetWindowHandle("image1");// MFC嵌套opencv窗口
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_TH_IMAGE1)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	GetDlgItem(IDC_TH_IMAGE1)->GetClientRect(&rect);
	m_window1 = Size(rect.right - rect.left - 2, rect.bottom - rect.top - 2);//获取图像窗口大小
	
	Mat zero = Mat::Mat(m_window1, CV_8UC3, Scalar(255, 255, 255));//定义一个与图片控件一样大的空白图片
	imshow("image1", zero);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Threshold::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}

void Threshold::DisplayZoom(Mat image, Size window, String window_name)
{
	if (image.data) {
		double zoom = MIN((double)window.width / image.cols, (double)window.height / image.rows);//适屏的图片缩放比例
				//缩放图片
		Mat image_zoom;
		if (zoom < 1) {
			resize(image, image_zoom, Size(), zoom, zoom, INTER_AREA); //缩小图片
		}
		else if (zoom == 1) {
			image_zoom = image;
		}
		else {
			resize(image, image_zoom, Size(), zoom, zoom, INTER_LINEAR);//放大图片
		}
		Mat zero = Mat::Mat(window, CV_8UC3, Scalar(255, 255, 255));//定义一个与图片控件一样大的黑色空白图片
		if (window_name == "image1") {
			cvtColor(zero, zero, COLOR_BGR2GRAY);
		}
		Mat roi(zero, Rect(0, 0, image_zoom.cols, image_zoom.rows));
		image_zoom.copyTo(roi);
		imshow(window_name, zero);
	}
}


void Threshold::OnBnClickedPath0()
{
	// TODO: 在此添加控件通知处理程序代码
	//打开文件对话框
	CFileDialog file_dialog(true, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("图片文件(*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png|所有文件(*.*) |*.*||"), NULL);
	//打开文件对话框的标题名
	file_dialog.m_ofn.lpstrTitle = _T("打开源图片");
	if (file_dialog.DoModal() == IDOK)
	{
		CString name = file_dialog.GetPathName();  // 获取图片路径
		string path = CT2A(name);// CString转string
		GetDlgItem(IDC_TH_PATH1)->SetWindowText(name);
		m_image1 = imread(path); //读取图片
		DisplayZoom(m_image1, m_window0, "image0");
		DisplayZoom(m_image1, m_window1, "image1");
		m_threshold1.SetPos(150);//当前停留的位置
		OnChangeThreshold0();
	}
}

void Threshold::OnChangeThreshold0()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString cs;
	m_threshold0.GetWindowTextW(cs);
	int th = _ttoi(cs);
	if (th >= 0 && th <= 255) {
		if (m_image1.data)
		{
			Mat image_gray, image_result;
			cvtColor(m_image1, image_gray, COLOR_BGR2GRAY);
			threshold(image_gray, image_result, th, 255, m_type);
			DisplayZoom(image_result, m_window1, "image1");
		}
	}
	else if (th > 255) {
		th = 255;
		cs.Format(_T("%d"), th);
		m_threshold0.SetWindowTextW(cs);
	}
	else {
		th = 0;
		cs.Format(_T("%d"), th);
		m_threshold0.SetWindowTextW(cs);
	}
}


void Threshold::OnCustomdrawThresold1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString cs;
	cs.Format(_T("%d"), m_threshold1.GetPos());
	m_threshold0.SetWindowTextW(cs);
	*pResult = 0;
}


void Threshold::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_BINARY;
	OnChangeThreshold0();
}


void Threshold::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_BINARY_INV;
	OnChangeThreshold0();
}


void Threshold::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_TRUNC;
	OnChangeThreshold0();
}


void Threshold::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_TOZERO;
	OnChangeThreshold0();
}


void Threshold::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_TOZERO_INV;
	OnChangeThreshold0();
}


void Threshold::OnBnClickedRadio6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_MASK;
	OnChangeThreshold0();
}


void Threshold::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_image1.data) {
		m_type = THRESH_OTSU;
		OnChangeThreshold0();
	}
}


void Threshold::OnBnClickedRadio8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_image1.data) {
		m_type = THRESH_TRIANGLE;
		OnChangeThreshold0();
	}
}
