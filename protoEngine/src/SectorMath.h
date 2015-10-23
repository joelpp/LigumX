#pragma once

#include "Settings.h"
#include "glm/glm.hpp"

typedef glm::vec2 Coord2;

class SectorMath{
	public:
    /**
     * [Normalized Takes in a World-Space point and returns the coordinates (on a normalized grid) of the sector holding it]
     * @param  UnNormalized [The WS position to check against]
     * @return              [The normalized coordinates of the sector's "lower-left" corner]
     */
    static Coord2 Normalized(Coord2 UnNormalized, float invSectorSize);

    /**
     * [sectorPosFromXY Takes in a WS point and returns the WS coordinates of the sector holding it]
     * @param  longLat [The WS position to check against]
     * @return              [The WS coordinates of the sector's "lower-left" corner]
     */
    static Coord2 sectorPosFromXY(Coord2 longLat, float sectorSize);
};