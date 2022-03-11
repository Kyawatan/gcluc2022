#include "TaskRock.h"
#include "TexQuad.h"
#include "BoxCollider.h"


TaskRock::TaskRock(KVector3 vPos)
	: TaskBase(0, -vPos.z, NULL)
	, m_pSprite(NULL)
	, m_pPlayer(NULL)
	, m_onCollisionDetection(false)
{
	// �X�v���C�g�쐬
	m_pSprite = new TexQuad(&m_TaskTransform, 128, 128, L"GameScene/Images/rock.png", 1, 1);
	m_pSprite->m_transform.SetPosition(KVector3{ 0, 16, 0 }); // �C���X�g�̑������^�X�N�̍��W�ɂ��炷
	m_TaskTransform.SetPosition(vPos);
	
	// �R���C�_�\�쐬
	m_pCollider = new BoxCollider(&m_TaskTransform, KVector3{ 128, 128, 0 });
	m_pCollider->m_transform.SetTransform(
		KVector3{ 0, 15, 0 },
		KVector3{ 0, 0, 0 },
		KVector3{ 0.5f, 0.4f, 1 }
	);
	//m_pCollider->AddLine(); // �⏕���\��
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
		// �v���C���[�̃C���X�^���X�𓾂�
		std::list<TaskBase*> playerList = GetAnyLayerNumTask(static_cast<int>(E_TaskLayerNum::Player));
		auto itr = playerList.begin();
		m_pPlayer = dynamic_cast<TaskPlayer*>(*itr);
	}
	// �����蔻��
	COLLISION playerCollision;
	if (m_pCollider->OnHit(m_pPlayer, &playerCollision))
	{
		m_pPlayer->Damage(); // �v���C���[�Ƀ_���[�W
		m_pCollider->SetInvalid(); // �R���C�_�\����
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