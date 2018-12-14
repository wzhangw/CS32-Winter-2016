#include "support.h"

bool isSameStreet(const StreetSegment& street1, const StreetSegment& street2)
{
	bool cond1 = (street1.segment.start.latitudeText == street2.segment.start.latitudeText);
	bool cond2 = (street1.segment.start.longitudeText == street2.segment.start.longitudeText);
	bool cond3 = (street1.segment.end.latitudeText == street2.segment.end.latitudeText);
	bool cond4 = (street1.segment.end.longitudeText == street2.segment.end.longitudeText);
	return (cond1 && cond2 && cond3 && cond4);
}

std::string getDirectionFromDegree(const double& deg)
{
	if (deg >= 0 && deg <= 22.5)
		return "east";
	if (deg > 22.5 && deg <= 67.5)
		return "northeast";
	if (deg > 67.5 && deg <= 112.5)
		return "north";
	if (deg > 112.5 && deg <= 157.5)
		return "northwest";
	if (deg > 157.5 && deg <= 202.5)
		return "west";
	if (deg > 202.5 && deg <= 247.5)
		return "southwest";
	if (deg > 247.5 && deg <= 292.5)
		return "south";
	if (deg > 292.5 && deg <= 337.5)
		return "southeast";
	if (deg > 337.5 && deg < 360)
		return "east";
	return "not a valid degree";
}

bool sameCoord(const GeoCoord& coord1, const GeoCoord& coord2)
{
	return (coord1.latitudeText == coord2.latitudeText && coord1.longitudeText == coord2.longitudeText);
}

bool sameCoord(const GeoCoord* coord1, const GeoCoord* coord2)
{
	return sameCoord(*coord1, *coord2);
}

GeoCoord getCoordOfAnotherEnd(const StreetSegment& street, const GeoCoord& gc)
{
	if (sameCoord(gc, street.segment.start) || sameCoord(gc, street.segment.end))
	{
		if (sameCoord(gc, street.segment.start))
			return street.segment.end;
		else if (sameCoord(gc, street.segment.end))
			return street.segment.start;
	}
	return GeoCoord();
}

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs)
{
	return sameCoord(lhs, rhs);
}

bool operator<(const GeoCoord& lhs, const GeoCoord& rhs)
{
	if (lhs.latitude < rhs.latitude)
		return true;
	else if (lhs.latitude == rhs.latitude)
		return lhs.longitude < rhs.longitude;
	return false;
}

bool operator>(const GeoCoord& lhs, const GeoCoord& rhs)
{
	if (lhs.latitude > rhs.latitude)
		return true;
	else if (lhs.latitude == rhs.latitude)
		return lhs.longitude > rhs.longitude;
	return false;
}

bool operator<=(const GeoCoord& lhs, const GeoCoord& rhs)
{
	return (lhs.latitude <= rhs.latitude);
}

bool operator>=(const GeoCoord& lhs, const GeoCoord& rhs)
{
	return (lhs.latitude >= rhs.latitude);
}

bool operator!=(const GeoCoord& lhs, const GeoCoord& rhs)
{
	return (!sameCoord(lhs, rhs));
}