
// PsycologyTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PsycologyTest.h"
#include "PsycologyTestDlg.h"
#include "afxdialogex.h"
#include "../PsiCommon/PsiScale.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

static int buttons[] = { 
	IDC_BUTTON_1, 
	IDC_BUTTON_2, 
	IDC_BUTTON_3, 
	IDC_BUTTON_4, 
	IDC_BUTTON_5, 
	IDC_BUTTON_6, 
	IDC_BUTTON_7, 
};

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPsycologyTestDlg dialog


CPsycologyTestDlg::CPsycologyTestDlg(shared_ptr<CPsiScale> scale, 
	CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PSYCOLOGYTEST_DIALOG, pParent),
	_psi_scale(scale),
	_current_question_index(0)
	, _question_number(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPsycologyTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_QUESTION, _question);
	DDX_Text(pDX, IDC_STATIC_QUESTION_NUMBER, _question_number);
}

BEGIN_MESSAGE_MAP(CPsycologyTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_PREV, &CPsycologyTestDlg::OnBnClickedPrev)
	ON_BN_CLICKED(ID_NEXT, &CPsycologyTestDlg::OnBnClickedNext)
	ON_BN_CLICKED(IDC_BUTTON_1, &CPsycologyTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CPsycologyTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CPsycologyTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CPsycologyTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CPsycologyTestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CPsycologyTestDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CPsycologyTestDlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// CPsycologyTestDlg message handlers


BOOL CPsycologyTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	for (unsigned int i = 0; i < sizeof (buttons) / sizeof(int); ++i)
	{
		GetDlgItem(buttons[i])->ShowWindow(SW_HIDE);
	}

	ShowQuestion(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPsycologyTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPsycologyTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPsycologyTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CPsycologyTestDlg::ShowQuestion(unsigned question_index)
{
	ASSERT(_psi_scale);

	if (question_index >= _psi_scale->GetQuestionCount())
		return false;

	_current_question_index = question_index;
	_question = _psi_scale->Question(_current_question_index).GetText();

	if (_psi_scale->IsSameChoice())
	{
		if (_current_question_index == 0)
		{
			auto& choices = _psi_scale->Choices();
			for (unsigned int i = 0; i < choices.size(); ++i)
			{
				CString button_text;
				button_text.Format(_T("    %c. %s"), _T('A') + i, choices[i].text);
				GetDlgItem(buttons[i])->SetWindowText(button_text);
			}
		}
	}
	else
	{
		auto& choices = _psi_scale->Question(_current_question_index).Choices();
		for (unsigned int i = 0; i < choices.size(); ++i)
		{
			CString button_text;
			button_text.Format(_T("    %c. %s"), _T('A') + i, choices[i].text);
			GetDlgItem(buttons[i])->SetWindowText(button_text);
		}
	}

	ShowButtons(_psi_scale->Choices().size());

	if (_answer_manager.IsAnswered(_psi_scale->GetId(), _current_question_index))
	{
		Check(_answer_manager.GetAnswer(_psi_scale->GetId(), 
			_current_question_index) - 1);
	}
	else
	{
		UncheckAll();
	}
	_question_number.Format(_T("%d / %d"), _current_question_index + 1, 
		_psi_scale->GetQuestionCount());

	UpdateData(FALSE);

	return true;
}

bool CPsycologyTestDlg::ShowButtons(unsigned choice_count)
{
	if (choice_count > 10)
		return false;

	for (unsigned int i = 0; i < choice_count; ++i)
	{
		GetDlgItem(buttons[i])->ShowWindow(SW_SHOW);
	}
	for (unsigned int i = choice_count; i < sizeof(buttons) / sizeof(int); ++i)
	{
		GetDlgItem(buttons[i])->ShowWindow(SW_HIDE);
	}

	AdjustSize(buttons[choice_count - 1]);

	return true;
}

void CPsycologyTestDlg::Check(int button_to_check)
{
	for (unsigned int i = 0; i < sizeof(buttons) / sizeof (int); ++i)
	{
// 		((CButton*)GetDlgItem(buttons[i]))->SetCheck(
//			(i != button_to_check) ? BST_UNCHECKED : BST_CHECKED);
		((CButton*)GetDlgItem(buttons[i]))->SetState(
			(i == button_to_check));
	}
}

void CPsycologyTestDlg::UncheckAll()
{
	for (auto button : buttons)
	{
//		((CButton*)GetDlgItem(button))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(button))->SetState(0);
	}
}

void CPsycologyTestDlg::OnBnClickedPrev()
{
	if (_current_question_index > 0)
	{
		ShowQuestion(_current_question_index - 1);
	}
}

void CPsycologyTestDlg::OnBnClickedNext()
{
	ASSERT(_psi_scale);
	if (_current_question_index < _psi_scale->GetQuestionCount() - 1)
	{
		ShowQuestion(_current_question_index + 1);
	}
}

void CPsycologyTestDlg::ProcessAnswer(unsigned int answer)
{
	// 1. 记录
	_answer_manager.AddAnswer(_psi_scale->GetId(), _current_question_index, answer);

	// 2. 下一道题。
	if (_current_question_index < _psi_scale->GetQuestionCount() - 1)
	{
		ShowQuestion(_current_question_index + 1);
	}
	else
	{
		int unanswer_question = _answer_manager.CheckForUnansweredQuestion(*_psi_scale);
		if (unanswer_question == -1)
		{
			if (AfxMessageBox(_T("您已经完成了该问卷，点击“确认”按钮返回。"), MB_OKCANCEL) ==
				IDOK)
			{
				__super::OnOK();
			}
		}
		else
		{
			if (AfxMessageBox(_T("还有尚未回答的问题，点击“确认”跳转到问题。"),
				MB_OKCANCEL) == IDOK)
			{
				ShowQuestion(unanswer_question);
			}
			else
			{
				__super::OnOK();
			}
		}
	}
}

void CPsycologyTestDlg::OnBnClickedButton1()
{
	ProcessAnswer(1);
}


void CPsycologyTestDlg::OnBnClickedButton2()
{
	ProcessAnswer(2);
}

void CPsycologyTestDlg::OnBnClickedButton3()
{
	ProcessAnswer(3);
}

void CPsycologyTestDlg::OnBnClickedButton4()
{
	ProcessAnswer(4);
}

void CPsycologyTestDlg::OnBnClickedButton5()
{
	ProcessAnswer(5);
}

void CPsycologyTestDlg::OnBnClickedButton6()
{
	ProcessAnswer(6);
}

void CPsycologyTestDlg::OnBnClickedButton7()
{
	ProcessAnswer(7);
}

void CPsycologyTestDlg::AdjustSize(int last_button)
{
	CRect button_rect;
	auto button = GetDlgItem(last_button);
	if (button == nullptr)
		return;

	button->GetWindowRect(&button_rect);
	ScreenToClient(&button_rect);

	CRect markrect;
	CRect dlgrect;
	CRect clientrect;

	MoveButtonUp(*GetDlgItem(ID_PREV), button_rect.bottom + 15);
	MoveButtonUp(*GetDlgItem(ID_NEXT), button_rect.bottom + 15);

	GetClientRect(&clientrect);  // client area of the dialog
	GetWindowRect(&dlgrect);	  // rectangle of the dialog window

								  // get height of the title bar
								  //int offset = dlgrect.Width() - clientrect.right ;

	CRect next_button_rect;
	auto next_button = GetDlgItem(ID_NEXT);
	ASSERT(next_button);

	next_button->GetWindowRect(&next_button_rect);
	ScreenToClient(&next_button_rect);

	int offset = dlgrect.Height() - clientrect.bottom;
	dlgrect.bottom = dlgrect.top + offset + next_button_rect.bottom + 10;

	MoveWindow(dlgrect.left, dlgrect.top, dlgrect.Width(), dlgrect.Height());
}

void CPsycologyTestDlg::MoveButtonUp(CWnd& button,
	unsigned int y_pos)
{
	CRect rect;
	button.GetWindowRect(rect);
	ScreenToClient(rect);

	rect.bottom = y_pos + rect.Height();
	rect.top = y_pos;
	button.MoveWindow(rect);
}
