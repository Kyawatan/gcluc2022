/*
	アプリ内共通定義
*/

#pragma once
#include "KawataLib.h"

using namespace Kawata;

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
