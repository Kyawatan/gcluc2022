#include "TaskMenuPointer.h"
#define _USE_MATH_DEFINES // math.hをインクルードする前に記述
#include <math.h>
#include "TexQuad.h"

#define PLAY_POS	KVector3{ 166, 79, 0 }
#define BOOK_POS	KVector3{ 210, -63, 0 }
#define HOWTO_POS	KVector3{ 177, -207, 0 }


TaskMenuPointer::TaskMenuPointer()
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::Default), NULL)
	, m_pSprite(NULL)
	, m_currentPos(E_Menu::Play)
{
	m_pSprite = new TexQuad(&m_TaskTransform, 96, 96, L"Resources/img/pointer.png", 1, 1);
	m_TaskTransform.SetPosition(PLAY_POS);
}

TaskMenuPointer::~TaskMenuPointer()
{
	delete m_pSprite;
}

void TaskMenuPointer::Update()
{
	Effect();
}

void TaskMenuPointer::Draw()
{
	m_pSprite->Draw();
}

const E_Menu TaskMenuPointer::GetCurrentPos()
{
	return m_currentPos;
}

void TaskMenuPointer::MoveUp()
{
	if (m_currentPos == E_Menu::Book)
	{
		// ずかん->スタート
		SetPlay();
	}
	else if (m_currentPos == E_Menu::Howto)
	{
		// 遊び方->ずかん
		SetBook();
	}
}

void TaskMenuPointer::MoveDown()
{
	if (m_currentPos == E_Menu::Book)
	{
		// ずかん->遊び方
		SetHowto();
	}
	else if (m_currentPos == E_Menu::Play)
	{
		// スタート->ずかん
		SetBook();
	}
}

void TaskMenuPointer::SetPlay()
{
	m_TaskTransform.SetPosition(PLAY_POS);
	m_TaskTransform.SetRotation(KVector3{ 0, 0, (6.0 / 180.0) * M_PI });
	m_currentPos = E_Menu::Play;
}

void TaskMenuPointer::SetBook()
{
	m_TaskTransform.SetPosition(BOOK_POS);
	m_TaskTransform.SetRotation(KVector3{ 0, 0, -(3.0 / 180.0) * M_PI });
	m_currentPos = E_Menu::Book;
}

void TaskMenuPointer::SetHowto()
{
	m_TaskTransform.SetPosition(HOWTO_POS);
	m_TaskTransform.SetRotation(KVector3{ 0, 0, -(11.0 / 180.0) * M_PI });
	m_currentPos = E_Menu::Howto;
}

void TaskMenuPointer::Effect()
{
	static const float fPeriod = 0.5f; // 周期
	static const float fHeight = 1.0f; // 高さ
	static float fAddNum = 0;
	fAddNum += GetDeltaTime() / fPeriod;
	if ((int)fAddNum % 2 == 0) return;
	float fCos = (float)cos((double)fAddNum * 2 * M_PI);
	float fScaling = fCos * GetDeltaTime() * fHeight;
	m_TaskTransform.SetScale(KVector3{ 1, m_TaskTransform.GetScale().y + fScaling, 1 });
}