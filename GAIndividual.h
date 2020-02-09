//
//  GAIndividual.h
//  Traveling Salesman
//
//  Created by Alexandre Soares on 1/10/2020.
//  Copyright © 2020 Alexandre Soares. All rights reserved.
//

#ifndef gaindividual_h
#define gaindividual_h

#include "GARandomizer.h"
#include "TSCity.h"
#include <string>
#include <vector>

class GAIndividual {
    friend bool operator< (const GAIndividual &, const GAIndividual &);
private:
    // Member variables
    int *_chromosome;
    unsigned _chromosomeSize;
    static GARandomizer _rand;
    mutable double _fitness = -1.0;
    const std::vector<TSCity> *_cities_list;
    
      
public:
    /*
     * Constructors
     */
    GAIndividual (const unsigned, const std::vector<TSCity> *);
    GAIndividual (const GAIndividual &, const GAIndividual &); // Construct by mating two individuals
    GAIndividual (const GAIndividual &);
    GAIndividual (GAIndividual &&);
    GAIndividual (): _chromosome(nullptr), _chromosomeSize(0), _fitness(10000000.0) {}
    
    ~GAIndividual();
    
    /*
     * Accessors
     */
    double fitness () const { return _fitness; }
    unsigned chromosomeSize() const { return _chromosomeSize; }
    const std::vector<TSCity>* citiesList() const { return _cities_list; }
    
    /*
     * Operations
     */
    GAIndividual& operator=(const GAIndividual &);
    GAIndividual& operator=(GAIndividual &&);
    void draw() const;
    
    /*
     * Maybe move some of these methods to a helper class
     *
     */
public:
    std::string chromosomeAsString () const;
    void mutate (double);
    
    virtual std::string description () const
    {
        return chromosomeAsString();
    }
    
private:
    /*
     * Should the individual be responsible for evaluating its own fitness?
     */
    void swapGenes(unsigned, unsigned);
    bool alreadyHasGene(const unsigned) const;
    int* firstEmptyGene() const;
    virtual void updateFitness () const;
        
};

bool operator< (const GAIndividual &, const GAIndividual &);

#endif
