//
//  genalg.cpp
//  Traveling Salesman
//
//  Created by Alexandre Soares on 1/10/2020.
//  Copyright © 2020 Alexandre Soares. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>

#include <GL/glut.h>

#include "GAPopulation.h"
#include "TSCity.h"




// Globals
GAIndividual best;
std::vector<TSCity> cities_list;
std::mutex mutex;


// GLUT globals
struct { int x, y; } prevMouseCoords;
struct { double x, y, z; } mapCenter = { 0.0, 0.0, 0.0 };
double angleX=0, angleY=0;


// FreeGLUT Helper functions
void computeMapCenter()
{
    for (auto &city : cities_list) {
        mapCenter.x += city.position().x;
        mapCenter.y += city.position().y;
        mapCenter.z += city.position().z;
    }
    mapCenter.x /= (double)cities_list.size();
    mapCenter.y /= (double)cities_list.size();
    mapCenter.z /= (double)cities_list.size();
}

void reshape(int w, int h)
{
    glViewport(0,0,(GLsizei)w, (GLsizei)h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (double)w/(double)h, 10.0, 300.0);
    
    glMatrixMode(GL_MODELVIEW);

}

void drawCities(const std::vector<TSCity> &cities_list)
{
    
    glColor3d(0.2, 0.2, 0.2);
    
    for (auto city : cities_list) {
      glPushMatrix();
      glTranslated(city.position().x, city.position().y, city.position().z);
      glutSolidSphere(0.8,10,10);
      glPopMatrix();
    }
    
    
}

void drawCircuit(const GAIndividual &ind)
{
    glColor3d(0.7, 0.0, 0.0);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_LINES);
    ind.draw();
    glEnd();
    
}


void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(50.0,50.0,120.0,50.0,50.0,0.0,0.0,1.0,0.0);

}

void display(void)
{
    glLoadIdentity();
    gluLookAt(50.0,50.0,120.0+mapCenter.z,50.0,50.0,0.0,0.0,1.0,0.0);
    
    glTranslated(mapCenter.x, 0.0, mapCenter.z);
    glRotated(angleX,0.0, 1.0, 0.0);
    glTranslated(-mapCenter.x, 0.0, -mapCenter.z);
    
    glTranslated(0.0, mapCenter.y, mapCenter.z);
    glRotated(angleY, 1.0, 0.0, 0.0);
    glTranslated(0.0, -mapCenter.y, -mapCenter.z);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawCities(cities_list);
    mutex.lock();
    drawCircuit(best);
    mutex.unlock();
    
    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN) {
        prevMouseCoords.x = x;
        prevMouseCoords.y = y;
    }
}

void mouseMoved(int x, int y)
{
    angleX += (double)(x-prevMouseCoords.x)/3;
    angleY += (double)(y-prevMouseCoords.y)/3;
    
    prevMouseCoords.x = x;
    prevMouseCoords.y = y;
}

// Function to run the Genetic Algorithm concurrently
void GA()
{
    // Standard parameters
    unsigned int pop_size = 1000;
    double mutation_rate = 0.001;
    double crossover_rate = 0.25;
    unsigned elitism_count = 10;
    unsigned tournament_size = 500;
    unsigned max_generations = 2000;
    unsigned cull_count = 10;
    unsigned chromosome_size = cities_list.size();
    unsigned generation;

    GAPopulation pop (pop_size, chromosome_size, &cities_list);
   
    
    // Start the chronometer before initializing GAPopulation
    auto start_time = std::chrono::high_resolution_clock::now();
        
    generation = 1;
    while (generation < max_generations) {
        std::cout << "Generation: " << generation;
        GAIndividual localBest = pop.getFittest();
        mutex.lock();
        if (best < localBest)
            best = localBest;
        else
            pop.insertIndividual(best);
        mutex.unlock();
        
        /*
        // Print fittest individual
        std::cout << "Best solution: "
        << pop.bestDescription() << " "
        << pop.bestFitness() << std::endl;
        */
        
        std::cout << " Best fitness: " << pop.bestFitness() << std::endl;
        
        pop.mutate(mutation_rate);
        pop = GAPopulation(pop, crossover_rate, elitism_count, tournament_size, cull_count);
        
        ++generation;
        
    }
    
    // Stop the chronometer after we're done
    auto end_time = std::chrono::high_resolution_clock::now();
    
    // Inform end stats
    std::cout << "Found solution in " << generation << " generations" << std::endl;
    std::cout << "Best solution: " << pop.bestDescription() << std::endl;
    std::cout << "Fitness: " << pop.bestFitness() << std::endl;
    
    // Inform how long it took to run things
    auto dur = std::chrono::duration<double>(end_time - start_time);
    std::cout << "Algorithm ran for " << dur.count() << " seconds" << std::endl;
    
}

// Function to join each thread in the list and report results
void wrapUp (std::vector<std::thread> &threads)
{
    for (auto &thread : threads)
        thread.join();
    
    std::cout << "Absolute best solution: "
              << best.chromosomeAsString()
              << " " << best.fitness() << std::endl;
}

// Initialize cities list randomly uniformly in a 100x100x100 grid
void makeRandomCities()
{
    for (int i=0; i<100; ++i)
        cities_list.push_back(TSCity(100u, 100u, 100u));
}

// Initialize cities from a file
void loadCities()
{
    std::ifstream file("cities.txt");
    if (file) {
        std::string line;
        while (getline(file, line)) {
            std::istringstream stream(line);
            double x, y, z;
            stream >> x >> y >> z;
            cities_list.push_back(TSCity(TSCity::Coordinates{x, y, z}));
        }
    }
    else {
        std::cout << "Could not open cities.txt, terminating" << std::endl;
    }
}


int main (int argc, char **argv)
{
    char option;
    
    // Determine if user wants randomly generated cities or wants to load the cities from a file
    std::cout << "Welcome to Traveling Salesman solver" << std::endl;
    std::cout << "Would you like to load cities from cities.txt? (y/n): ";
    std::cin >> option;

    if (std::tolower(option) == 'n') {
        std::cout << "Map will be randomly generated" << std::endl;
        makeRandomCities();
    }
    else {
        loadCities();
    }

    if (cities_list.empty()) {
        std::cout << "Cannot run algorithm on an empty map!" << std::endl;
        return 1;
    }
    
    // Compute center of map
    computeMapCenter();

    // Create and join threads
    std::vector<std::thread> threads;
    for (int i=0; i!=5; ++i)
        threads.push_back(std::thread(GA));
    
    std::thread wrap(wrapUp, std::ref(threads));
    

    // Initialize and run visualization of solution
    glutInit(&argc, argv);
    glutInitWindowSize(700, 700);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Traveling Salesman");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMoved);
    glutIdleFunc(glutPostRedisplay);
    init();
    glutMainLoop();
    
    
    
    return 0;
    
}
