#include "TaskRock.h"
#include <random>
#include "ScrapTexQuad.h"
#include "BoxCollider.h"


TaskRock::TaskRock(KVector3 vPos)
	: TaskBase(0, vPos.z, NULL)
	, m_pSprite(NULL)
	, m_pPlayer(NULL)
	, m_onCollisionDetection(false)
	, m_iTipNum(0)
{
	// スプライト作成
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	// テクスチャをセット
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 128;
	texInfo.iTipHeight = 128;
	texInfo.iTipRow = 1;
	texInfo.iTipColumn = 2;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(256, 128, L"Resources/img/rock.png", &texInfo);
	//m_pSprite->m_transform.SetPosition(KVector3{ 0, -16, 0 }); // イラストの足元をタスクの座標にずらす
	m_TaskTransform.SetPosition(vPos);
	
	// コライダ―作成
	m_pCollider = new BoxCollider(&m_TaskTransform, KVector3{ 128, 128, 0 });
	m_pCollider->m_transform.SetTransform(
		KVector3{ -20, 0, 0 },
		KVector3{ 0, 0, 0 },
		KVector3{ 0.25f, 0.4f, 1 }
	);
	//m_pCollider->AddLine(); // 補助線表示

	// グラフィックをランダムで決定
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(0, 1);
	m_iTipNum = (int)distr(eng);
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
		m_pPlayer->SetDamage(); // プレイヤーにダメージ
		m_pCollider->SetInvalid(); // コライダ―無効
	}
}

void TaskRock::Draw()
{
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(0, m_iTipNum);
	m_pCollider->DrawLine();
}

void TaskRock::OnCollisionDetection()
{
	m_onCollisionDetection = true;
}