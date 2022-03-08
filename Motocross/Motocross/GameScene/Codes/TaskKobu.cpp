#include "TaskKobu.h"
#include "TexQuad.h"
#include "Line.h"


TaskKobu::TaskKobu(KVector3 vPos)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::Default), NULL)
	, m_pSprite(NULL)
	, m_pCollisionLine(NULL)
	, m_pCollisionDetector(NULL)
	, m_vCollisionStart()
	, m_vCollisionEnd()
{
	m_pSprite = new TexQuad(&m_TaskTransform, 500, 720, L"GameScene/Images/kobu.png", 1, 1);
	m_TaskTransform.SetPosition(vPos);
}

TaskKobu::~TaskKobu()
{
	delete m_pSprite;
	if (m_pCollisionDetector != NULL) delete m_pCollisionDetector;
	if (m_pCollisionLine != NULL) delete m_pCollisionLine;
}

void TaskKobu::Update()
{
	if (m_pCollisionDetector != NULL)
	{
		// コブとプレイヤーの当たり判定
		m_pCollisionDetector->CollisionDetection();
	}
}

void TaskKobu::Draw()
{
	if (m_pCollisionLine != NULL)
	{
		// デバッグ用補助線描画
		m_pCollisionLine->Draw();
	}
	m_pSprite->Draw();
}

void TaskKobu::SetCollisionDetector(KVector2 fStart, KVector2 fEnd)
{
	// 既に当たり判定が有効であればreturn
	if (m_pCollisionDetector != NULL) return;

	m_vCollisionStart = fStart;
	m_vCollisionEnd = fEnd;
	m_pCollisionDetector = new CollisionDetector(m_vCollisionStart, m_vCollisionEnd);
}

void TaskKobu::DrawCollisionLine()
{
	// 当たり判定が有効でない、既に補助線が有効であればreturn
	if (m_pCollisionDetector == NULL && m_pCollisionLine != NULL) return;

	m_pCollisionLine = new Line(&m_TaskTransform, m_vCollisionStart, m_vCollisionEnd);
}