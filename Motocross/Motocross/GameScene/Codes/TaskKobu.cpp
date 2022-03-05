#include "TaskKobu.h"
#include "TexQuad.h"

TaskKobu::TaskKobu(KVector3 vPos)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::Default), NULL)
	, m_pSprite(NULL)
{
	m_pSprite = new TexQuad(&m_TaskTransform, 500, 720, L"GameScene/Images/kobu.png", 1, 1);
	m_TaskTransform.AddChild(m_pSprite);
	m_TaskTransform.SetPosition(vPos);
}

TaskKobu::~TaskKobu()
{
	delete m_pSprite;
}

void TaskKobu::Update()
{

}

void TaskKobu::Draw()
{
	m_pSprite->Draw();
}
