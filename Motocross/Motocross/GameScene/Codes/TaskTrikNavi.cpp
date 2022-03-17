#include "TaskTrikNavi.h"
#define _USE_MATH_DEFINES // math.h�C���N���[�h�O�ɋL�q�K�v
#include <math.h>
#include "ScrapTexQuad.h"


TaskTrikNavi::TaskTrikNavi(GameDirector* pGameDirector, int iIndex, E_TrikDifficulty eDifficulty, KVector3 vPos)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::UI), NULL)
	, m_pSprite(NULL)
	, m_pGameDirector(pGameDirector)
	, m_iTrikNum(static_cast<int>(eDifficulty))
	, m_iJumpRumpIndex(iIndex)
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
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(384, 128, L"Resources/img/trik_navi.png", &texInfo);
}

TaskTrikNavi::~TaskTrikNavi()
{
	delete m_pSprite;
}

void TaskTrikNavi::Update()
{
	// ������������W�����v��̔ԂłȂ����return
	if (m_pGameDirector->GetCurrentJumpRump() != m_iJumpRumpIndex) return;

	if (m_pGameDirector->GetCurrentEventName() == E_EventName::QTEStart)
	{
		// �����̓�x�̃��[�����ۂ��ŃG�t�F�N�g��ύX
		int eDifficulty = static_cast<int>(m_pGameDirector->GetQTEDifficulty());
		if (eDifficulty == m_iTrikNum)
		{
			FadeOut(true);
		}
		else
		{
			FadeOut(false);
		}
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
	static const float fPeriod = 2.0f; // ����2�b
	static const int fRadius = 20; // ���a20px
	m_fEmphasizeCos += GetDeltaTime() / fPeriod;
	float fCos = (float)cos((double)m_fEmphasizeCos * 2 * M_PI);
	float fMovement = fCos * GetDeltaTime() * fRadius;
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
	static const int fOneSecMovement = 60; // 1�b��60px
	m_TaskTransform.Translate(KVector3{ 0, GetDeltaTime() * fOneSecMovement, 0 });
	// 30px��Ɉړ������瓧��
	static const int fWaitPx = 30;
	static const float fSpeedOpacity = 0.5f; // 0.5�b�œ�����
	m_fFadeOutOpacityWait += GetDeltaTime() * fOneSecMovement;
	if (fWaitPx <= m_fFadeOutOpacityWait)
	{
		m_fOpacity -= GetDeltaTime() / fSpeedOpacity;
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
	static const float fSpeedOpacity = 0.1f; // 0.1�b�œ�����
	m_fOpacity -= GetDeltaTime() / fSpeedOpacity;
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