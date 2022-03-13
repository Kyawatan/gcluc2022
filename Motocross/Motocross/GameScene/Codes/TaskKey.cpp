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
	// �X�v���C�g�쐬
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	//m_pSprite->SetOpacity(0); // ����
	// �e�N�X�`�����Z�b�g
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

	// ��ʒ����ɔz�u
	KVector3 vCameraPos = GetpCamera()->GetEyePt();
	KVector3 vPos = KVec3Add(m_vPos, KVector3{ vCameraPos.x, vCameraPos.y, 0 });
	m_TaskTransform.SetPosition(vPos);
	// ���݂̃G�t�F�N�g�����s
	if (m_pFunc != NULL) (this->*m_pFunc)();
}

void TaskKey::Draw()
{
	if (m_eCurrentKeyPos == E_KeyPossition::Hide) return;
	
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(0, m_iKeyCode);
}

/*****************************************************************************
	�o��
*****************************************************************************/
void TaskKey::FadeIn(E_KeyPossition eKeyPos)
{
	assert(eKeyPos != E_KeyPossition::Hide);

	/*****************************************************************************
		�J�n
	*****************************************************************************/
	// �J�n�ʒu
	m_eCurrentKeyPos = eKeyPos;
	m_vPos = SetPosition(eKeyPos);
	m_pSprite->m_transform.SetScale(KVector3{ 0, 0, 0 });
	// Now�L�[�ȊO�͈Â�����
	if (m_eCurrentKeyPos != E_KeyPossition::Now)
	{
		m_pSprite->SetDiffuse(m_vDiffuse);
	}
	this->SetFunc(&TaskKey::FadeIn);
}

void TaskKey::FadeIn()
{
	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	KVector3 vScale = m_pSprite->m_transform.GetScale();

	// �g��
	float fScaleAmount = 7 * GetDeltaTime(); // 0.1�b
	m_pSprite->m_transform.SetScale(KVec3Add(vScale, KVector3{ fScaleAmount, fScaleAmount, 0 }));

	/*****************************************************************************
		�I��
	*****************************************************************************/
	if (1 <= vScale.x || m_isFinish)
	{
		m_pSprite->m_transform.SetScale(KVector3{ 1, 1, 0 });
		ExitFunc();
	}
}

/*****************************************************************************
	����
*****************************************************************************/
void TaskKey::FadeOut()
{
	/*****************************************************************************
		�J�n
	*****************************************************************************/
	if (m_isOnce)
	{
		// �܂��\�����Ă��Ȃ���΂��̂܂ܔj��
		if (m_eCurrentKeyPos == E_KeyPossition::Hide)
		{
			SetTaskStateDying();
			return;
		}
		this->SetFunc(&TaskKey::FadeOut);
	}

	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	KVector3 vScale = m_pSprite->m_transform.GetScale();

	// �k��
	float fScaleAmount = 7 * GetDeltaTime(); // 0.1�b
	m_pSprite->m_transform.SetScale(KVec3Sub(vScale, KVector3{ fScaleAmount, fScaleAmount, 0 }));

	/*****************************************************************************
		�I��
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
		�J�n
	*****************************************************************************/
	if (m_isOnce)
	{
		this->SetFunc(&TaskKey::SuccessEffect);
	}

	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	KVector3 vScale = m_pSprite->m_transform.GetScale();

	// �g��
	float fScaleAmount = GetDeltaTime();
	m_pSprite->m_transform.SetScale(KVec3Add(vScale, KVector3{ fScaleAmount, fScaleAmount, 0 }));
	// ����
	m_fOpacity -= GetDeltaTime() * 10; // 0.1�b
	m_pSprite->SetOpacity(m_fOpacity);

	/*****************************************************************************
		�I��
	*****************************************************************************/
	if (m_fOpacity <= 0 || m_isFinish)
	{
		SetTaskStateDying();
		ExitFunc();
	}
}

void TaskKey::FailureEffect()
{
	m_pSprite->SetDiffuse(KVector3{ 1, 0.5, 0 }); // �Ԃ�����
}

/*****************************************************************************
	�ړ�
*****************************************************************************/
void  TaskKey::TranslatetoNext()
{
	/*****************************************************************************
		�J�n
	*****************************************************************************/
	if (m_isOnce)
	{
		m_eCurrentKeyPos = static_cast<E_KeyPossition>(static_cast<int>(m_eCurrentKeyPos) + 1);
		this->SetFunc(&TaskKey::TranslatetoNext);
	}

	/*****************************************************************************
		�J��Ԃ�
	*****************************************************************************/
	// Now�ֈړ�����Ƃ�
	if (m_eCurrentKeyPos == E_KeyPossition::Now)
	{
		// �O���t�B�b�N�𖾂邭����
		if (m_vDiffuse.x < 1)
		{
			m_vDiffuse = KVec3Add(m_vDiffuse, 3 * GetDeltaTime()); // 0.1�b
			m_pSprite->SetDiffuse(m_vDiffuse);
		}
		// �g�傷��
		KVector3 vScale = m_pSprite->m_transform.GetScale();
		if (vScale.x < 1)
		{
			float fScaleAmount = 3 * GetDeltaTime(); // 0.1�b
			m_pSprite->m_transform.SetScale(KVec3Add(vScale, KVector3{ fScaleAmount, fScaleAmount, 0 }));
		}
		else
		{
			m_pSprite->m_transform.SetScale(KVector3{ 1, 1, 0 });
		}
	}
	// �ړ�
	m_vPos = KVec3Add(m_vPos, KVector3{ -1280 * GetDeltaTime(), 0, 0 }); // 0.1�b

	/*****************************************************************************
		�I��
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
	���̑�
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
	// �܂��ȑO�̃G�t�F�N�g�����s���Ȃ狭���I�����Ď��̃G�t�F�N�g���Z�b�g
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