#include <fstream>
#include <string>
#include <iostream>

#include "GeneticAlgorithm.h"
#include "AStar.h"

std::vector<int> ReadObj(std::string path, int* _w, int* _h); // Reads the map file and puts the contents into a vector

int main(int argc, char *argv[])
{
	int w = 0; // The width of the map
	int h = 0; // The height of the map
	int xStart, yStart, xFinish, yFinish; // the start and finish positions

	std::vector<int> mapData = ReadObj("../maps/HardMap.txt", &w, &h);

	char option = '0';
	std::cout << "Would you like to solve using AStar or Genetic Algorithm?" << std::endl; // Menu to select algorithm method
	std::cout << "(1) AStar" << std::endl;
	std::cout << "(2) Genetic Algorithm" << std::endl;
	while (option != '1' && option != '2')
	{
		std::cin >> option;
	}
	if (option == '1')
	{
		AStar aStarProgram;
		aStarProgram.RunProgram(w, h, mapData);
		aStarProgram.Present(w, h);
	}
	else if(option == '2')
	{
		GeneticAlgorithm gaProgram;
		gaProgram.RunProgram(w, h, mapData);
		gaProgram.Present(w, h);
	}

	return 0;
}

std::vector<int> ReadObj(std::string path, int* _w, int* _h)
{
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		throw std::exception();
	}

	std::string line;
	std::vector<int> data;

	int i = 0;

	while (!file.eof())
	{
		std::getline(file, line); // Stores data in a string

		*_w = atoi(&line.at(i));// Case for if the width is double digits
		if (*_w >= 10) {i += 3;}
		else {i += 2;}

		*_h = atoi(&line.at(i));
		if (*_h >= 10) {i += 3;} // Case for if the height is double digits
		else {i += 2;}

		while (i < line.size())
		{
			if (line.at(i) != ' ') // Ignores spaces
			{
				data.push_back(atoi(&line.at(i))); // Stores data in an int array
			}
			i++;
		}
	}
	return data;
}