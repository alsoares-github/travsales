//
//  TSCity.cpp
//  Traveling Salesman
//
//  Created by Alexandre Soares on 1/10/2020.
//  Copyright © 2020 Alexandre Soares. All rights reserved.
//

#include <cmath>
#include "TSCity.h"
#include "GARandomizer.h"

double distance(const TSCity &A, const TSCity &B)
{
    return std::sqrt((A._pos.x-B._pos.x)*(A._pos.x-B._pos.x)+
                     (A._pos.y-B._pos.y)*(A._pos.y-B._pos.y)+
                     (A._pos.z-B._pos.z)*(A._pos.z-B._pos.z));
}

// makes random 2D city by setting third coordinate to 0
TSCity::TSCity(unsigned maxX, unsigned maxY)
: TSCity::TSCity(maxX, maxY, 0) {}

TSCity::TSCity(unsigned maxX, unsigned maxY, unsigned maxZ)
{
    GARandomizer rand;
    _pos.x = rand.drawNumberBetweenZeroAndOne()*maxX;
    _pos.y = rand.drawNumberBetweenZeroAndOne()*maxY;
    _pos.z = rand.drawNumberBetweenZeroAndOne()*maxZ;
}

std::string TSCity::positionAsString() const
{
    std::string response;
    response += std::to_string(_pos.x);
    response += " " + std::to_string(_pos.y);
    response += " " + std::to_string(_pos.z);
    
    return response;
}