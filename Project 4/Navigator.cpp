#include "provided.h"
#include "support.h"
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>
using namespace std;

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	MapLoader m_maploader;
	AttractionMapper m_attMapper;
	SegmentMapper m_segMapper;
	struct coordWithRoute
	{
		coordWithRoute() {}
		GeoCoord coord;
	};
	bool isNotInQueue(const GeoCoord& gc, queue<coordWithRoute> coordQueue) const;
	StreetSegment getTheirSegment(const GeoCoord& gc1, const GeoCoord& gc2) const;
	bool isNotInVector(const GeoCoord& gc, vector<GeoCoord>& coordVec) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	bool loadSuccess = m_maploader.load(mapFile);
	if (!loadSuccess)
		return false;
	m_attMapper.init(m_maploader);
	m_segMapper.init(m_maploader);
	return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	directions.clear();
	// check if start and end address are valid
	GeoCoord startCoord, endCoord;
	if (!m_attMapper.getGeoCoord(start, startCoord))
		return NAV_BAD_SOURCE;
	if (!m_attMapper.getGeoCoord(end, endCoord))
		return NAV_BAD_DESTINATION;
	vector<StreetSegment> startSt = m_segMapper.getSegments(startCoord);
	vector<StreetSegment> endSt = m_segMapper.getSegments(endCoord);

	// case: start point and end point at the same street segment
	if (isSameStreet(startSt[0], endSt[0]))
	{
		GeoSegment currSeg(startCoord, endCoord);
		double angle = angleOfLine(currSeg);
		string direction = getDirectionFromDegree(angle);
		double distance = distanceEarthMiles(startCoord, endCoord);
		directions.push_back(NavSegment(direction, startSt[0].streetName, distance, currSeg));
		return NAV_SUCCESS;
	}

	// case: start and end point at different street segments
	vector<GeoCoord> visitedPoints;
	coordWithRoute startPt, endPt;
	startPt.coord = startCoord; endPt.coord = endCoord;
	queue<coordWithRoute> potentialPoints;
	potentialPoints.push(startPt);
	MyMap<GeoCoord, GeoCoord> tracker;

	// explore all possible coordinates until the program reaches the destination
	// does not guarantee the optimal route
	bool foundSuccess = false;
	while (!potentialPoints.empty() && !foundSuccess)
	{
		coordWithRoute curr = potentialPoints.front();
		potentialPoints.pop();
		vector<StreetSegment> associateSegs = m_segMapper.getSegments(curr.coord);
		for (size_t i = 0; i < associateSegs.size(); i++)
		{
			// return true if found the destination on the street
			for (size_t j = 0; j < associateSegs[i].attractions.size(); j++)
			{
				if (sameCoord(associateSegs[i].attractions[j].geocoordinates, endCoord))
				{
					tracker.associate(endCoord, curr.coord);
					foundSuccess = true;
				}
			}

			// add potential next points
			GeoCoord coords[2] = { associateSegs[i].segment.start, associateSegs[i].segment.end };
			for (size_t k = 0; k < 2; k++)
			{
				if (!sameCoord(coords[k], startCoord) && isNotInQueue(coords[k], potentialPoints) 
					&& isNotInVector(coords[k], visitedPoints))
				{
					coordWithRoute newCoord;
					newCoord.coord = coords[k];
					potentialPoints.push(newCoord);
					visitedPoints.push_back(newCoord.coord);
					tracker.associate(newCoord.coord, curr.coord);
				}
			}
		}
	}

	if (!foundSuccess)
		return NAV_NO_ROUTE;

	GeoCoord last = endCoord;
	vector<GeoCoord> route;
	route.push_back(endCoord);
	while (!sameCoord(last, startCoord))
	{
		GeoCoord* rightnow = tracker.find(last);
		route.push_back(*rightnow);
		last = *rightnow;
	}

	bool isStartSeg = true;
	if (foundSuccess)
	{
		StreetSegment prevSt;
		for (size_t i = route.size() - 1; i > 0; i--)
		{
			StreetSegment currSt = getTheirSegment(route[i], route[i - 1]);

			// switch start and end point if necessary (skip if this is the first segment)
			if (!isStartSeg && !sameCoord(currSt.segment.start, route[i]))
			{
				GeoCoord temp = currSt.segment.start;
				currSt.segment.start = currSt.segment.end;
				currSt.segment.end = temp;
			}
			isStartSeg = false;

			// take turn if current street is different from previous one
			if (prevSt.streetName != "" && prevSt.streetName != currSt.streetName)
			{
				double angle = angleBetween2Lines(prevSt.segment, currSt.segment);
				string direction;
				if (angle < 180)
					direction = "left";
				if (angle >= 180)
					direction = "right";
				NavSegment newNav(direction, currSt.streetName);
				directions.push_back(newNav);
			}

			// proceed
			GeoSegment currSeg(route[i], route[i - 1]);
			double angle = angleOfLine(currSeg);
			string direction = getDirectionFromDegree(angle);
			double distance = distanceEarthMiles(route[i], route[i - 1]);
			NavSegment secNewNav(direction, currSt.streetName, distance, currSeg);
			directions.push_back(secNewNav);
			prevSt = currSt;
		}
		return NAV_SUCCESS;
	}
	return NAV_NO_ROUTE; 
}

bool NavigatorImpl::isNotInVector(const GeoCoord& gc, vector<GeoCoord>& coordVec) const
{
	for (size_t i = 0; i < coordVec.size(); i++)
		if (sameCoord(gc, coordVec[i]))
			return false;
	return true;
}

bool NavigatorImpl::isNotInQueue(const GeoCoord& gc, queue<coordWithRoute> coordQueue) const
{
	while (!coordQueue.empty())
	{
		coordWithRoute temp = coordQueue.front();
		coordQueue.pop();
		if (sameCoord(temp.coord, gc))
			return false;
	}
	return true;
}

StreetSegment NavigatorImpl::getTheirSegment(const GeoCoord& gc1, const GeoCoord& gc2) const
{
	vector<StreetSegment> segs1 = m_segMapper.getSegments(gc1);
	vector<StreetSegment> segs2 = m_segMapper.getSegments(gc2);
	for (size_t i = 0; i < segs1.size(); i++)
	{
		for (size_t j = 0; j < segs2.size(); j++)
		{
			if (isSameStreet(segs1[i], segs2[j]))
				return segs1[i];
		}
	}
	// if no common segment, return an empty segment
	return StreetSegment();
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}



