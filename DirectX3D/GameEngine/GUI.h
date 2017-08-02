#ifndef GUI_H
#define GUI_H

#include "defines.h"

// GUI�ؼ��ṹ
struct stGUIControl
{
	int m_type;    // �ؼ�����
	int m_id;      // �ؼ�ID
	unsigned long m_color;   // �ؼ���ɫ

	int m_listID;            // �����������������ID�򶥵㻺��ID

	int m_xPos, m_yPos;      // �ؼ���XY����
	int m_width, m_height;   // �ؼ��Ŀ�͸�
#ifdef UNICODE
	TCHAR *m_text;
#else
	char *m_text;            // ָ��̬�ؼ��ı��ַ���ָ��
#endif
	int m_upTex, m_downTex, m_overTex;  // ��ť�ĵ��𡢰��º�ͣ��
};



// GUI�ؼ�ϵͳ��
class CGUISystem
{
public:
	// ���캯��
	CGUISystem() : m_controls(0), m_totalControls(0), m_backDropID(-1)
	{
	}
	// ��������
	~CGUISystem()
	{
		// Shutdown();
	}

	int IncreaseControls();

	// ��ӱ���ͼ�ؼ�
	bool AddBackdrop(int texID, int staticID);
#ifdef UNICODE
	// ��Ӿ�̬�ı��ؼ�
	bool AddStaticText(int id, TCHAR *text, int x, int y, unsigned long color, int fontID);
#else
	bool AddStaticText(int id, char *text, int x, int y, unsigned long color, int fontID);
#endif

	// ��Ӱ�ť�ؼ�
	bool AddButton(int id, int x, int y, int width,int height,
		int upID, int overID,int downID,unsigned int staticID);

	// �ڴ�����
	void Shutdown();

	// ��ȡָ��ID��GUI�ؼ�����
	stGUIControl *GetGUIControl(int id)
	{
		if (id < 0 || id >= m_totalControls)
		{
			return NULL;
		}
		return &m_controls[id];
	}

	// ���ؿؼ��������� m_totalControls
	int GetTotalControls()
	{
		return m_totalControls;
	}

	// ����ָ������ͼ�ؼ�����
	stGUIControl *GetBackDrop()
	{
		if (m_backDropID >= 0 && m_totalControls)
		{
			return &m_controls[m_backDropID];
		}

		// ʧ���򷵻�0ֵ
		return NULL;
	}


private:
	stGUIControl *m_controls;    // GUI�ؼ�����ָ��
	int m_totalControls;         // �ؼ����������
	int m_backDropID;            // ����ͼ�ؼ���ID
};

#endif
