#include "provided.h"
#include <string>
#include <fstream>
#include <cctype>
#include "MyMap.h"
#include "support.h"
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	std::vector<StreetSegment> m_segments;
	size_t m_nSegs;
};

MapLoaderImpl::MapLoaderImpl()
	: m_nSegs(0)
{}

MapLoaderImpl::~MapLoaderImpl()
{}

bool MapLoaderImpl::load(string mapFile)
{
	// load file
	ifstream infile(mapFile);

	// check loading is successful or not
	if (!infile)
		return false;

	std::string nextLine;
	
	while (getline(infile, nextLine))
	{
		StreetSegment newSeg;
		// get the street name
		newSeg.streetName = nextLine;

		// get the street's starting and ending points
		getline(infile, nextLine); // now nextLine contains start and end coordinates info
		string coordinates[4];
		int nCoords = 0;
		std::string num;
		for (size_t i = 0; i < nextLine.size(); i++)
		{
			if (isdigit(nextLine[i]) || nextLine[i] == '.' || nextLine[i] == '-')
				num += nextLine[i];
			else if (!num.empty())
			{
				coordinates[nCoords] = num;
				num.clear();
				nCoords++;
			}
		}
		coordinates[3] = num;
		newSeg.segment = GeoSegment(GeoCoord(coordinates[0], coordinates[1]), GeoCoord(coordinates[2], coordinates[3]));

		// get information about attractions on the street
		getline(infile, nextLine);
		int nAttractions = stoi(nextLine);
		for (int i = 0; i < nAttractions; i++)
		{
			// get info about one new attraction
			getline(infile, nextLine);
			Attraction newAttraction;
			std::string temp, coord1, coord2;
			bool atCoords = false;
			for (size_t j = 0; j < nextLine.size(); j++)
			{
				if (nextLine[j] == '|') // when reach '|', store name of attraction
				{
					newAttraction.name = temp;
					temp.clear();
					atCoords = true;
				}
				else if (!atCoords) // getting the attraction name
					temp += nextLine[j];
				else // getting coordinates
				{
					if (isdigit(nextLine[j]) || nextLine[j] == '.' || nextLine[j] == '-')
						temp += nextLine[j];
					else if (!temp.empty())
					{
						coord1 = temp;
						temp.clear();
					}
				}
			}
			coord2 = temp;
			newAttraction.geocoordinates = GeoCoord(coord1, coord2);
			// store the new attraction in the street segment
			newSeg.attractions.push_back(newAttraction);
		}

		// put the new street segment into the map
		m_segments.push_back(newSeg);
		m_nSegs++;
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_nSegs;
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < 0 || segNum > getNumSegments() - 1)
		return false;
	seg = m_segments[segNum];
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
	return m_impl->getSegment(segNum, seg);
}
