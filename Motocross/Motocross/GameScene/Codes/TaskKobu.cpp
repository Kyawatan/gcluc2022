#include "TaskKobu.h"
#include "TexQuad.h"
#include "Line.h"


TaskKobu::TaskKobu(KVector3 vPos)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::Default), NULL)
	, m_pSprite(NULL)
	, m_pCollisionLine(NULL)
{
	m_pSprite = new TexQuad(&m_TaskTransform, 520, 720, L"Resources/img/kobu.png", 1, 1);
	m_TaskTransform.SetPosition(vPos);
}

TaskKobu::~TaskKobu()
{
	delete m_pSprite;
	if (m_pCollisionLine != NULL) delete m_pCollisionLine;
}

void TaskKobu::Update()
{

}

void TaskKobu::Draw()
{
	m_pSprite->Draw();

	// デバッグ用補助線描画
	if (m_pCollisionLine != NULL)
	{
		m_pCollisionLine->Draw();
	}
}

void TaskKobu::DrawCollisionLine(KVector2 vStart, KVector2 vEnd)
{
	// 既に補助線が有効であればreturn
	if (m_pCollisionLine != NULL) return;
	m_pCollisionLine = new Line(&m_TaskTransform, vStart, vEnd);
}