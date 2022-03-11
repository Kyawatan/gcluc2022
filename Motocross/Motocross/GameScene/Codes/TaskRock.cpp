#include "TaskRock.h"
#include "TexQuad.h"
#include "BoxCollider.h"


TaskRock::TaskRock(KVector3 vPos)
	: TaskBase(0, -vPos.z, NULL)
	, m_pSprite(NULL)
	, m_pPlayer(NULL)
	, m_onCollisionDetection(false)
{
	// スプライト作成
	m_pSprite = new TexQuad(&m_TaskTransform, 128, 128, L"GameScene/Images/rock.png", 1, 1);
	m_pSprite->m_transform.SetPosition(KVector3{ 0, 16, 0 }); // イラストの足元をタスクの座標にずらす
	m_TaskTransform.SetPosition(vPos);
	
	// コライダ―作成
	m_pCollider = new BoxCollider(&m_TaskTransform, KVector3{ 128, 128, 0 });
	m_pCollider->m_transform.SetTransform(
		KVector3{ 0, 15, 0 },
		KVector3{ 0, 0, 0 },
		KVector3{ 0.5f, 0.4f, 1 }
	);
	//m_pCollider->AddLine(); // 補助線表示
}

TaskRock::~TaskRock()
{
	delete m_pSprite;
}

void TaskRock::Update()
{
	//if (!m_onCollisionDetection) return;

	if (m_pPlayer == NULL)
	{
		// プレイヤーのインスタンスを得る
		std::list<TaskBase*> playerList = GetAnyLayerNumTask(static_cast<int>(E_TaskLayerNum::Player));
		auto itr = playerList.begin();
		m_pPlayer = dynamic_cast<TaskPlayer*>(*itr);
	}
	// 当たり判定
	COLLISION playerCollision;
	if (m_pCollider->OnHit(m_pPlayer, &playerCollision))
	{
		m_pPlayer->Damage(); // プレイヤーにダメージ
		m_pCollider->SetInvalid(); // コライダ―無効
	}
}

void TaskRock::Draw()
{
	m_pSprite->Draw();
	m_pCollider->DrawLine();
}

void TaskRock::OnCollisionDetection()
{
	m_onCollisionDetection = true;
}