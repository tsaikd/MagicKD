// ChildView.h : CChildView ���O������
//


#pragma once


// CChildView ����

class CChildView : public CWnd
{
// �غc
public:
	CChildView();

// �ݩ�
public:

// �@�~
public:

// �мg
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �{���X��@
public:
	virtual ~CChildView();

	// ���ͪ��T�������禡
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

