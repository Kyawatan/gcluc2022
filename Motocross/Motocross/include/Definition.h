/*
	アプリ内共通定義
*/

#pragma once
#include "KawataLib.h"

using namespace Kawata;

#define COURSE_LENGTH 5760	// コース全長

/****************************************************
	class
****************************************************/

class SceneGame;

class TaskPlayer;
class TaskBackGround;

class CameraController;


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
	BackGround = 1000,
	Default = 900,
	UI = -1000,
};

// タスクのLayer番号
enum class E_TaskLayerNum
{
	Player,
};