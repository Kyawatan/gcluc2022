#include "TaskCorn.h"
#include "TexQuad.h"
#include "Line.h"


TaskCorn::TaskCorn(KVector3 vPos)
	: TaskBase(0, vPos.z, NULL)
	, m_pSprite(NULL)
	, m_pCollisionLine(NULL)
	, m_pCollisionDetector(NULL)
	, m_vCollisionStart()
	, m_vCollisionEnd()
{
	// スプライト生成
	m_pSprite = new TexQuad(&m_TaskTransform, 64, 64, L"GameScene/Images/corn.png", 1, 1);
	m_pSprite->m_transform.SetPosition(KVector3{ 0, 16, 0 }); // イラストの足元をタスクの座標にずらす
	m_TaskTransform.SetPosition(vPos);
}

TaskCorn::~TaskCorn()
{
	delete m_pSprite;
	if (m_pCollisionDetector != NULL) delete m_pCollisionDetector;
	if (m_pCollisionLine != NULL) delete m_pCollisionLine;
}

void TaskCorn::Update()
{
	if (m_pCollisionDetector != NULL)
	{
		// コーンとプレイヤーの当たり判定
		m_pCollisionDetector->CollisionDetection();
	}
}

void TaskCorn::Draw()
{
	m_pSprite->Draw();
	if (m_pCollisionLine != NULL)
	{
		// デバッグ用補助線描画
		m_pCollisionLine->Draw();
	}
}

void TaskCorn::SetCollisionDetector(KVector2 vEnd)
{
	// 既に当たり判定が有効であればreturn
	if (m_pCollisionDetector != NULL) return;

	m_vCollisionStart = KVector2{ m_TaskTransform.GetPosition().x, m_TaskTransform.GetPosition().y };
	m_vCollisionEnd = vEnd;
	m_pCollisionDetector = new CollisionDetector(m_vCollisionStart, m_vCollisionEnd);
}

void TaskCorn::DrawCollisionLine()
{
	// 当たり判定が有効でない、既に補助線が有効であればreturn
	if (m_pCollisionDetector == NULL && m_pCollisionLine != NULL) return;

	m_pCollisionLine = new Line(&m_TaskTransform, m_vCollisionStart, m_vCollisionEnd);
}