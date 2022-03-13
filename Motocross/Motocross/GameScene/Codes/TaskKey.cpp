#include "TaskKey.h"
#include <assert.h>
#include "ScrapTexQuad.h"

#define KEY_POS_NOW			KVector3{ 0, 200, 0 }
#define KEY_POS_NEXT		KVector3{ 115, 200, 0 }
#define KEY_POS_NEXTNEXT	KVector3{ 230, 200, 0 }

TaskKey::TaskKey(int iKeyCode)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::UI), NULL)
	, m_eCurrentKeyPos(E_KeyPossition::Hide)
	, m_vPos()
	, m_vDiffuse(KVector3 { 0.8f, 0.8f, 0.8f })
	, m_fOpacity(1)
	, m_pSprite(NULL)
	, m_iKeyCode(iKeyCode)
	, m_isOnce(true)
	, m_isFinish(true)
	, m_pFunc(NULL)
{
	// スプライト作成
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	//m_pSprite->SetOpacity(0); // 透明
	// テクスチャをセット
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 256;
	texInfo.iTipHeight = 256;
	texInfo.iTipRow = 3;
	texInfo.iTipColumn = 3;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(768, 768, L"GameScene/Images/keys.png", &texInfo);
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

/*****************************************************************************
	出現
*****************************************************************************/
void TaskKey::FadeIn(E_KeyPossition eKeyPos)
{
	assert(eKeyPos != E_KeyPossition::Hide);

	/*****************************************************************************
		開始
	*****************************************************************************/
	// 開始位置
	m_eCurrentKeyPos = eKeyPos;
	m_vPos = SetPosition(eKeyPos);
	m_pSprite->m_transform.SetScale(KVector3{ 0, 0, 0 });
	// Nowキー以外は暗くする
	if (m_eCurrentKeyPos != E_KeyPossition::Now)
	{
		m_pSprite->SetDiffuse(m_vDiffuse);
	}
	this->SetFunc(&TaskKey::FadeIn);
}

void TaskKey::FadeIn()
{
	/*****************************************************************************
		繰り返し
	*****************************************************************************/
	KVector3 vScale = m_pSprite->m_transform.GetScale();

	// 拡大
	float fScaleAmount = 7 * GetDeltaTime(); // 0.1秒
	m_pSprite->m_transform.SetScale(KVec3Add(vScale, KVector3{ fScaleAmount, fScaleAmount, 0 }));

	/*****************************************************************************
		終了
	*****************************************************************************/
	if (1 <= vScale.x || m_isFinish)
	{
		m_pSprite->m_transform.SetScale(KVector3{ 1, 1, 0 });
		ExitFunc();
	}
}

/*****************************************************************************
	消滅
*****************************************************************************/
void TaskKey::FadeOut()
{
	/*****************************************************************************
		開始
	*****************************************************************************/
	if (m_isOnce)
	{
		// まだ表示していなければそのまま破棄
		if (m_eCurrentKeyPos == E_KeyPossition::Hide)
		{
			SetTaskStateDying();
			return;
		}
		this->SetFunc(&TaskKey::FadeOut);
	}

	/*****************************************************************************
		繰り返し
	*****************************************************************************/
	KVector3 vScale = m_pSprite->m_transform.GetScale();

	// 縮小
	float fScaleAmount = 7 * GetDeltaTime(); // 0.1秒
	m_pSprite->m_transform.SetScale(KVec3Sub(vScale, KVector3{ fScaleAmount, fScaleAmount, 0 }));

	/*****************************************************************************
		終了
	*****************************************************************************/
	if (vScale.x <= 0 || m_isFinish)
	{
		m_pSprite->m_transform.SetScale(KVector3{ 0, 0, 0 });
		SetTaskStateDying();
		ExitFunc();
	}
}

void TaskKey::SuccessEffect()
{
	/*****************************************************************************
		開始
	*****************************************************************************/
	if (m_isOnce)
	{
		this->SetFunc(&TaskKey::SuccessEffect);
	}

	/*****************************************************************************
		繰り返し
	*****************************************************************************/
	KVector3 vScale = m_pSprite->m_transform.GetScale();

	// 拡大
	float fScaleAmount = GetDeltaTime();
	m_pSprite->m_transform.SetScale(KVec3Add(vScale, KVector3{ fScaleAmount, fScaleAmount, 0 }));
	// 透明
	m_fOpacity -= GetDeltaTime() * 10; // 0.1秒
	m_pSprite->SetOpacity(m_fOpacity);

	/*****************************************************************************
		終了
	*****************************************************************************/
	if (m_fOpacity <= 0 || m_isFinish)
	{
		SetTaskStateDying();
		ExitFunc();
	}
}

void TaskKey::FailureEffect()
{
	m_pSprite->SetDiffuse(KVector3{ 1, 0.5, 0 }); // 赤くする
}

/*****************************************************************************
	移動
*****************************************************************************/
void  TaskKey::TranslatetoNext()
{
	/*****************************************************************************
		開始
	*****************************************************************************/
	if (m_isOnce)
	{
		m_eCurrentKeyPos = static_cast<E_KeyPossition>(static_cast<int>(m_eCurrentKeyPos) + 1);
		this->SetFunc(&TaskKey::TranslatetoNext);
	}

	/*****************************************************************************
		繰り返し
	*****************************************************************************/
	// Nowへ移動するとき
	if (m_eCurrentKeyPos == E_KeyPossition::Now)
	{
		// グラフィックを明るくする
		if (m_vDiffuse.x < 1)
		{
			m_vDiffuse = KVec3Add(m_vDiffuse, 3 * GetDeltaTime()); // 0.1秒
			m_pSprite->SetDiffuse(m_vDiffuse);
		}
		// 拡大する
		KVector3 vScale = m_pSprite->m_transform.GetScale();
		if (vScale.x < 1)
		{
			float fScaleAmount = 3 * GetDeltaTime(); // 0.1秒
			m_pSprite->m_transform.SetScale(KVec3Add(vScale, KVector3{ fScaleAmount, fScaleAmount, 0 }));
		}
		else
		{
			m_pSprite->m_transform.SetScale(KVector3{ 1, 1, 0 });
		}
	}
	// 移動
	m_vPos = KVec3Add(m_vPos, KVector3{ -1280 * GetDeltaTime(), 0, 0 }); // 0.1秒

	/*****************************************************************************
		終了
	*****************************************************************************/
	if (m_vPos.x <= SetPosition(m_eCurrentKeyPos).x || m_isFinish)
	{
		if (m_eCurrentKeyPos == E_KeyPossition::Now)
		{
			m_pSprite->SetDiffuse(KVector3{ 1, 1, 1 });
			m_pSprite->m_transform.SetScale(KVector3{ 1, 1, 0 });
		}
		m_vPos = SetPosition(m_eCurrentKeyPos);
		ExitFunc();
	}
}

/*****************************************************************************
	その他
*****************************************************************************/

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
	// まだ以前のエフェクトを実行中なら強制終了して次のエフェクトをセット
	if (m_pFunc != NULL && !m_isFinish)
	{
		m_isFinish = true;
		(this->*m_pFunc)();
	}
	m_isOnce = false;
	m_isFinish = false;
	m_pFunc = pFunc; 
}

void TaskKey::ExitFunc()
{
	m_isOnce = true;
	m_isFinish = true;
	m_pFunc = NULL;
}