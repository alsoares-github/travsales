//
//  GAPopulation.h
//  Traveling Salesman
//
//  Created by Alexandre Soares on 1/10/2020.
//  Copyright © 2020 Alexandre Soares. All rights reserved.
//

#ifndef gapopulation_h
#define gapopulation_h

#include <vector>

#include "GAIndividual.h"

class GAPopulation {
private:
    // Member variables
    mutable std::vector<GAIndividual> individuals;
    static GARandomizer rand;
    mutable double fitness {-1.0};
    mutable bool amISorted {false};
    GAPopulation() = default;
    
public:
    GAPopulation (unsigned int, unsigned int, const std::vector<TSCity> *);
    GAPopulation (GAPopulation const &, double, unsigned, unsigned, unsigned);
    
    /*
     * Utility
     */
    double bestFitness () const { return getFittest().fitness (); }
    std::string bestChromosomeAsString () const { return getFittest().chromosomeAsString (); }
    std::string bestDescription () const { return getFittest().description(); }
    void mutate (double);
    void mutate (double, unsigned int);
    GAIndividual& getFittest () const { return getNthFittest (0); }
    void insertIndividual (const GAIndividual &ind) { individuals.push_back(ind); }
    
protected:
    GAIndividual& selectBySpinningWheel () const;
    GAIndividual& selectByTournament (unsigned) const;
    void updateFitness () const;
    void sortIndividualsByFitness () const;
    
    GAIndividual& getNthFittest (unsigned int n) const;

};

#endif