// FindContours.cpp: 实现文件
//

#include "stdafx.h"
#include "opencv.h"
#include "FindContours.h"
#include "afxdialogex.h"

FindContours g_FindContours = new FindContours;
// FindContours 对话框

IMPLEMENT_DYNAMIC(FindContours, CDialogEx)

FindContours::FindContours(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_FINDCONTOURS, pParent)
{

}

FindContours::~FindContours()
{
}

void FindContours::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FC_TH2, m_th2);
	DDX_Control(pDX, IDC_FC_MODE0, m_mode0);
	DDX_Control(pDX, IDC_FC_METHOD0, m_method0);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_FC_TH1, m_edit_th1);
}


BEGIN_MESSAGE_MAP(FindContours, CDialogEx)
	ON_BN_CLICKED(IDC_FC_TH0, &FindContours::OnBnClickedFcTh0)
	ON_EN_CHANGE(IDC_FC_TH1, &FindContours::OnEnChangeFcTh1)
	ON_CBN_SELCHANGE(IDC_FC_TH2, &FindContours::OnCbnSelchangeFcTh2)
	ON_BN_CLICKED(IDC_FC_MODE0, &FindContours::OnBnClickedFcMode0)
	ON_BN_CLICKED(IDC_FC_MODE1, &FindContours::OnBnClickedFcMode1)
	ON_BN_CLICKED(IDC_FC_MODE2, &FindContours::OnBnClickedFcMode2)
	ON_BN_CLICKED(IDC_FC_MODE3, &FindContours::OnBnClickedFcMode3)
	ON_BN_CLICKED(IDC_FC_MODE4, &FindContours::OnBnClickedFcMode4)
	ON_BN_CLICKED(IDC_FC_METHOD0, &FindContours::OnBnClickedFcMethod0)
	ON_BN_CLICKED(IDC_FC_METHOD1, &FindContours::OnBnClickedFcMethod1)
	ON_BN_CLICKED(IDC_FC_METHOD2, &FindContours::OnBnClickedFcMethod2)
	ON_BN_CLICKED(IDC_FC_METHOD3, &FindContours::OnBnClickedFcMethod3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &FindContours::OnNMCustomdrawSlider1)
END_MESSAGE_MAP()


// FindContours 消息处理程序
BOOL FindContours::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_FC_TH1)->SetWindowTextW(L"150");
	m_threshold = 150;
	m_slider1.SetRange(0, 255);//设置范围
	m_slider1.SetTicFreq(1);//设置显示刻度的间隔
	m_slider1.SetPos(150);//当前停留的位置

	int i;
	LPCTSTR lpszString[8] = { L"THRESH_BINARY",    L"THRESH_BINARY_INV" ,L"THRESH_TRUNC" , L"THRESH_TOZERO" ,
							  L"THRESH_TOZERO_INV",L"THRESH_MASK",       L"THRESH_OTSU" ,  L"THRESH_TRIANGLE" };
	for (i = 0; i < 8; i++) {
		//m_th2.AddString(lpszString[i]);
		m_th2.InsertString(i, lpszString[i]);
	}
	m_th2.SetCurSel(0);
	m_type = THRESH_BINARY;

	m_mode0.SetCheck(1);
	m_mode = RETR_EXTERNAL;

	m_method0.SetCheck(1);
	m_method = CHAIN_APPROX_NONE;

	HWND hWnd, hParent;
	CRect rect;
	namedWindow("fc_image0");
	hWnd = (HWND)cvGetWindowHandle("fc_image0");// MFC嵌套opencv窗口
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_FC_IMAGE0)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	GetDlgItem(IDC_FC_IMAGE0)->GetClientRect(&rect);
	m_window0 = Size(rect.right - rect.left - 2, rect.bottom - rect.top - 2);//获取图像窗口大小
	m_image0 = imread("res//findContours.PNG", 1);
	DisplayZoom(m_image0, m_window0, "fc_image0");

	namedWindow("fc_image1");
	hWnd = (HWND)cvGetWindowHandle("fc_image1");// MFC嵌套opencv窗口
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_FC_IMAGE1)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	GetDlgItem(IDC_FC_IMAGE1)->GetClientRect(&rect);
	m_window1 = Size(rect.right - rect.left - 2, rect.bottom - rect.top - 2);//获取图像窗口大小
	Mat zero = Mat::Mat(m_window1, CV_8UC3, Scalar(255, 255, 255));//定义一个与图片控件一样大的空白图片
	imshow("fc_image1", zero);

	namedWindow("fc_image2");
	hWnd = (HWND)cvGetWindowHandle("fc_image2");// MFC嵌套opencv窗口
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_FC_IMAGE2)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	GetDlgItem(IDC_FC_IMAGE2)->GetClientRect(&rect);
	m_window1 = Size(rect.right - rect.left - 2, rect.bottom - rect.top - 2);//获取图像窗口大小
	zero = Mat::Mat(m_window1, CV_8UC3, Scalar(255, 255, 255));//定义一个与图片控件一样大的空白图片
	imshow("fc_image2", zero);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void FindContours::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void FindContours::OnBnClickedFcTh0()
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
		m_image2 = imread(path); //读取图片
		DisplayZoom(m_image2, m_window0, "fc_image0");
		OnEnChangeFcTh1();
	}
}

void FindContours::DisplayZoom(Mat image, Size window, String window_name)
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
		if (window_name == "fc_image1") {
			cvtColor(zero, zero, COLOR_BGR2GRAY);
		}
		Mat roi(zero, Rect(0, 0, image_zoom.cols, image_zoom.rows));
		image_zoom.copyTo(roi);
		imshow(window_name, zero);
	}
}


void FindContours::OnEnChangeFcTh1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString cs;
	//m_threshold0.GetWindowTextW(cs);
	GetDlgItem(IDC_FC_TH1)->GetWindowTextW(cs);
	int th = _ttoi(cs);
	if (th >= 0 && th <= 255) {
		if (m_image2.data)
		{
			Mat image_gray, iamge_th, image_result;
			cvtColor(m_image2, image_gray, COLOR_BGR2GRAY);
			threshold(image_gray, iamge_th, th, 255, m_type);
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			m_image2.copyTo(image_result);
			if (m_mode == RETR_FLOODFILL) {
				Mat image_32S;
				iamge_th.convertTo(image_32S, CV_32SC1);
				findContours(image_32S, contours, hierarchy, m_mode, m_method);
			}
			else {
				findContours(iamge_th, contours, hierarchy, m_mode, m_method);
			}
			
			for (size_t i = 0; i < contours.size(); i++)
			{
				drawContours(image_result, contours, i, Scalar(0,255,0), 2, LINE_8, hierarchy, 0);
			}
			DisplayZoom(iamge_th, m_window1, "fc_image1");
			DisplayZoom(image_result, m_window1, "fc_image2");
		}
	}
	else if (th > 255) {
		th = 255;
		cs.Format(_T("%d"), th);
		GetDlgItem(IDC_FC_TH1)->SetWindowTextW(cs);
	}
	else {
		th = 0;
		cs.Format(_T("%d"), th);
		GetDlgItem(IDC_FC_TH1)->SetWindowTextW(cs);
	}
}

void FindContours::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString cs;
	cs.Format(_T("%d"), m_slider1.GetPos());
	m_edit_th1.SetWindowTextW(cs);
	*pResult = 0;
}

void FindContours::OnCbnSelchangeFcTh2()
{
	// TODO: 在此添加控件通知处理程序代码
	int type[8] = { THRESH_BINARY,     THRESH_BINARY_INV , THRESH_TRUNC,  THRESH_TOZERO,
				    THRESH_TOZERO_INV, THRESH_MASK,        THRESH_OTSU ,  THRESH_TRIANGLE };
	m_type = type[m_th2.GetCurSel()];
	OnEnChangeFcTh1();
}


void FindContours::OnBnClickedFcMode0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_mode = RETR_EXTERNAL;
	OnEnChangeFcTh1();
}


void FindContours::OnBnClickedFcMode1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_mode = RETR_LIST;
	OnEnChangeFcTh1();
}


void FindContours::OnBnClickedFcMode2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_mode = RETR_CCOMP;
	OnEnChangeFcTh1();
}


void FindContours::OnBnClickedFcMode3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_mode = RETR_TREE;
	OnEnChangeFcTh1();
}


void FindContours::OnBnClickedFcMode4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_mode = RETR_FLOODFILL;
	OnEnChangeFcTh1();
}


void FindContours::OnBnClickedFcMethod0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_method = CHAIN_APPROX_NONE;
	OnEnChangeFcTh1();
}


void FindContours::OnBnClickedFcMethod1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_method = CHAIN_APPROX_SIMPLE;
	OnEnChangeFcTh1();
}


void FindContours::OnBnClickedFcMethod2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_method = CHAIN_APPROX_TC89_L1;
	OnEnChangeFcTh1();
}


void FindContours::OnBnClickedFcMethod3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_method = CHAIN_APPROX_TC89_KCOS;
	OnEnChangeFcTh1();
}
