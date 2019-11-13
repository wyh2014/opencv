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
	DDX_Control(pDX, IDC_EDIT1, m_path);
	DDX_Control(pDX, IDC_EDIT2, m_threshold);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_RADIO1, m_button);
}


BEGIN_MESSAGE_MAP(Threshold, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Threshold::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &Threshold::OnChangeEdit2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &Threshold::OnCustomdrawSlider1)
	ON_BN_CLICKED(IDC_RADIO3, &Threshold::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO1, &Threshold::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &Threshold::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, &Threshold::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &Threshold::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &Threshold::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &Threshold::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &Threshold::OnBnClickedRadio8)
END_MESSAGE_MAP()


// Threshold 消息处理程序


BOOL Threshold::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	namedWindow("image");
	HWND hWnd = (HWND)cvGetWindowHandle("image");// MFC嵌套opencv窗口
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_IMAGE)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	CRect rect;
	GetDlgItem(IDC_IMAGE)->GetClientRect(&rect);
	m_window = Size(rect.right - rect.left - 2, rect.bottom - rect.top - 2);//获取图像窗口大小

	m_threshold.SetWindowTextW(L"0");
	Mat zero = Mat::Mat(m_window, CV_8UC3, Scalar(255, 255, 255));//定义一个与图片控件一样大的空白图片
	imshow("image", zero);

	m_slider.SetRange(0, 255);//设置范围
	m_slider.SetTicFreq(1);//设置显示刻度的间隔
	m_slider.SetPos(0);//当前停留的位置

	m_type = THRESH_BINARY;
	m_button.SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Threshold::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}

void Threshold::DisplayZoom(Mat image, Size window)
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
		cvtColor(zero, zero, COLOR_BGR2GRAY);
		Mat roi(zero, Rect(0, 0, image_zoom.cols, image_zoom.rows));
		image_zoom.copyTo(roi);
		imshow("image", zero);
	}
}


void Threshold::OnBnClickedButton1()
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
		m_path.SetWindowTextW(name);
		m_image = imread(path); //读取图片
		DisplayZoom(m_image, m_window);
	}
}

void Threshold::OnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (m_image.data)
	{
		CString cs;
		m_threshold.GetWindowTextW(cs);
		int th = _ttoi(cs);
		Mat image_gray, image_result;
		cvtColor(m_image, image_gray, COLOR_BGR2GRAY);
		threshold(image_gray, image_result, th, 255, m_type);
		DisplayZoom(image_result, m_window);
	}
	
}


void Threshold::OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString cs;
	cs.Format(_T("%d"), m_slider.GetPos());
	GetDlgItem(IDC_EDIT2)->SetWindowText(cs);
	*pResult = 0;
}


void Threshold::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_BINARY;
	OnChangeEdit2();
}


void Threshold::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_BINARY_INV;
	OnChangeEdit2();
}


void Threshold::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_TRUNC;
	OnChangeEdit2();
}


void Threshold::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_TOZERO;
	OnChangeEdit2();
}


void Threshold::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_TOZERO_INV;
	OnChangeEdit2();
}


void Threshold::OnBnClickedRadio6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_type = THRESH_MASK;
	OnChangeEdit2();
}


void Threshold::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_image.data) {
		m_slider.SetPos(0);//当前停留的位置
		m_type = THRESH_OTSU;
		CString cs;
		m_threshold.GetWindowTextW(cs);
		int th = _ttoi(cs);
		Mat image_gray, image_result;
		cvtColor(m_image, image_gray, COLOR_BGR2GRAY);
		threshold(image_gray, image_result, 0, 255, m_type);
		DisplayZoom(image_result, m_window);
	}
}


void Threshold::OnBnClickedRadio8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_image.data) {
		m_slider.SetPos(0);//当前停留的位置
		m_type = THRESH_TRIANGLE;
		CString cs;
		m_threshold.GetWindowTextW(cs);
		int th = _ttoi(cs);
		Mat image_gray, image_result;
		cvtColor(m_image, image_gray, COLOR_BGR2GRAY);
		threshold(image_gray, image_result, 0, 255, m_type);
		DisplayZoom(image_result, m_window);
	}
}
