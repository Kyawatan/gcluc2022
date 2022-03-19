#include "TaskGate.h"
#include "TexQuad.h"
#include "Line.h"


TaskGame::TaskGame(KVector3 vPos, E_FrameName eFrameName)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::Forward), NULL)
	, m_pSprite(NULL)
	, m_pCollisionLine(NULL)
{
	// スプライト生成
	switch (eFrameName)
	{
	case E_FrameName::Left:
		m_pSprite = new TexQuad(&m_TaskTransform, 300, 720, L"Resources/img/gate_left.png", 1, 1);
		SetDrawNum(static_cast<int>(E_TaskDrawNum::Backward));
		break;
	case E_FrameName::Goal:
		//m_pSprite = new TexQuad(&m_TaskTransform, 300, 720, L"GameScene/Images/frame_right_goal.png", 1, 1);
		m_pSprite = new TexQuad(&m_TaskTransform, 300, 720, L"Resources/img/gate_right.png", 1, 1);
		break;
	case E_FrameName::Base:
		m_pSprite = new TexQuad(&m_TaskTransform, 300, 720, L"Resources/img/gate_right.png", 1, 1);
		break;
	}
	m_TaskTransform.SetPosition(vPos);
}

TaskGame::~TaskGame()
{
	delete m_pSprite;
	if (m_pCollisionLine != NULL) delete m_pCollisionLine;
}

void TaskGame::Update()
{

}

void TaskGame::Draw()
{
	m_pSprite->Draw();

	// デバッグ用補助線描画
	if (m_pCollisionLine != NULL)
	{
		m_pCollisionLine->Draw();
	}
}

void TaskGame::DrawCollisionLine(KVector2 vStart, KVector2 vEnd)
{
	// 既に補助線が有効であればreturn
	if (m_pCollisionLine != NULL) return;
	m_pCollisionLine = new Line(&m_TaskTransform, vStart, vEnd);
}