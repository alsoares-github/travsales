//
//  GAPopulation.cpp
//  Traveling Salesman
//
//  Created by Alexandre Soares on 1/10/2020.
//  Copyright © 2020 Alexandre Soares. All rights reserved.
//

#include <iostream>
#include <algorithm>

#include "GAPopulation.h"

GARandomizer GAPopulation::rand;

/*
 * Constructors
 */

// Create GAPopulation with given number of individuals with given chromosome size
GAPopulation::GAPopulation (unsigned int size, unsigned int chromosomeSize, const std::vector<TSCity> *cities)
{
    for (int person = 0; person < size; ++person)
        individuals.push_back (GAIndividual(chromosomeSize, cities));
    
    updateFitness();
    
}

// Create GAPopulation by applying crossover with existing GAPopulation
GAPopulation::GAPopulation (GAPopulation const &pop, double crossoverRate,
                            unsigned elitismCount, unsigned tournamentSize,
                            unsigned cull_count)
{
    // Start by sorting existing GAPopulation
    pop.sortIndividualsByFitness();
    
    //GARandomizer rand;
    
    for (auto indpbegin = pop.individuals.begin(), indpend = pop.individuals.end(), indp = indpbegin;
         indp != indpend; ++indp) // Iterate through individuals
    {
        if (indp - indpbegin <= pop.individuals.size()-1-cull_count) // Check if individual is at the tail end of fitness
        {
            if (crossoverRate > rand.drawNumberBetweenZeroAndOne() // Check if individual has been selected to mate and
                and indp - indpbegin >= elitismCount)              // is not elite
            {
                GAIndividual &mate = pop.selectByTournament(tournamentSize); // Conditions apply, so find a mate
                GAIndividual offspring = GAIndividual (*indp, mate); // Generate offspring
                
                individuals.push_back(offspring); // Insert offspring in the new GAPopulation
            }
            else // Individual did not mate; forward it to the new GAPopulation
                individuals.push_back(*indp);
        }
        else // Fitness too low; replace
            individuals.push_back(GAIndividual((*indp).chromosomeSize(), (*indp).citiesList()));
        
    }
}


void GAPopulation::updateFitness () const
{
    fitness = 0;
    
    for (auto &ind : individuals)
        fitness += ind.fitness();

}

GAIndividual& GAPopulation::getNthFittest (unsigned int n) const
{
    sortIndividualsByFitness ();
    return individuals[n];
}

GAIndividual& GAPopulation::selectBySpinningWheel() const
{
    // Spin wheel
    updateFitness();
    //double wheelPosition = fitness * GARandomizer().drawNumberBetweenZeroAndOne();
    double wheelPosition = (double)fitness * rand.drawNumberBetweenZeroAndOne();
    
    // Find parent
    double wheel = 0.0;
    for (auto &ind : individuals) {
        wheel += (double)ind.fitness();
        if (wheel > wheelPosition) return ind;
    }
    
    return *(--individuals.end());
}

GAIndividual& GAPopulation::selectByTournament(unsigned tournamentSize) const
{
    std::vector<GAIndividual *> participants;
    
    // Choose who will participate in the tournament
    for (int i=0; i < tournamentSize; ++i) {
        int chosenIndex = rand.drawIntegerInRange(0, (int)individuals.size()-1);
        participants.push_back(&individuals[chosenIndex]);
    }
    
    // Sort the tournament participants by fitness
    std::sort(participants.rbegin(), participants.rend(),
              [] (GAIndividual * const lhs, GAIndividual * const rhs) {
                  return lhs->fitness() > rhs->fitness(); });
    
    // Return the best
    return *(participants[0]);
    
}


void GAPopulation::mutate (double mutationRate)
{
    for (auto &ind : individuals)
        ind.mutate(mutationRate);
    amISorted = false;
}

void GAPopulation::mutate (double mutationRate, unsigned int elitismCount)
{
    for (auto indpbegin = individuals.begin(), indpend = individuals.end(), indp = indpbegin;
    indp != indpend; ++indp)
         if (indp - indpbegin >= elitismCount)
             indp->mutate(mutationRate);
}

void GAPopulation::sortIndividualsByFitness () const
{
    if (!amISorted) {
        std::sort(individuals.rbegin(), individuals.rend());
        amISorted = true;
    }
}
