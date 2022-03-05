#include "TaskBackGround.h"
#include "TexQuad.h"

TaskBackGround::TaskBackGround()
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::BackGround), NULL)
	, m_pSprite(NULL)
{
	m_pSprite = new TexQuad(&m_TaskTransform, 1440, 720, L"GameScene/Images/background.png", 4, 1);
	m_TaskTransform.AddChild(m_pSprite);
	m_TaskTransform.SetPosition(KVector3{ 1440 * 4 * 0.5f, 0, 0 }); // �摜�̒[�����[���h���W��(0,0,0)�ɍ��킹��
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
