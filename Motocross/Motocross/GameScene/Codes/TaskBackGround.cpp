#include "TaskBackGround.h"
#include "TexQuad.h"

TaskBackGround::TaskBackGround()
	: TaskBase(0, 0, NULL)
	, m_pSprite(NULL)
{
	m_pSprite = new TexQuad(&m_TaskTransform, 1440, 720, L"GameScene/Images/background.png");
	m_TaskTransform.AddChild(m_pSprite);
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
