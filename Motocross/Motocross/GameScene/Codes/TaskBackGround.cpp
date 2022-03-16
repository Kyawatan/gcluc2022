#include "TaskBackGround.h"
#include "TexQuad.h"

#define GAME_TEX_WIDTH	1440
#define GAME_REPEAT_NUM	10


TaskBackGround::TaskBackGround(E_SceneName eCurrentScene)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::BackGround), NULL)
	, m_pSprite(NULL)
	, m_fCourseLength(GAME_TEX_WIDTH * GAME_REPEAT_NUM)
{
	assert(eCurrentScene == E_SceneName::Title || eCurrentScene == E_SceneName::Game);

	switch (eCurrentScene)
	{
	case E_SceneName::Title:
		m_pSprite = new TexQuad(&m_TaskTransform, 1280, 720, L"TitleScene/Images/background.png", 1, 1);
		break;

	case E_SceneName::Game:
		m_pSprite = new TexQuad(&m_TaskTransform, GAME_TEX_WIDTH, 720, L"GameScene/Images/background.png", GAME_REPEAT_NUM, 1);
		m_TaskTransform.SetPosition(KVector3{ m_fCourseLength * 0.5f, 0, 0 }); // 画像の端をワールド座標の(0,0,0)に合わせる
		break;
	}
}

TaskBackGround::~TaskBackGround()
{
	delete m_pSprite;
}

void TaskBackGround::Update()
{

}

void TaskBackGround::Draw()
{
	m_pSprite->Draw();
}

const float TaskBackGround::GetCourseLength()
{
	return m_fCourseLength;
}