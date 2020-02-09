//
//  GARandomizer.h
//  Traveling Salesman
//
//  Created by Alexandre Soares on 1/10/2020.
//  Copyright © 2020 Alexandre Soares. All rights reserved.
//

#ifndef garandomizer_h
#define garandomizer_h


#include <random>

class GARandomizer {
private:
    /*
     * We can use random_device (true random number generator, but slow)
     * or mt19937 (fast pRNG)
     */
    mutable std::random_device rd;
    mutable std::mt19937 mtrd;
public:
    double drawNumberBetweenZeroAndOne() const;
    unsigned short drawZeroOrOne() const;
    int drawIntegerInRange(const int a, const int b);
    
    GARandomizer (): mtrd(rd()) { }
    
};

inline double GARandomizer::drawNumberBetweenZeroAndOne () const
{
    /*
     * The statement below constructs a temporary distribution object just to reply to the request
     * using STL std::uniform_real_distribution. Uncomment to use this method instead
     * return std::uniform_real_distribution<>()(mtrd);
     */
    
    return (double)mtrd()/(mtrd.max()-mtrd.min());
}

inline unsigned short GARandomizer::drawZeroOrOne () const
{
    /*
     * The statement below constructs a temporary distribution object just to reply to the request
     * using STL std::uniform_int_distribution. Uncomment to use this method instead
     * return std::uniform_int_distribution<unsigned short>(0,1)(rd);
     */
    
    return (mtrd() < (mtrd.min()+mtrd.max())/2 ?  0 : 1);
}

inline int GARandomizer::drawIntegerInRange(const int a, const int b)
{
    return std::uniform_int_distribution<>(a, b)(mtrd);
}

#endif