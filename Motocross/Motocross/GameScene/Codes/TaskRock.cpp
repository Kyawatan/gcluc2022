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
	// �X�v���C�g�쐬
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	// �e�N�X�`�����Z�b�g
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 128;
	texInfo.iTipHeight = 128;
	texInfo.iTipRow = 1;
	texInfo.iTipColumn = 2;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(256, 128, L"Resources/img/rock.png", &texInfo);
	//m_pSprite->m_transform.SetPosition(KVector3{ 0, -16, 0 }); // �C���X�g�̑������^�X�N�̍��W�ɂ��炷
	m_TaskTransform.SetPosition(vPos);
	
	// �R���C�_�\�쐬
	m_pCollider = new BoxCollider(&m_TaskTransform, KVector3{ 128, 128, 0 });
	m_pCollider->m_transform.SetTransform(
		KVector3{ -20, 0, 0 },
		KVector3{ 0, 0, 0 },
		KVector3{ 0.25f, 0.4f, 1 }
	);
	//m_pCollider->AddLine(); // �⏕���\��

	// �O���t�B�b�N�������_���Ō���
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
		// �v���C���[�̃C���X�^���X�𓾂�
		std::list<TaskBase*> playerList = GetAnyLayerNumTask(static_cast<int>(E_TaskLayerNum::Player));
		auto itr = playerList.begin();
		m_pPlayer = dynamic_cast<TaskPlayer*>(*itr);
	}
	// �����蔻��
	COLLISION playerCollision;
	if (m_pCollider->OnHit(m_pPlayer, &playerCollision))
	{
		m_pPlayer->SetDamage(); // �v���C���[�Ƀ_���[�W
		m_pCollider->SetInvalid(); // �R���C�_�\����
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