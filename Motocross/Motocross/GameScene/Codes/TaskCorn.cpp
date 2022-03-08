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
	// �X�v���C�g����
	m_pSprite = new TexQuad(&m_TaskTransform, 64, 64, L"GameScene/Images/corn.png", 1, 1);
	m_pSprite->m_transform.SetPosition(KVector3{ 0, 16, 0 }); // �C���X�g�̑������^�X�N�̍��W�ɂ��炷
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
		// �R�[���ƃv���C���[�̓����蔻��
		m_pCollisionDetector->CollisionDetection();
	}
}

void TaskCorn::Draw()
{
	m_pSprite->Draw();
	if (m_pCollisionLine != NULL)
	{
		// �f�o�b�O�p�⏕���`��
		m_pCollisionLine->Draw();
	}
}

void TaskCorn::SetCollisionDetector(KVector2 vEnd)
{
	// ���ɓ����蔻�肪�L���ł����return
	if (m_pCollisionDetector != NULL) return;

	m_vCollisionStart = KVector2{ m_TaskTransform.GetPosition().x, m_TaskTransform.GetPosition().y };
	m_vCollisionEnd = vEnd;
	m_pCollisionDetector = new CollisionDetector(m_vCollisionStart, m_vCollisionEnd);
}

void TaskCorn::DrawCollisionLine()
{
	// �����蔻�肪�L���łȂ��A���ɕ⏕�����L���ł����return
	if (m_pCollisionDetector == NULL && m_pCollisionLine != NULL) return;

	m_pCollisionLine = new Line(&m_TaskTransform, m_vCollisionStart, m_vCollisionEnd);
}