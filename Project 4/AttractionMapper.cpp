#include "provided.h"
#include <string>
#include "MyMap.h"
#include "support.h"
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	string formalizeName(string oldName) const;
	MyMap<std::string, Attraction> m_attractionMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	// loop through all streets
	for (size_t i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment currStreet;
		ml.getSegment(i, currStreet);
		// loop through all attractions on the street
		for (size_t j = 0; j < currStreet.attractions.size(); j++)
		{
			string name = formalizeName(currStreet.attractions[j].name);
			m_attractionMap.associate(name, currStreet.attractions[j]);
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	// re-format the input attraction name
	string modifiedName = formalizeName(attraction);

	// find the attraction
	const Attraction* temp = m_attractionMap.find(modifiedName);
	if (temp == nullptr)
		return false;
	gc = temp->geocoordinates;
	return true;
}

string AttractionMapperImpl::formalizeName(string oldName) const
{
	string name = oldName;
	for (size_t i = 0; i < name.size(); i++)
	{
		if (i == 0)
			name[0] = toupper(name[0]);
		else if (name[i] == ' ' && i != name.size() - 1)
		{
			name[i + 1] = toupper(name[i + 1]);
			i++;
		}
		else
			name[i] = tolower(name[i]);
	}
	return name;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
