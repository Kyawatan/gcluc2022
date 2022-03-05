#include "TaskCorn.h"
#include "TexQuad.h"

TaskCorn::TaskCorn(KVector3 vPos)
	: TaskBase(0, vPos.z, NULL)
	, m_pSprite(NULL)
{
	m_pSprite = new TexQuad(&m_TaskTransform, 64, 64, L"GameScene/Images/corn.png", 1, 1);
	m_pSprite->m_transform.SetPosition(KVector3{ 0, 16, 0 }); // イラストの足元をタスクの座標にずらす
	m_TaskTransform.AddChild(m_pSprite);
	m_TaskTransform.SetPosition(vPos);
}

TaskCorn::~TaskCorn()
{
	delete m_pSprite;
}

void TaskCorn::Update()
{

}

void TaskCorn::Draw()
{
	m_pSprite->Draw();
}
