#include "provided.h"
#include <vector>
#include "MyMap.h"
#include "support.h"
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	bool isAssociated(const GeoCoord& gc, const StreetSegment& street) const;
	void associateNewStreet(const double location, const StreetSegment& street);
	MyMap<double, std::vector<StreetSegment>> m_map;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	// loop through all streets
	for (size_t i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment currStreet;
		ml.getSegment(i, currStreet);
		// associate the street with start locatiion
		double startCoord = currStreet.segment.start.latitude + currStreet.segment.start.longitude;
		associateNewStreet(startCoord, currStreet);
		
		// associate the street with end location
		double endCoord = currStreet.segment.end.latitude + currStreet.segment.end.longitude;
		associateNewStreet(endCoord, currStreet);

		// associate the street with attraction locations, if any
		for (size_t j = 0; j < currStreet.attractions.size(); j++)
		{
			double coord = currStreet.attractions[j].geocoordinates.latitude
							+ currStreet.attractions[j].geocoordinates.longitude;
			associateNewStreet(coord, currStreet);
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;
	double coordsInDouble = gc.latitude + gc.longitude;
	const vector<StreetSegment> *candidates = m_map.find(coordsInDouble);
	for (size_t i = 0; i < candidates->size(); i++)
	{
		const StreetSegment& candidate = candidates->at(i);
		if (isAssociated(gc, candidate))
			segments.push_back(candidate);
	}
	return segments; 
}

bool SegmentMapperImpl::isAssociated(const GeoCoord& gc, const StreetSegment& street) const
{
	if (gc.latitudeText == street.segment.start.latitudeText && gc.longitudeText == street.segment.start.longitudeText)
		return true;
	if (gc.latitudeText == street.segment.end.latitudeText && gc.longitudeText == street.segment.end.longitudeText)
		return true;
	for (size_t i = 0; i < street.attractions.size(); i++)
	{
		if (gc.latitudeText == street.attractions[i].geocoordinates.latitudeText &&
			gc.longitudeText == street.attractions[i].geocoordinates.longitudeText)
			return true;
	}
	return false;
}

void SegmentMapperImpl::associateNewStreet(const double location, const StreetSegment& street)
{
	std::vector<StreetSegment> * temp_ptr = m_map.find(location);
	if (temp_ptr != nullptr)
	{
		bool hasSameOne = false;
		for (size_t i = 0; i < temp_ptr->size(); i++)
		{
			const StreetSegment& temp = temp_ptr->at(i);
			if (isSameStreet(temp_ptr->at(i), street))
				hasSameOne = true;
		}
		if (!hasSameOne)
			temp_ptr->push_back(street);
	}
	else
	{
		std::vector<StreetSegment> newVec;
		newVec.push_back(street);
		m_map.associate(location, newVec);
	}
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}

