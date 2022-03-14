#include "TaskStealFrame.h"
#include "TexQuad.h"
#include "Line.h"


TaskStealFrame::TaskStealFrame(KVector3 vPos, E_FrameName eFrameName)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::Forward), NULL)
	, m_pSprite(NULL)
	, m_pCollisionLine(NULL)
{
	// スプライト生成
	//switch (eFrameName)
	//{
	//case E_FrameName::Left:
	//	m_pSprite = new TexQuad(&m_TaskTransform, 300, 720, L"GameScene/Images/frame_left.png", 1, 1);
	//	SetDrawNum(static_cast<int>(E_TaskDrawNum::Backward));
	//	break;
	//case E_FrameName::Goal:
	//	//m_pSprite = new TexQuad(&m_TaskTransform, 300, 720, L"GameScene/Images/frame_right_goal.png", 1, 1);
	//	m_pSprite = new TexQuad(&m_TaskTransform, 300, 720, L"GameScene/Images/frame_right_base.png", 1, 1);
	//	break;
	//case E_FrameName::Base:
	//	m_pSprite = new TexQuad(&m_TaskTransform, 300, 720, L"GameScene/Images/frame_right_base.png", 1, 1);
	//	break;
	//}
	m_TaskTransform.SetPosition(vPos);
}

TaskStealFrame::~TaskStealFrame()
{
	delete m_pSprite;
	if (m_pCollisionLine != NULL) delete m_pCollisionLine;
}

void TaskStealFrame::Update()
{

}

void TaskStealFrame::Draw()
{
	//m_pSprite->Draw();

	// デバッグ用補助線描画
	if (m_pCollisionLine != NULL)
	{
		m_pCollisionLine->Draw();
	}
}

void TaskStealFrame::DrawCollisionLine(KVector2 vStart, KVector2 vEnd)
{
	// 既に補助線が有効であればreturn
	if (m_pCollisionLine != NULL) return;
	m_pCollisionLine = new Line(&m_TaskTransform, vStart, vEnd);
}