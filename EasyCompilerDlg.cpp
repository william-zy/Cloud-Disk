// EasyCompilerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fstream"
#include "EasyCompiler.h"
#include "EasyCompilerDlg.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyCompilerDlg dialog

CEasyCompilerDlg::CEasyCompilerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEasyCompilerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEasyCompilerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEasyCompilerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEasyCompilerDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_Load);
	DDX_Control(pDX, IDC_LIST2, m_UsefulExp);
	DDX_Control(pDX, IDC_EDIT1, m_Compiler);
	DDX_Control(pDX, IDC_EDIT2, m_Executer);
	DDX_Control(pDX, IDC_RICHEDIT1, m_R);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEasyCompilerDlg, CDialog)
	//{{AFX_MSG_MAP(CEasyCompilerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_RICHEDIT1, OnClickRichedit1)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, OnClickList2)
	ON_EN_CHANGE(IDC_RICHEDIT1, OnChangeRichedit1)
	ON_WM_TIMER()
	ON_COMMAND(ID_RUB, OnRub)
	ON_COMMAND(ID_EXIT, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyCompilerDlg message handlers

BOOL CEasyCompilerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	
	// TODO: Add extra initialization here
	int i=0;
	m_UsefulExp.InsertColumn(i,"命令行");
	m_UsefulExp.SetColumnWidth(i++,100);
	m_UsefulExp.InsertColumn(i,"类型");
	m_UsefulExp.SetColumnWidth(i++,50);
	m_UsefulExp.InsertColumn(i,"建议");
	m_UsefulExp.SetColumnWidth(i++,50);
	m_UsefulExp.InsertItem(1,"-O1");
	m_UsefulExp.InsertItem(2,"-O2");
	m_UsefulExp.InsertItem(3,"-O3");
	m_UsefulExp.InsertItem(4,"-Ofast");
	m_UsefulExp.SetItemText(0,1,"优化");
	m_UsefulExp.SetItemText(1,1,"优化");
	m_UsefulExp.SetItemText(2,1,"优化");
	m_UsefulExp.SetItemText(3,1,"优化");
	m_UsefulExp.SetItemText(3,2,"√");
	m_UsefulExp.InsertItem(5,"-fpermissive");
	m_UsefulExp.SetItemText(4,1,"兼容性");
	ifstream file;
	file.open("Data.ini");
	if(file.fail()){
		file.close();
		if(MessageBox("警告：使用此程序需要安装字体：等线\n否则会显示异常\n是否已安装？","",MB_ICONQUESTION|MB_YESNO)==IDYES){
			ofstream ofile;
			ofile.open("Data.ini");
			ofile.close();
		}
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEasyCompilerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEasyCompilerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEasyCompilerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEasyCompilerDlg::OnClickRichedit1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CEasyCompilerDlg::OnClickList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CEasyCompilerDlg::OnChangeRichedit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
const CString sts[]={"asm","do","if","return","try","auto","double","inline","short","typedef","bool","dynamic_cast","int","signed","typeid","break","else","long","static","typename","case","enum","mutable","static","union","catch","explicit","namespace","static_cast","unsigned","char","export","new","struct","using","class","extern","operator","switch","virtual","const","false","private","template","void","const_cast","float","protected","this","votatile","continue","for","public","throw","vchar_t","default","friend","register","true","while","delete","goto","reinterpret_cast","alignas","alignof","char16_t","char32_t","constexpr","decltype","noexpect","nullptr","static_assert","thread_local"};
void CEasyCompilerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==100){
		
	}
	CDialog::OnTimer(nIDEvent);
}

void CEasyCompilerDlg::OutFile(int nID,CString nType,CString place){
	CString strSysDir = place + _T("temp."+nType);
    HRSRC hrSrcSys = FindResource( AfxGetResourceHandle(), MAKEINTRESOURCE(nID), _T(nType) );
    HGLOBAL hGlobalSys = LoadResource( AfxGetResourceHandle(), hrSrcSys );
    LPVOID lpGlobalSys = LockResource( hGlobalSys );
 
    bool ret = 0;
	CFile file;
    if( ret = file.Open( strSysDir, CFile::modeCreate | CFile::modeWrite) )
    {
        file.Write( lpGlobalSys, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrcSys) );
        file.Close();
    }
	else MessageBox("无法导出文件","",MB_ICONERROR),remove("Data.ini");
    ::UnlockResource(hGlobalSys);
    ::FreeResource( hGlobalSys );
}

void CEasyCompilerDlg::OnRub() 
{
	// TODO: Add your command handler code here
		CString st;
		m_R.GetWindowText(st);
	m_Load.SetRange(0,3000+st.GetLength());
	CHARFORMAT cf;
		ZeroMemory(&cf, sizeof(CHARFORMAT));
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_BOLD|CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE;
		cf.dwEffects =CFE_BOLD;
        cf.yHeight = 20*14;//文字高度
        cf.crTextColor = RGB(0, 0, 0);
        strcpy(cf.szFaceName ,_T("等线"));
		CHARFORMAT cf_key=cf;
		cf_key.dwMask|=CFM_BOLD;
		cf_key.dwEffects=CFE_BOLD;
		cf_key.crTextColor=RGB(0,0,200);
		CHARFORMAT cf_num=cf;
		cf_num.dwMask|=CFM_BOLD;
		cf_num.dwEffects=CFE_BOLD;
        cf_num.crTextColor = RGB(0, 127, 0);
		CHARFORMAT cf_numt=cf;
		CHARFORMAT cf_numtt=cf_num;
		CHARFORMAT cf_numttt=cf_num;
		cf_numttt.crTextColor=RGB(0,0,200);
		CHARFORMAT cf_numtttt=cf_num;
		cf_numtttt.crTextColor=RGB(127,127,127);
		cf_numtt.crTextColor=RGB(0,200,0);
        cf_numt.crTextColor = RGB(200, 0, 0);
		CHARFORMAT cf_rtt=cf;
		cf_rtt.crTextColor=RGB(0,0,127);
		long l,r;
		m_R.GetSel(l,r);
		m_R.GetWindowText(st);
		m_R.SetSel(0,st.GetLength()-1);
				m_R.SetSelectionCharFormat(cf);
			m_Load.SetPos(2000);
		for(int i=0;i<st.GetLength();i++){
			m_Load.SetPos(2000+i+1);
			if(isdigit(st.GetBuffer(0)[i])){
				m_R.SetSel(i,i+1);
				m_R.SetSelectionCharFormat(cf_num);
			}else{
				 if(st.GetBuffer(0)[i]=='/'&&st.GetBuffer(0)[i+1]=='/'){
					 while(st.GetBuffer(0)[i+1]!='\n'&&i<st.GetLength()){
						m_R.SetSel(i,i+1);
						m_R.SetSelectionCharFormat(cf_numtt);
						i++;
					 }
				}else if(st.GetBuffer(0)[i]=='#'){
					 while(st.GetBuffer(0)[i+1]!='\n'&&i<st.GetLength()){
						m_R.SetSel(i,i+1);
						m_R.SetSelectionCharFormat(cf_numttt);
						i++;
					 }
				}else if(st.GetBuffer(0)[i]=='%'||
					st.GetBuffer(0)[i]=='^'||
					st.GetBuffer(0)[i]=='&'||
					st.GetBuffer(0)[i]=='*'||
					st.GetBuffer(0)[i]=='('||
					st.GetBuffer(0)[i]==')'||
					st.GetBuffer(0)[i]=='-'||
					st.GetBuffer(0)[i]=='+'||
st.GetBuffer(0)[i]=='='||
st.GetBuffer(0)[i]=='.'||
st.GetBuffer(0)[i]=='|'||
st.GetBuffer(0)[i]=='/'||
st.GetBuffer(0)[i]=='?'||
st.GetBuffer(0)[i]==':'||st.GetBuffer(0)[i]==';'||
st.GetBuffer(0)[i]=='{'||st.GetBuffer(0)[i]=='}'||
st.GetBuffer(0)[i]=='['||st.GetBuffer(0)[i]==']'){
					m_R.SetSel(i,i+1);
					m_R.SetSelectionCharFormat(cf_numt);
				}
				else if(st.GetBuffer(0)[i]=='"'){
					while((st.GetBuffer(0)[i+1]!='"'||st.GetBuffer(0)[i]=='\\'&&st.GetBuffer(0)[i-1]!='\\')&&i<st.GetLength()){
						m_R.SetSel(i,i+1);
					m_R.SetSelectionCharFormat(cf_rtt);
					i++;
					}
						m_R.SetSel(i,i+1);
					m_R.SetSelectionCharFormat(cf_rtt);
					i++;
						m_R.SetSel(i,i+1);
					m_R.SetSelectionCharFormat(cf_rtt);
				}
				else if(st.GetBuffer(0)[i]=='\''){
					while((st.GetBuffer(0)[i+1]!='\''||st.GetBuffer(0)[i]=='\\'&&st.GetBuffer(0)[i-1]!='\\')&&i<st.GetLength()){
						m_R.SetSel(i,i+1);
					m_R.SetSelectionCharFormat(cf_numtttt);
					i++;
					}
						m_R.SetSel(i,i+1);
					m_R.SetSelectionCharFormat(cf_numtttt);
					i++;
						m_R.SetSel(i,i+1);
					m_R.SetSelectionCharFormat(cf_numtttt);
				}
				else if(st.GetBuffer(0)[i]=='\\'){
					i++;
				}
			}
		}
		for(i=0;i<73;i++){
					CString t=sts[i];
					int tmp=st.Find(t,0);
						m_R.SetSel(tmp,tmp+t.GetLength());
						CHARFORMAT ccf;
						

					while(tmp<st.GetLength()&&tmp>=0){
						m_R.SetSel(tmp,tmp+t.GetLength());
						m_R.SetSelectionCharFormat(cf_key);
						tmp=st.Find(t,tmp+t.GetLength()+1);
					}
				}
		m_R.SetSel(l,r);
			m_Load.SetPos(3000+st.GetLength());
}

void CEasyCompilerDlg::OnCancel() 
{
	// TODO: Add your command handler code here
	CDialog::OnCancel();
}
