#include "TaskMenu.h"
#include "TexQuad.h"


TaskMenu::TaskMenu(TaskWindowEffect* pWindowEffect)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::Menu), NULL)
	, m_pWindowEffect(pWindowEffect)
	, m_pSprite(NULL)
	, m_fSpeed(2100)
	, m_onSlideIn(false)
	, m_onSlideOut(false)
	, m_fWaitTime(0)
{
	m_pSprite = new TexQuad(&m_TaskTransform, 1280, 720, L"Resources/img/menu_howto.png", 1, 1);
	m_TaskTransform.SetPosition(KVector3{ 0, 700, 0 });
	m_onSlideIn = true;
}

TaskMenu::~TaskMenu()
{
	delete m_pSprite;
}

void TaskMenu::Update()
{
	if (0 < m_fWaitTime)
	{
		m_fWaitTime -= GetDeltaTime();
		if (m_fWaitTime <= 0) SetTaskStateDying();
	}
	else if (m_onSlideOut)
	{
		SlideOut();
	}
	else if (m_onSlideIn)
	{
		SlideIn();
	}
	else if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
	{
		m_onSlideOut = true;
	}
}

void TaskMenu::Draw()
{
	m_pSprite->Draw();
}

void TaskMenu::SlideIn()
{
	float fMovement = GetDeltaTime() * m_fSpeed;
	m_TaskTransform.Translate(KVector3{ 0, -fMovement, 0 });

	if (m_TaskTransform.GetPosition().y <= 0)
	{
		m_onSlideIn = false;
	}
}

void TaskMenu::SlideOut()
{
	float fMovement = GetDeltaTime() * m_fSpeed;
	m_TaskTransform.Translate(KVector3{ 0, fMovement, 0 });

	if (700 <= m_TaskTransform.GetPosition().y)
	{
		m_pWindowEffect->FadeIn(0.8f);
		m_fWaitTime = 1.0f;
		m_onSlideOut = false;
	}
}