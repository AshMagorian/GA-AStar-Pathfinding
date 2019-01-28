#include <vector>
#include <iostream>
#include <ctime>
#include <SDL.h>
#include <random>

#include "Chromosome.h"

class GeneticAlgorithm
{
public:
	GeneticAlgorithm();
	~GeneticAlgorithm();

	void RunProgram(int _w, int _h, std::vector<int> _mapData);

	void Present(int _w, int _h);

private:

	int xStart;
	int yStart;
	int xFinish;
	int yFinish;
	std::vector<std::vector<int>> map; // A 2d vector to hold map data

	int successfulChromasomeNo; // the index number of the chromosome which reaches the end
	bool gaFinish; // Trigger for when the ending condition is met
	int chromosomeSize; // How many values a chromosome can hold
	float crossoverProbability;
	float mutationProbability;
	int numberOfIterations;

	int cNumber; // Number of chromosomes in the population

	std::vector<Chromosome> chromosomes;
};