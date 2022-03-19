#include "TaskWindowEffect.h"
#include "ColorQuad.h"


TaskWindowEffect::TaskWindowEffect(float fOpacity)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::WindowEffect), NULL)
	, m_pSprite(NULL)
	, m_fOpacity(fOpacity)
	, m_isOnce(true)
	, m_isFinish(true)
	, m_fDarkTime(0)
	, m_fFadeInTime(0)
	, m_fFadeOutTime(0)
	, m_pFunc(NULL)

{
	m_pSprite = new ColorQuad(&m_TaskTransform, 1500, 1000);
	dynamic_cast<ColorQuad*>(m_pSprite)->SetColor(0, 0, 0, 1);
	m_pSprite->SetOpacity(m_fOpacity);
	m_TaskTransform.SetPosition(GetpCamera()->GetEyePt());
}

TaskWindowEffect::~TaskWindowEffect()
{
	delete m_pSprite;
}

void TaskWindowEffect::Update()
{
	if (0 < m_fOpacity)
	{
		m_TaskTransform.SetPosition(GetpCamera()->GetEyePt());
	}
	// ���݂̃G�t�F�N�g�����s
	if (m_pFunc != NULL) (this->*m_pFunc)();
}

void TaskWindowEffect::Draw()
{
	if (0 < m_fOpacity)
	{
		m_pSprite->Draw();
	}
}

void TaskWindowEffect::Dark(float fTime)
{
	/*****************************************************************************
		�J�n
	*****************************************************************************/
	if (m_isOnce)
	{
		m_fDarkTime = fTime;
		SetDrawNum(static_cast<int>(E_TaskDrawNum::UI) - 1); // UI�͎�O�ɕ\��
		this->SetFunc(&TaskWindowEffect::Dark);
	}
}

void TaskWindowEffect::Dark()
{
	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	// �����x���グ��
	m_fOpacity += GetDeltaTime() / m_fDarkTime;
	m_pSprite->SetOpacity(m_fOpacity);

	/*****************************************************************************
		�I��
	*****************************************************************************/
	if (0.5f <= m_fOpacity || m_isFinish)
	{
		m_fOpacity = 0.5f;
		m_pSprite->SetOpacity(m_fOpacity);
		SetDrawNum(static_cast<int>(E_TaskDrawNum::UI));
		ExitFunc();
	}
}

void TaskWindowEffect::FadeIn(float fTime)
{
	/*****************************************************************************
		�J�n
	*****************************************************************************/
	if (m_isOnce)
	{
		m_fFadeInTime = fTime;
		this->SetFunc(&TaskWindowEffect::FadeIn);
	}
}

void TaskWindowEffect::FadeIn()
{
	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	// �����x��������
	m_fOpacity -= GetDeltaTime() / m_fFadeInTime;
	m_pSprite->SetOpacity(m_fOpacity);

	/*****************************************************************************
		�I��
	*****************************************************************************/
	if (m_fOpacity <= 0 || m_isFinish)
	{
		m_fOpacity = 0;
		m_pSprite->SetOpacity(m_fOpacity);
		ExitFunc();
	}
}

void TaskWindowEffect::FadeOut(float fTime)
{
	/*****************************************************************************
		�J�n
	*****************************************************************************/
	if (m_isOnce)
	{
		m_fFadeOutTime = fTime;
		m_pSprite->SetOpacity(m_fOpacity);
		this->SetFunc(&TaskWindowEffect::FadeOut);
	}
}

void TaskWindowEffect::FadeOut()
{
	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	// �����x���グ��
	m_fOpacity += GetDeltaTime() / m_fFadeOutTime;
	m_pSprite->SetOpacity(m_fOpacity);

	/*****************************************************************************
		�I��
	*****************************************************************************/
	if (1 <= m_fOpacity || m_isFinish)
	{
		m_fOpacity = 1;
		m_pSprite->SetOpacity(m_fOpacity);
		ExitFunc();
	}
}

/*****************************************************************************
	���̑�
*****************************************************************************/
void TaskWindowEffect::SetFunc(void (TaskWindowEffect::* pFunc)())
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

void TaskWindowEffect::ExitFunc()
{
	m_isOnce = true;
	m_isFinish = true;
	m_pFunc = NULL;
}