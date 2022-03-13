/*
	�A�v�������ʒ�`
*/

#pragma once
#include "KawataLib.h"

using namespace Kawata;

#define COURSE_LENGTH 14400	// �R�[�X�S��

///****************************************************
//	class
//****************************************************/
//
//class SceneGame;
//
//class TaskPlayer;
//class TaskBackGround;
//class TaskFlag;
//class TaskKobu;
//class TaskKey;
//
//class CameraController;
//class LaneManager;
//class CollisionDetector;
//class QTEController;


/****************************************************
	enum class
****************************************************/

// �V�[����
enum class E_SceneName
{
	Title,
	Game,
	Result,
};

// �^�X�N��Draw�����i�ԍ��̏��������̂���O�ɗ���j
enum class E_TaskDrawNum
{
	BackGround		= 1000,
	Backward		= 900,
	Default			= 500,
	PlayerDefault	= -160,
	Forward			= -500,
	UI				= -1000,
};

// �^�X�N��Layer�ԍ�
enum class E_TaskLayerNum
{
	Player,
	Obstacle,
};