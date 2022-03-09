#include "TaskKey.h"
#include "ScrapTexQuad.h"

#define KEY_INTERVAL 100

TaskKey::TaskKey(int iKeyCode)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::UI), NULL)
	, m_pSprite(NULL)
	, m_iKeyCode(iKeyCode)
	, m_onEnable(false)
{
	// スプライト作成
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	// テクスチャをセット
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 128;
	texInfo.iTipHeight = 128;
	texInfo.iTipRow = 3;
	texInfo.iTipColumn = 3;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(384, 384, L"GameScene/Images/keys.png", &texInfo);
}

TaskKey::~TaskKey()
{

}

void TaskKey::Update()
{
	KVector3 vCameraPos = GetpCamera()->GetEyePt();
	m_TaskTransform.SetPosition(KVector3{ vCameraPos.x, vCameraPos.y + 200, 0});
}

void TaskKey::Draw()
{
	if (m_onEnable)
	{
		dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(0, m_iKeyCode);
	}
}

bool TaskKey::GetOnEnable()
{
	return m_onEnable;
}

void TaskKey::SetOnEnable(bool a)
{
	m_onEnable = a;
}