#include "TaskBackGround.h"
#include "TexQuad.h"

#define TEX_WIDTH	1440
#define REPEAT_NUM	4


TaskBackGround::TaskBackGround()
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::BackGround), NULL)
	, m_pSprite(NULL)
	, m_fCourseLength(TEX_WIDTH * REPEAT_NUM)
{
	m_pSprite = new TexQuad(&m_TaskTransform, TEX_WIDTH, 720, L"GameScene/Images/background.png", REPEAT_NUM, 1);
	m_TaskTransform.SetPosition(KVector3{ m_fCourseLength * 0.5f, 0, 0 }); // 画像の端をワールド座標の(0,0,0)に合わせる
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