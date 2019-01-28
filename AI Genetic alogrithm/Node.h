#ifndef NODE_H
#define NODE_H

#include <cmath>

class Node
{
public:
	Node(int _xp, int _yp, int _d, int _p);

	~Node();

	int getxPos() const { return m_xPos; }
	int getyPos() const { return m_yPos; }
	int getLevel() const { return m_level; }
	int getPriority() const { return m_priority; }

	void updatePriority(const int & xDest, const int & yDest);

	// give better priority to going strait instead of diagonally
	void nextLevel(const int & _i, int _dir); // i: direction

	const int & estimate(const int & _xDest, const int & _yDest) const;
private:

	// current position
	int m_xPos;
	int m_yPos;
	// total distance already travelled to reach the node
	int m_level;
	// priority=level+remaining distance estimate
	int m_priority;  // smaller: higher priority
};

bool operator<(const Node & _a, const Node & _b); // Determine priority (in the priority queue)
#endif