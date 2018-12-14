#ifndef SUPPORT_INCLUDED
#define SUPPORT_INCLUDED

#include "MyMap.h"
#include "provided.h"
#include <string>

bool isSameStreet(const StreetSegment& street1, const StreetSegment& street2);

std::string getDirectionFromDegree(const double& deg);

bool sameCoord(const GeoCoord& coord1, const GeoCoord& coord2);

bool sameCoord(const GeoCoord* coord1, const GeoCoord* coord2);

GeoCoord getCoordOfAnotherEnd(const StreetSegment& street, const GeoCoord& gc);

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs);

bool operator<(const GeoCoord& lhs, const GeoCoord& rhs);

bool operator>(const GeoCoord& lhs, const GeoCoord& rhs);

bool operator<=(const GeoCoord& lhs, const GeoCoord& rhs);

bool operator>=(const GeoCoord& lhs, const GeoCoord& rhs);

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs);

#endif