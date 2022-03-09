#include "LaneManager.h"
#include <assert.h>

#define LANE_INTERVAL	100
#define LANE_CENTER		-160


LaneManager::LaneManager()
	: m_fLaneInterval(LANE_INTERVAL)
	, m_fLaneCenterPos(LANE_CENTER)
	, m_fLaneRightPos(LANE_CENTER - LANE_INTERVAL)
	, m_fLaneLeftPos(LANE_CENTER + LANE_INTERVAL)
	, m_fLaneBorderRightCenterPos(LANE_CENTER - (LANE_INTERVAL * 0.5f))
	, m_fLaneBorderCenterLeftPos(LANE_CENTER + (LANE_INTERVAL * 0.5f))
{

}

LaneManager::~LaneManager()
{

}

E_CourseLane LaneManager::GetCurrentLane(float fPos)
{
	assert(m_fLaneRightPos <= fPos && fPos <= m_fLaneLeftPos);

	if (m_fLaneRightPos <= fPos && fPos < m_fLaneBorderRightCenterPos)
	{
		return E_CourseLane::Right;
	}
	else if (m_fLaneBorderCenterLeftPos < fPos && fPos <= m_fLaneLeftPos)
	{
		return E_CourseLane::Left;
	}
	else
	{
		return E_CourseLane::Center;
	}
}

const float LaneManager::GetLaneInterval()
{
	return LANE_INTERVAL;
}

const float LaneManager::GetLanePos(E_CourseLane eLane)
{
 	if (eLane == E_CourseLane::Right)
	{
		return m_fLaneRightPos;
	}
	else if (eLane == E_CourseLane::Left)
	{
		return m_fLaneLeftPos;
	}
	else
	{
		return m_fLaneCenterPos;
	}
}

const float LaneManager::GetBorderRightCenterPos()
{
	return m_fLaneBorderRightCenterPos;
}

const float LaneManager::GetBorderCenterLeftPos()
{
	return m_fLaneBorderCenterLeftPos;
}