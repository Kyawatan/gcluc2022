#include "TaskTrikNavi.h"
#define _USE_MATH_DEFINES // math.h�C���N���[�h�O�ɋL�q�K�v
#include <math.h>
#include "ScrapTexQuad.h"


TaskTrikNavi::TaskTrikNavi(GameDirector* pGameDirector, int iTrikNum, KVector3 vPos)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::UI), NULL)
	, m_pSprite(NULL)
	, m_pGameDirector(pGameDirector)
	, m_iTrikNum(iTrikNum)
	, m_fOpacity(1)
	, m_isOnce(true)
	, m_isFinish(true)
	, m_fEmphasizeCos(1)
	, m_fFadeOutOpacityWait(0)
	, m_pFunc(&TaskTrikNavi::Emphasize)
{
	// �X�v���C�g�쐬
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	m_TaskTransform.SetPosition(KVec3Add(vPos, KVector3{0, 50, 0}));
	// �e�N�X�`�����Z�b�g
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 128;
	texInfo.iTipHeight = 128;
	texInfo.iTipRow = 1;
	texInfo.iTipColumn = 3;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(384, 128, L"GameScene/Images/trik_navi.png", &texInfo);
}

TaskTrikNavi::~TaskTrikNavi()
{

}

void TaskTrikNavi::Update()
{
	switch (m_pGameDirector->GetCurrentEventName())
	{
	case E_EventName::QTEStart:
		// �����̓�x�̃��[�����ۂ��ŃG�t�F�N�g��ύX
		int eDifficulty = static_cast<int>(m_pGameDirector->GetQTEDifficulty());
		if (eDifficulty == m_iTrikNum + 2)
		{
			FadeOut(true);
		}
		else
		{
			FadeOut(false);
		}
		break;
	}

	// ���݂̃G�t�F�N�g�����s
	if (m_pFunc != NULL) (this->*m_pFunc)();
}

void TaskTrikNavi::Draw()
{
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(0, m_iTrikNum);
}

void TaskTrikNavi::Emphasize()
{
	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	// �㉺�ړ�
	m_fEmphasizeCos += GetDeltaTime() / 2; // ����2�b
	float fCos = (float)cos((double)m_fEmphasizeCos * 2 * M_PI);
	float fMovement = fCos * GetDeltaTime() * 20; // ���a20px
	m_TaskTransform.Translate(KVector3{ 0, fMovement, 0 });
}

/*****************************************************************************
	����
*****************************************************************************/
void TaskTrikNavi::FadeOut(bool isSelect)
{
	/*****************************************************************************
		�J�n
	*****************************************************************************/
	if (m_isOnce)
	{
		if (isSelect)
		{
			this->SetFunc(&TaskTrikNavi::SelectFadeOut);
		}
		else
		{
			this->SetFunc(&TaskTrikNavi::FadeOut);
		}
	}
}

void TaskTrikNavi::SelectFadeOut()
{
	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	// ��ړ�
	float fMovement = GetDeltaTime() * 60; // 1�b��60px
	m_TaskTransform.Translate(KVector3{ 0, fMovement, 0 });
	// ����
	m_fFadeOutOpacityWait += fMovement;
	if (30 <= m_fFadeOutOpacityWait)
	{
		m_fOpacity -= GetDeltaTime() * 2; // 0.5�b
		m_pSprite->SetOpacity(m_fOpacity);
	}

	/*****************************************************************************
		�I��
	*****************************************************************************/
	if (m_fOpacity <= 0 || m_isFinish)
	{
		m_pSprite->SetOpacity(0);
		SetTaskStateDying();
		ExitFunc();
	}
}

void TaskTrikNavi::FadeOut()
{
	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	// ����
	m_fOpacity -= GetDeltaTime() * 10; // 0.1�b
	m_pSprite->SetOpacity(m_fOpacity);

	/*****************************************************************************
		�I��
	*****************************************************************************/
	if (m_fOpacity <= 0 || m_isFinish)
	{
		m_pSprite->SetOpacity(0);
		SetTaskStateDying();
		ExitFunc();
	}
}

/*****************************************************************************
	���̑�
*****************************************************************************/
void TaskTrikNavi::SetFunc(void (TaskTrikNavi::* pFunc)())
{
	// �܂��ȑO�̃G�t�F�N�g�����s���Ȃ狭���I�����Ď��̃G�t�F�N�g���Z�b�g
	if (m_pFunc != NULL && !m_isFinish)
	{
		m_isFinish = true;
		(this->*m_pFunc)();
	}
	m_isOnce = false;
	m_isFinish = false;
	m_pFunc = pFunc;
}

void TaskTrikNavi::ExitFunc()
{
	m_isOnce = true;
	m_isFinish = true;
	m_pFunc = NULL;
}