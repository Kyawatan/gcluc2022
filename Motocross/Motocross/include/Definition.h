/*
	アプリ内共通定義
*/

#pragma once
#include "KawataLib.h"

using namespace Kawata;

#define COURSE_LENGTH 14400	// コース全長


/****************************************************
	enum class
****************************************************/

// シーン名
enum class E_SceneName
{
	Title,
	Game,
	Result,
};

// タスクのDraw順序（番号の小さいものが手前に来る）
enum class E_TaskDrawNum
{
	BackGround		= 1000,
	Backward		= 900,
	Default			= 500,
	PlayerDefault	= -160,
	Forward			= -500,
	UI				= -1000,
	Bear			= -1100,
};

// タスクのLayer番号
enum class E_TaskLayerNum
{
	Player,
	Obstacle,
};