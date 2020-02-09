//
//  TSCity.h
//  Traveling Salesman
//
//  Created by Alexandre Soares on 1/10/2020.
//  Copyright © 2020 Alexandre Soares. All rights reserved.
//

#ifndef TSCity_h
#define TSCity_h

#include <string>

class TSCity
{
    friend double distance(const TSCity &, const TSCity &);
public:
    struct Coordinates {
        double x;
        double y;
        double z;
    };
private:
    Coordinates _pos;
public:
    TSCity(unsigned, unsigned); // make city with random 2D coordinates
    TSCity(unsigned, unsigned, unsigned); // make city with random 3D coordinates
    TSCity(const Coordinates pos) : _pos {pos} {} // make city with given coordinates
    std::string positionAsString() const;
    Coordinates position() const { return _pos; }
};

double distance(const TSCity &, const TSCity &);

#endif /* TSCity_h */
