#include "TaskKey.h"
#include <assert.h>
#include "ScrapTexQuad.h"

#define KEY_POS_NOW			KVector3{ 0, 200, 0 }
#define KEY_POS_NEXT		KVector3{ 128, 200, 0 }
#define KEY_POS_NEXTNEXT	KVector3{ 256, 200, 0 }

TaskKey::TaskKey(int iKeyCode)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::UI), NULL)
	, m_eCurrentKeyPos(E_KeyPossition::Hide)
	, m_vPos()
	, m_vDiffuse(KVector3 { 0.7f, 0.7f, 0.7f })
	, m_fOpacity(0)
	, m_pSprite(NULL)
	, m_iKeyCode(iKeyCode)
	, m_isOnce(true)
	, m_pFunc(NULL)
{
	// スプライト作成
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	m_pSprite->SetOpacity(0); // 透明
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
	if (m_eCurrentKeyPos == E_KeyPossition::Hide) return;

	// 画面中央に配置
	KVector3 vCameraPos = GetpCamera()->GetEyePt();
	KVector3 vPos = KVec3Add(m_vPos, KVector3{ vCameraPos.x, vCameraPos.y, 0 });
	m_TaskTransform.SetPosition(vPos);
	// 現在のエフェクトを実行
	if (m_pFunc != NULL) (this->*m_pFunc)();
}

void TaskKey::Draw()
{
	if (m_eCurrentKeyPos == E_KeyPossition::Hide) return;
	
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(0, m_iKeyCode);
}

void TaskKey::FadeIn(E_KeyPossition eKeyPos)
{
	assert(eKeyPos != E_KeyPossition::Hide);

	m_eCurrentKeyPos = eKeyPos;
	m_vPos = SetPosition(eKeyPos);
	if (m_eCurrentKeyPos != E_KeyPossition::Now)
	{
		m_pSprite->SetDiffuse(m_vDiffuse); // 暗くしておく
	}
	this->SetFunc(&TaskKey::FadeIn);
}

void TaskKey::FadeIn()
{
	m_fOpacity += GetDeltaTime() * 5; // 0.2秒
	m_pSprite->SetOpacity(m_fOpacity);

	// 終了
	if (1 <= m_fOpacity)
	{
		ExitFunc();
	}
}

void TaskKey::FadeOut()
{
	if (m_isOnce)
	{
		this->SetFunc(&TaskKey::FadeOut);
	}

	m_fOpacity -= GetDeltaTime() * 5; // 0.2秒
	m_pSprite->SetOpacity(m_fOpacity);

	// 終了
	if (m_fOpacity <= 0)
	{
		SetTaskStateDying();
		ExitFunc();
	}
}

void TaskKey::SuccessEffect()
{
	if (m_isOnce)
	{
		this->SetFunc(&TaskKey::SuccessEffect);
	}

	// 終了
	SetTaskStateDying();

	ExitFunc();
}

void  TaskKey::TranslatetoNext()
{
	if (m_isOnce)
	{
		m_eCurrentKeyPos = static_cast<E_KeyPossition>(static_cast<int>(m_eCurrentKeyPos) + 1);
		this->SetFunc(&TaskKey::TranslatetoNext);
	}

	// 移動
	if (m_eCurrentKeyPos == E_KeyPossition::Now)
	{
		// グラフィックを明るくする
		m_vDiffuse = KVec3Add(m_vDiffuse, 0.01f);
		m_pSprite->SetDiffuse(m_vDiffuse);

		//KVector3 vScale = m_pSprite->m_transform.GetScale();
		//if (vScale.x < 2)
		//{
		//	m_pSprite->m_transform.SetScale(KVec3Add(vScale, KVector3{ 0.02f, 0.02f, 0 }));
		//}
	}
	m_vPos = KVec3Add(m_vPos, KVector3{ -1000 * GetDeltaTime(), 0, 0 });

	// 移動が終了していなければreturn
	if (SetPosition(m_eCurrentKeyPos).x < m_vPos.x) return;

	// 終了
	if (m_eCurrentKeyPos == E_KeyPossition::Now)
	{
		m_pSprite->SetDiffuse(KVector3{ 1, 1, 1 });
		//m_pSprite->m_transform.SetScale(KVector3{ 2, 2, 0 });
	}
	m_vPos = SetPosition(m_eCurrentKeyPos);
	ExitFunc();
}

KVector3 TaskKey::SetPosition(E_KeyPossition eKeyPos)
{
	assert(eKeyPos != E_KeyPossition::Hide);

	if (eKeyPos == E_KeyPossition::Now)
	{
		return KEY_POS_NOW;
	}
	else if (eKeyPos == E_KeyPossition::Next)
	{
		return KEY_POS_NEXT;
	}
	else
	{
		return KEY_POS_NEXTNEXT;
	}
}

void TaskKey::SetFunc(void (TaskKey::* pFunc)())
{
	m_isOnce = false;
	m_pFunc = pFunc; 
}

void TaskKey::ExitFunc()
{
	m_isOnce = true;
	m_pFunc = NULL;
}