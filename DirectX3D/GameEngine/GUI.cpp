#include "stdafx.h"
#include "GUI.h"

// ��m_controls��������β������һ��GUI�ؼ�����
int CGUISystem::IncreaseControls()
{
	// �״δ����ؼ�����ʱ������һ������1��Ԫ�صĿؼ�����
	if (!m_controls)
	{
		// m_fonts��ʼ��Ϊ��̬�����Ŀؼ���������
		m_controls = new stGUIControl[1];
		if (!m_controls)
		{
			return GM_FAIL;
		}

		// Ϊ��������ڴ�����ʼ������
		// ��m_controls��Ԫ�ص��ڴ���������0
		memset(&m_controls[0], 0, sizeof(stGUIControl));
	}
	else
	{
		// ��ʱ�Ŀؼ�����ָ�룬���ڱ��浱ǰ�ؼ������ֵ
		stGUIControl *temp;

		// ָ��̬���ٵĿؼ��������飬�ÿؼ���������Ԫ�رȵ�ǰ��1λ
		temp = new stGUIControl[m_totalControls + 1];
		if (!temp)
		{
			return GM_FAIL;
		}

		// ��temp��ָ����ڴ���������0
		memset(temp, 0, sizeof(stGUIControl) * (m_totalControls + 1));

		// ��m_controls�ؼ����������ȫ�����ݸ��Ƶ�temp��
		memcpy(temp, m_controls, sizeof(stGUIControl) * m_totalControls);

		delete[] m_controls;   // ����m_controls��������

		m_controls = temp;     // m_controls���������õ�ǰ��temp���������ȫ������
	}

	return GM_OK;
}


// ��ӱ���ͼ�ؼ�
bool CGUISystem::AddBackdrop(int texID, int sID)
{
	if (texID < 0 || sID < 0)
	{
		return false;
	}

	if (m_backDropID < 0)
	{
		// ��m_controls��������β������һ��GUI�ؼ�����
		if (! IncreaseControls() )
		{
			return false;
		}

		// ���ÿؼ�����ĳ�Ա����
		m_controls[m_totalControls].m_type = UGP_GUI_BACKDROP;  // ����ͼ�ؼ�����
		m_controls[m_totalControls].m_upTex = texID;            // ����ID
		m_controls[m_totalControls].m_listID = sID;             // ��̬����ID


		m_backDropID = m_totalControls;   // �õ�ǰ�ؼ�������������ǰ�ؼ�ID��ֵ

		m_totalControls++;   // �ؼ�������������1
	}
	else
	{
		m_controls[m_backDropID].m_upTex = texID;   // ��ȡ����ID
		m_controls[m_backDropID].m_listID = sID;    // ��ȡ��̬����ID
	}

	return true;
}

// ��Ӿ�̬�ı��ؼ�
#ifdef UNICODE
bool CGUISystem::AddStaticText(
							   int id,                 // �ؼ�ID
							   TCHAR *text,             // Ҫ��ʾ���ı�
							   int x, int y,           // �ı���ʼX��Y����
							   unsigned long color,    // �ı���ɫ
							   int fontID              // ��������ID
							   )
#else
bool CGUISystem::AddStaticText(
							   int id,                 // �ؼ�ID
							   char *text,             // Ҫ��ʾ���ı�
							   int x, int y,           // �ı���ʼX��Y����
							   unsigned long color,    // �ı���ɫ
							   int fontID              // ��������ID
							   )
#endif


{
	if (!text || fontID < 0)
	{
		return false;
	}

	// ��m_controls��������β������һ��GUI�ؼ�����
	if (! IncreaseControls() )
	{
		return false;
	}

	// ��ȡ�ı��ַ�����
	int len = 0;//strlen(text);
	StringCchLength(text, STRSAFE_MAX_CCH, (size_t*)&len);
	// ���ÿؼ�����ĳ�Ա����
	if (!len)
	{
		m_controls[m_totalControls].m_type = UGP_GUI_FPS;
	}
	else
	{
		m_controls[m_totalControls].m_type = UGP_GUI_STATICTEXT;   // ��̬�ı��ؼ�����
	}
	m_controls[m_totalControls].m_id = id;            // �ؼ�ID
	m_controls[m_totalControls].m_color = color;      // �ı���ɫ
	m_controls[m_totalControls].m_xPos = x;           // �ؼ�X����
	m_controls[m_totalControls].m_yPos = y;           // �ؼ�Y����
	m_controls[m_totalControls].m_listID = fontID;    // ��������ID


	// ���ٴ���ı��ַ����ڴ��
#ifdef UNICODE
	m_controls[m_totalControls].m_text = new TCHAR[len + 1];
	if (!m_controls[m_totalControls].m_text) return false;
	memset(m_controls[m_totalControls].m_text, 0, sizeof(TCHAR)*(len+1));
	// ���ı�text���Ƶ��ÿؼ���Ӧ�ĳ�Ա������
	memcpy(m_controls[m_totalControls].m_text, text, len*sizeof(TCHAR)); 
#else 
	m_controls[m_totalControls].m_text = new char[len + 1];
	if (!m_controls[m_totalControls].m_text) return false;
	memset(m_controls[m_totalControls].m_text, 0, sizeof(char)*len);
	memcpy(m_controls[m_totalControls].m_text, text, len*sizeof(char)); 
#endif
	// �ؼ�������������
	m_totalControls++;

	return true;
}

// ��Ӱ�ť�ؼ�
bool CGUISystem::AddButton(int id,                 // �ؼ�ID
						   int x, int y,           // �ؼ�����ʼX��Y����
						   int width, int height,  // �ؼ��Ŀ�Ⱥ͸߶�
						   int upID, int overID, int downID,  // �ؼ���3��״̬������ID
						   unsigned int staticID   // ��ʾ��ť������״�ľ�̬����ID
						   )
{
	// ��m_controls��������β������һ��GUI�ؼ�����
	if (! IncreaseControls() )
	{
		return false;
	}

	// ���ÿؼ�����ĳ�Ա����
	m_controls[m_totalControls].m_type = UGP_GUI_BUTTON;  // ��ť�ؼ�����
	m_controls[m_totalControls].m_id = id;                // �ؼ�ID
	m_controls[m_totalControls].m_xPos = x;               // �ؼ���X����
	m_controls[m_totalControls].m_yPos = y;               // �ؼ���Y����
	m_controls[m_totalControls].m_width = width;          // �ؼ����
	m_controls[m_totalControls].m_height = height;        // �ؼ��߶�
	m_controls[m_totalControls].m_upTex = upID;           // ��ť����״̬������ID
	m_controls[m_totalControls].m_overTex = overID;       // ���ͣ���ڰ�ť�ϵ�����ID
	m_controls[m_totalControls].m_downTex = downID;       // ��ť�����µ�����ID
	m_controls[m_totalControls].m_listID = staticID;      // ��ʾ��ť������״�ľ�̬����ID

	// �ؼ�������������1
	m_totalControls++;

	return true;
}


// �ڴ�����
void CGUISystem::Shutdown()
{
	// �ͷſؼ����������ȫ���ڴ��
	for (int s=0; s < m_totalControls; s++)
	{
		if (m_controls[s].m_text)
		{
			delete[] m_controls[s].m_text;   // ɾ���ؼ�������ı��ַ��ڴ�
			m_controls[s].m_text = NULL;
		}
	}

	// �ؼ�����������0
	m_totalControls = 0;

	if (m_controls)
	{
		delete[] m_controls;
	}

	// ��ָ��ָ��Ϊ��
	m_controls = NULL;
}
