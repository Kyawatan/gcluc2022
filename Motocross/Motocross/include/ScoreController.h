#pragma once
#include "Definition.h"
#include "ScoreUIManager.h"

enum class E_Point
{
	TrikBiginner		= 30,
	TrikIntermediate	= 50,
	TrikAdvanced		= 100,
	PointGate			= 10,
	DamageRock			= -10,
};

class ScoreController
{
public:
	friend class SceneGame;
	friend class SceneResult;

	ScoreController();
	~ScoreController();

	void AddPoint(E_Point ePoint);

private:
	ScoreUIManager	m_ScoreUIManager;
	int				m_iTotalScore;

	// friend classÇæÇØÇ™åƒÇ—èoÇ∑
	void Update();
	const int GetTotalScore();
	void SetTotalScore(const int iTotalScore);
};