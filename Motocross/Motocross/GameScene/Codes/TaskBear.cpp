#include "TaskBear.h"
#include <assert.h>
#define _USE_MATH_DEFINES // math.h���C���N���[�h����O�ɋL�q
#include <math.h>
#include "ScrapTexQuad.h"

#define INITIAL_POS KVector3{ 460, -120, 0 }

TaskBear::TaskBear()
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::Bear), NULL)
	, m_pSprite(NULL)
	, m_vPos(INITIAL_POS)
	, m_iTipNum(0)
	, m_fWaitTime(0)
	, m_fAngle(0)
	, m_onEnable(false)
	, m_isOnce(true)
	, m_isFinish(false)
	, m_pFunc(NULL)
{
	// �X�v���C�g����
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	m_TaskTransform.SetRotation(KVector3{ 0, 0, (1.0 / 9.0) * M_PI });
	// �e�N�X�`�����Z�b�g
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 512;
	texInfo.iTipHeight = 768;
	texInfo.iTipRow = 2;
	texInfo.iTipColumn = 2;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(1024, 1536, L"GameScene/Images/bear_point.png", &texInfo);
}

TaskBear::~TaskBear()
{
	delete m_pSprite;
}

void TaskBear::Update()
{
	if (m_onEnable)
	{
		// �J�������W�ɍ��킹��
		KVector3 vCameraPos = GetpCamera()->GetEyePt();
		KVector3 vPos = KVec3Add(m_vPos, KVector3{ vCameraPos.x, vCameraPos.y, 0 });
		m_TaskTransform.SetPosition(vPos);
	}
	// ���݂̃G�t�F�N�g�����s
	if (m_pFunc != NULL) (this->*m_pFunc)();
}

void TaskBear::Draw()
{
	if (m_onEnable)
	{
		dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(0, m_iTipNum);
	}
}

/*****************************************************************************
	���_
*****************************************************************************/
void TaskBear::SetAppear(int iTipNum)
{
	/*****************************************************************************
		�J�n
	*****************************************************************************/
	if (m_isOnce)
	{
		// ���_�ɍ������O���t�B�b�N��\��
		const float fAddWaitTime = 2.0f;
		const float fDeductWaitTime = 1.0f;
		m_iTipNum = iTipNum;
		if (m_iTipNum < 3)
		{
			m_fWaitTime = fAddWaitTime;
		}
		else
		{
			m_fWaitTime = fDeductWaitTime;
		}
		m_onEnable = true;
		m_isOnce = false;
		this->SetFunc(&TaskBear::Appear);
	}
}

void TaskBear::Appear()
{
	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	if (m_iTipNum < 3)
	{
		// �㉺�ړ�
		static const float fPeriod = 1.0f; // ����1�b
		static const int fRadius = 30; // ���a30px
		m_fAngle += GetDeltaTime() / fPeriod;
		float fSin = (float)sin((double)m_fAngle * 2 * M_PI);
		float fMovement = fSin * GetDeltaTime() * fRadius;
		m_vPos = KVec3Add(m_vPos, KVector3{ 0, fMovement, 0 });
	}
	m_fWaitTime -= GetDeltaTime();

	/*****************************************************************************
		�I��
	*****************************************************************************/
	if (m_fWaitTime <= 0 || m_isFinish)
	{
		// �����ʒu�ɖ߂��Ĕ�\��
		m_vPos = INITIAL_POS;
		m_fWaitTime = 0;
		m_fAngle = 0;
		m_onEnable = false;
		ExitFunc();
	}
}

/*****************************************************************************
	���̑�
*****************************************************************************/
void TaskBear::SetFunc(void (TaskBear::* pFunc)())
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

void TaskBear::ExitFunc()
{
	m_isOnce = true;
	m_isFinish = true;
	m_pFunc = NULL;
}