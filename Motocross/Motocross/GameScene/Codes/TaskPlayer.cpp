#include "TaskPlayer.h"
#include "ScrapTexQuad.h"

#define RUN_SPEED	250
#define MOVE_SPEED	200

TaskPlayer::TaskPlayer()
	: TaskBase(0, 10, NULL)
	, m_pSprite(NULL)
{
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 256;
	texInfo.iTipHeight = 256;
	texInfo.iTipRow = 1;
	texInfo.iTipColumn = 3;
	m_pSprite = new ScrapTexQuad(&m_TaskTransform, 768, 256, L"GameScene/Images/player.png", &texInfo);
	m_TaskTransform.AddChild(m_pSprite);

	m_pAnim = new KawataAnimation();
	const int num = 3;
	int orders[num] = { 0, 1, 2 };
	float speeds[num] = { 0.1f, 0.1f, 0.1f };
	m_pAnim->SetAnimationInfo(0, num, orders, speeds, 0);
	m_pAnim->SetAnimation(0);

}

TaskPlayer::~TaskPlayer()
{
	delete m_pSprite;
	delete m_pAnim;
}

void TaskPlayer::Update()
{
	m_pAnim->Update();
	AutoRun();
	Move();
}

void TaskPlayer::Draw()
{
	m_pSprite->Draw(m_pAnim->GetCurrentIndex());
}

void TaskPlayer::AutoRun()
{
	m_TaskTransform.Translate(KVector3{ RUN_SPEED * GetDeltaTime(), 0, 0 });
}

void TaskPlayer::Move()
{
	if (GetpKeyState()->Pressed(E_KEY_NAME::W))
	{
		m_TaskTransform.Translate(KVector3{ 0, RUN_SPEED * GetDeltaTime(), 0 });
	}
	if (GetpKeyState()->Pressed(E_KEY_NAME::S))
	{
		m_TaskTransform.Translate(KVector3{ 0, -RUN_SPEED * GetDeltaTime(), 0 });
	}
}