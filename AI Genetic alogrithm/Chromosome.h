#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include <random>
#include <ctime>
#include <iostream>

enum direction { N, NE, E, SE, S, SW, W, NW };
//               0  1   2  3   4  5   6  7
class Chromosome
{
public:
	Chromosome();
	Chromosome(int _cLength);
	~Chromosome();

	void SetRandomValues();

	direction GetDirection(int _index) { return m_values.at(_index); }

	void CalculateEndPos(int _xStart, int _yStart, int _xFinish, int _yFinish, std::vector<std::vector<int>> _map, int _width, int _height);
	float CalculateFitness(int _xFinish, int _yFinish);

	void CalculatePositionStep(int _index, int* _x, int* _y, int _xStart, int _yStart, int _xFinish, int _yFinish, std::vector<std::vector<int>> _map, int _width, int _height);

	void AddValue();

	float GetFitness()								{ return m_fitness; }
	bool GetFinish()								{ return m_finish; }
	void SetFinish(bool _value)						{ m_finish = _value; }

	void setBreedingChance(float _totalFitness)		{ m_breedingChance = (m_fitness / _totalFitness) * 100.0f; }
	float getBreedingChance()						{ return m_breedingChance; }

	direction GetValue(int _index)					{ return m_values.at(_index); }
	void SetValue(int _index, direction _newValue)	{ m_values.at(_index) = _newValue; }

private:

	int m_cLength;

	std::vector<direction> m_values;

	int m_xCurrentPosition;
	int m_yCurrentPosition;
	float m_fitness;
	float m_breedingChance;

	bool m_finish;
};
#endif