//
//  GAIndividual.cpp
//  Traveling Salesman
//
//  Created by Alexandre Soares on 1/10/2020.
//  Copyright © 2020 Alexandre Soares. All rights reserved.
//

#include "GAIndividual.h"
#include <GL/glut.h>

GARandomizer GAIndividual::_rand;

GAIndividual::~GAIndividual()
{
    delete[] _chromosome;
}

// Construct individual with random chromosome of given length
GAIndividual::GAIndividual (const unsigned len, const std::vector<TSCity> *cities): _chromosomeSize{len}, _cities_list{cities}
{
    _chromosome = new int[len];
    
    for (unsigned i=0; i != len; ++i)
        _chromosome[i] = i;
    
    for (unsigned i=0; i!= _chromosomeSize; ++i)
        swapGenes(_rand.drawIntegerInRange(0, _chromosomeSize-1),
                  _rand.drawIntegerInRange(0, _chromosomeSize-1));
        

    updateFitness ();
}

// Copy individual
GAIndividual::GAIndividual (const GAIndividual &ind)
: _chromosomeSize(ind._chromosomeSize), _fitness(ind._fitness), _cities_list{ind._cities_list}
{
    _chromosome = new int[_chromosomeSize];
    for (unsigned i=0; i != _chromosomeSize; ++i)
        _chromosome[i] = ind._chromosome[i];
}

void GAIndividual::swapGenes(unsigned g1, unsigned g2)
{
    if (g1 >= _chromosomeSize or g2 >= _chromosomeSize)
        return;
    
    double temp = _chromosome[g1];
    _chromosome[g1] = _chromosome[g2];
    _chromosome[g2] = temp;
    
}

// Assign individual
GAIndividual& GAIndividual::operator=(const GAIndividual &orig)
{
    // Create new chromosome
    int *p = new int[orig._chromosomeSize];
    
    // Copy over the chromosome
    for (unsigned gene=0; gene!= orig._chromosomeSize; ++gene)
        p[gene] = orig._chromosome[gene];
    
    delete[] _chromosome;
    _chromosome = p;
  
    // Assign members;
    _chromosomeSize = orig._chromosomeSize;
    _fitness = orig._fitness;
    _cities_list = orig._cities_list;

    return *this;
}


// Move individual
GAIndividual::GAIndividual(GAIndividual &&orig)
:_chromosomeSize(orig._chromosomeSize), _chromosome(orig._chromosome),
 _fitness(orig._fitness), _cities_list(orig._cities_list)
{
    orig._chromosome = nullptr;
    orig._chromosomeSize = 0;
}

// Move assign
GAIndividual& GAIndividual::operator=(GAIndividual && orig)
{
    _chromosomeSize = orig._chromosomeSize;
    _chromosome = orig._chromosome;
    _fitness = orig._fitness;
    _cities_list = orig._cities_list;
    
    orig._chromosome = nullptr;
    orig._chromosomeSize = 0;
    
    return *this;
}

bool GAIndividual::alreadyHasGene(const unsigned gene) const
{
    for (unsigned i=0; i != _chromosomeSize; ++i)
        if (_chromosome[i] == gene) return true;
    return false;
}

int* GAIndividual::firstEmptyGene() const
{
    for (unsigned i=0; i != _chromosomeSize; ++i)
        if (_chromosome[i] == -1)
            return _chromosome+i;
    return nullptr;
}

// Construct individual by mating two other individuals
GAIndividual::GAIndividual (const GAIndividual &parent1, const GAIndividual &parent2): _chromosomeSize(parent1._chromosomeSize), _cities_list{parent1._cities_list}
{
    // Check to see if parents' chromosome sizes match
    if (parent1._chromosomeSize != parent2._chromosomeSize) {
        _chromosomeSize = 0;
        return;
    }
    
    _chromosome = new int[_chromosomeSize];
    
    // Fill offspring chromosome with invalid genes
    for (unsigned i=0; i < _chromosomeSize; ++i)
        _chromosome[i] = -1;
    
    // Select beginning and end positions to copy parent1's chromosome
    unsigned begin = _rand.drawIntegerInRange(0, _chromosomeSize-1);
    unsigned end = _rand.drawIntegerInRange(begin, _chromosomeSize-1);
    
    // Copy over the selected section of parent1's chromosome
    for (unsigned i=begin; i < end; ++i)
        _chromosome[i] = parent1._chromosome[i];
    
    // Fill the remaining of the chromosome with parent2's genes
    int* gene = nullptr;
    for (unsigned i=0; i != _chromosomeSize; ++i) {
        unsigned index = (end+1+i)%_chromosomeSize;
        if (!alreadyHasGene(parent2._chromosome[index]) and (gene=firstEmptyGene()) != nullptr)
            *gene = parent2._chromosome[index];
    }
            
    updateFitness();
}

// Transcribe chromosome to string
std::string GAIndividual::chromosomeAsString () const
{
    std::string transcript;
    
    if (_chromosomeSize != 0)
        transcript += std::to_string(_chromosome[0]);
    
    for (unsigned gene=1; gene != _chromosomeSize; ++gene)
        transcript += " " + std::to_string(_chromosome[gene]);
    
    return transcript;
}

void GAIndividual::mutate (double mutationRate)
{
    for (unsigned gene=0; gene != _chromosomeSize; ++gene)
        if (_rand.drawNumberBetweenZeroAndOne() < mutationRate)
            swapGenes(gene, _rand.drawIntegerInRange(0, _chromosomeSize-1));
    
    updateFitness();
}

void GAIndividual::updateFitness() const
{
    _fitness = 0.0;
    for (unsigned gene=0; gene!=_chromosomeSize-1; ++gene) {
        unsigned city_index = _chromosome[gene];
        unsigned next_city_index = _chromosome[gene+1];
        _fitness += distance((*_cities_list)[city_index], (*_cities_list)[next_city_index]);
    }
    unsigned last_city = _chromosome[_chromosomeSize-1];
    unsigned first_city = _chromosome[0];
    _fitness += distance((*_cities_list)[last_city], (*_cities_list)[first_city]);
}

bool operator< (const GAIndividual &ind1, const GAIndividual &ind2)
{
    return ind1._fitness > ind2._fitness;
}

void GAIndividual::draw() const
{
    if (_chromosomeSize == 0) return;
    
    for (unsigned i=0; i!=_chromosomeSize-1; ++i) {
        const TSCity &city1 = (*_cities_list)[_chromosome[i]];
        const TSCity &city2 = (*_cities_list)[_chromosome[i+1]];
        glVertex3d(city1.position().x, city1.position().y, city1.position().z);
        glVertex3d(city2.position().x, city2.position().y, city2.position().z);
    }
    const TSCity &cityfinal = (*_cities_list)[_chromosome[_chromosomeSize-1]];
    const TSCity &cityinitial = (*_cities_list)[_chromosome[0]];
    glVertex3d(cityfinal.position().x, cityfinal.position().y, cityfinal.position().z);
    glVertex3d(cityinitial.position().x, cityinitial.position().y, cityinitial.position().z);
}
