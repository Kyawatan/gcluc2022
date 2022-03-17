#include "TaskFlag.h"
#include "TexQuad.h"
#include "Line.h"


TaskFlag::TaskFlag(KVector3 vPos)
	: TaskBase(0, vPos.z, NULL)
	, m_pSprite(NULL)
	, m_pCollisionLine(NULL)
{
	// �X�v���C�g����
	m_pSprite = new TexQuad(&m_TaskTransform, 64, 64, L"Resources/img/flag.png", 1, 1);
	m_pSprite->m_transform.SetPosition(KVector3{ 0, 16, 0 }); // �C���X�g�̑������^�X�N�̍��W�ɂ��炷
	m_TaskTransform.SetPosition(vPos);
}

TaskFlag::~TaskFlag()
{
	delete m_pSprite;
	if (m_pCollisionLine != NULL) delete m_pCollisionLine;
}

void TaskFlag::Update()
{

}

void TaskFlag::Draw()
{
	if (m_pCollisionLine != NULL)
	{
		// �f�o�b�O�p�⏕���`��
		m_pCollisionLine->Draw();
	}
	m_pSprite->Draw();
}

void TaskFlag::DrawCollisionLine(KVector2 vStart, KVector2 vEnd)
{
	// ���ɕ⏕�����L���ł����return
	if (m_pCollisionLine != NULL) return;
	m_pCollisionLine = new Line(&m_TaskTransform, vStart, vEnd);
}