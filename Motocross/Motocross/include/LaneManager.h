#pragma once


// �R�[�X�̃��[��
enum class E_CourseLane
{
	Right,		// �E�i��ʎ�O�j
	Center,		// ����
	Left,		// ���i��ʉ��j
};

enum class E_CourseChange
{
	Right = -1,	// 1�E��
	Left = 1,	// 1����
};

class LaneManager
{
public:
	LaneManager();
	~LaneManager();

	// ���݈ʒu���Ă��郌�[����Ԃ�
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