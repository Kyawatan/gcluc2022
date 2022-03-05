#pragma once


// コースのレーン
enum class E_CourseLane
{
	Right,		// 右（画面手前）
	Center,		// 中央
	Left,		// 左（画面奥）
};

enum class E_CourseChange
{
	Right = -1,	// 1つ右へ
	Left = 1,	// 1つ左へ
};

class LaneManager
{
public:
	LaneManager();
	~LaneManager();

	// 現在位置しているレーンを返す
	E_CourseLane GetCurrentLane(float fPos);

	const float GetLaneInterval();
	const float GetLanePos(E_CourseLane eLane);
	const float GetBorderRightCenterPos();
	const float GetBorderCenterLeftPos();

private:
	const float m_fLaneInterval;
	const float m_fLaneCenterPos;
	const float m_fLaneRightPos;
	const float m_fLaneLeftPos;
	const float m_fLaneBorderRightCenterPos;
	const float m_fLaneBorderCenterLeftPos;
};