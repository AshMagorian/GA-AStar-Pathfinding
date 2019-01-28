#include "Chromosome.h"

Chromosome::Chromosome()
{
	m_cLength = 0;
	m_finish = false;
	m_values.resize(m_cLength);
}

Chromosome::Chromosome(int _cLength)
{
	m_cLength = _cLength;
	m_finish = false;
	m_values.resize(m_cLength);
}

Chromosome::~Chromosome()
{

}

void Chromosome::SetRandomValues()
{
	std::random_device m_generator;
	std::uniform_int_distribution<int> m_distribution(0, 7);

	for (int j = 0; j < m_cLength; j++)
	{
		int r = m_distribution(m_generator);
		if (r == 0) { m_values.at(j) = N; }
		else if (r == 1) { m_values.at(j) = NE; }
		else if (r == 2) { m_values.at(j) = E; }
		else if (r == 3) { m_values.at(j) = SE; }
		else if (r == 4) { m_values.at(j) = S; }
		else if (r == 5) { m_values.at(j) = SW; }
		else if (r == 6) { m_values.at(j) = W; }
		else if (r == 7) { m_values.at(j) = NW; }
	}

}

void Chromosome::CalculateEndPos(int _xStart, int _yStart, int _xFinish, int _yFinish, std::vector<std::vector<int>> _map, int _width, int _height)
{
	float tempFitness = 0.0f;
	m_xCurrentPosition = _xStart;
	m_yCurrentPosition = _yStart;

	int xOldPosition;
	int yOldPosition;

	int tempX = 0;
	int tempY = 0;

	m_fitness = 0;

	for (int i = 0; i < m_cLength; i++)
	{
		xOldPosition = m_xCurrentPosition;
		yOldPosition = m_yCurrentPosition;

		if (m_values.at(i) == N || m_values.at(i) == NW || m_values.at(i) == NE) // if moving north
		{
			m_yCurrentPosition -= 1;
		}
		if (m_values.at(i) == E || m_values.at(i) == NE || m_values.at(i) == SE) // if moving east
		{
			m_xCurrentPosition += 1;
		}
		if (m_values.at(i) == S || m_values.at(i) == SE || m_values.at(i) == SW) // if moving south
		{
			m_yCurrentPosition += 1;
		}
		if (m_values.at(i) == W || m_values.at(i) == NW || m_values.at(i) == SW) // if moving west
		{
			m_xCurrentPosition -= 1;
		}

		if ((m_xCurrentPosition < 0) || (m_yCurrentPosition < 0) || (m_xCurrentPosition >= _width) || (m_yCurrentPosition >= _height)) // Checks for out of bounds
		{
			m_xCurrentPosition = xOldPosition;
			m_yCurrentPosition = yOldPosition;
		}

		if (_map[m_xCurrentPosition][m_yCurrentPosition] == 1) //Moves back if it hits the wall
		{
			m_xCurrentPosition = xOldPosition;
			m_yCurrentPosition = yOldPosition;
		}

		if (m_xCurrentPosition == _xFinish && m_yCurrentPosition == _yFinish) // Sets flag if finished
		{
			m_finish = true;
		}

		tempFitness = CalculateFitness(_xFinish, _yFinish); 
		if (m_fitness <= tempFitness) // Calculates the closest it gets to the finish
		{
			m_fitness = tempFitness;
			tempX = m_xCurrentPosition;
			tempY = m_yCurrentPosition;

		}

	}
}

float Chromosome::CalculateFitness(int _xFinish, int _yFinish)
{
	float xDifference = abs(_xFinish - m_xCurrentPosition);
	float yDifference = abs(_yFinish - m_yCurrentPosition);

	return (1.0f / (xDifference + yDifference + 1.0f));
}

void Chromosome::CalculatePositionStep(int _index, int* _x, int* _y, int _xStart, int _yStart, int _xFinish, int _yFinish, std::vector<std::vector<int>> _map, int _width, int _height)
{
	if (_index == 0)
	{
		m_xCurrentPosition = _xStart;
		m_yCurrentPosition = _yStart;
	}
	int xOldPosition;
	int yOldPosition;

	xOldPosition = m_xCurrentPosition;
	yOldPosition = m_yCurrentPosition;

	if (m_values.at(_index) == N || m_values.at(_index) == NW || m_values.at(_index) == NE) // if moving north
	{
		m_yCurrentPosition -= 1;
	}
	if (m_values.at(_index) == E || m_values.at(_index) == NE || m_values.at(_index) == SE) // if moving east
	{
		m_xCurrentPosition += 1;
	}
	if (m_values.at(_index) == S || m_values.at(_index) == SE || m_values.at(_index) == SW) // if moving south
	{
		m_yCurrentPosition += 1;
	}
	if (m_values.at(_index) == W || m_values.at(_index) == NW || m_values.at(_index) == SW) // if moving west
	{
		m_xCurrentPosition -= 1;
	}

	if (m_xCurrentPosition == _xFinish && m_yCurrentPosition == _yFinish) // Sets flag if finished
	{
		m_finish = true;
	}
	if ((m_xCurrentPosition < 0) || (m_yCurrentPosition < 0) || (m_xCurrentPosition >= _width) || (m_yCurrentPosition >= _height)) // Checks for out of bounds
	{
		m_xCurrentPosition = xOldPosition;
		m_yCurrentPosition = yOldPosition;
	}

	if (_map[m_xCurrentPosition][m_yCurrentPosition] == 1) //Moves back if it hits the wall
	{
		m_xCurrentPosition = xOldPosition;
		m_yCurrentPosition = yOldPosition;
	}

	if (m_xCurrentPosition == _xFinish && m_yCurrentPosition == _yFinish)
	{
		m_finish = true;
	}

	*_x = m_xCurrentPosition;
	*_y = m_yCurrentPosition;
}

void Chromosome::AddValue()
{
	m_cLength++;
	direction tempDirection;

	std::random_device generator;
	std::uniform_int_distribution<int> distribution(0, 7);
	int r = distribution(generator);
	if (r == 0) { tempDirection = N; }
	else if (r == 1) { tempDirection = NE; }
	else if (r == 2) { tempDirection = E; }
	else if (r == 3) { tempDirection = SE; }
	else if (r == 4) { tempDirection = S; }
	else if (r == 5) { tempDirection = SW; }
	else if (r == 6) { tempDirection = W; }
	else if (r == 7) { tempDirection = NW; }

	m_values.push_back(tempDirection); // Adds a random value to the end of the vector
}