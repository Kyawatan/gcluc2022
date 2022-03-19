#include "TaskBackGround.h"
#include "TexQuad.h"

#define GAME_TEX_WIDTH	1440
#define GAME_REPEAT_NUM	10


TaskBackGround::TaskBackGround(E_SceneName eCurrentScene)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::BackGround), NULL)
	, m_pSprite(NULL)
	, m_fCourseLength(GAME_TEX_WIDTH * GAME_REPEAT_NUM)
	, m_onSlideIn(false)
{
	switch (eCurrentScene)
	{
	case E_SceneName::Title:
		m_pSprite = new TexQuad(&m_TaskTransform, 1280, 720, L"Resources/img/background_title.png", 1, 1);
		break;

	case E_SceneName::Game:
		m_pSprite = new TexQuad(&m_TaskTransform, GAME_TEX_WIDTH, 720, L"Resources/img/background_game.png", GAME_REPEAT_NUM, 1);
		m_TaskTransform.SetPosition(KVector3{ m_fCourseLength * 0.5f, 0, 0 }); // �摜�̒[�����[���h���W��(0,0,0)�ɍ��킹��
		break;

	case E_SceneName::Result:
		m_pSprite = new TexQuad(&m_TaskTransform, 1280, 720, L"Resources/img/background_result.png", 1, 1);
		m_TaskTransform.SetPosition(KVector3{ 0, 700, 0 });
		m_onSlideIn = true;
		break;
	}
}

TaskBackGround::~TaskBackGround()
{
	delete m_pSprite;
}

void TaskBackGround::Update()
{
	if (m_onSlideIn)
	{
		SlideIn();
	}
}

void TaskBackGround::Draw()
{
	m_pSprite->Draw();
}

const float TaskBackGround::GetCourseLength()
{
	return m_fCourseLength;
}

void TaskBackGround::SlideIn()
{
	static const float fSpeed = 2100;
	float fMovement = GetDeltaTime() * fSpeed;
	m_TaskTransform.Translate(KVector3{ 0, -fMovement, 0 });

	if (m_TaskTransform.GetPosition().y <= 0)
	{
		m_onSlideIn = false;
	}
}