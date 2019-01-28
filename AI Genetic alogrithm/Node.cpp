#include "Node.h"

Node::Node(int _xp, int _yp, int _d, int _p)
{
	m_xPos = _xp; m_yPos = _yp; m_level = _d; m_priority = _p;
}

Node::~Node()
{

}

void Node::updatePriority(const int & _xDest, const int & _yDest)
{
	m_priority = m_level + estimate(_xDest, _yDest) * 10; //A*
}

void Node::nextLevel(const int & _i, int _dir) // i: direction
{
	m_level += (_dir == 8 ? (_i % 2 == 0 ? 10 : 14) : 10);
}

const int & Node::estimate(const int & _xDest, const int & _yDest) const
{
	static int xd, yd, d;
	xd = _xDest - m_xPos;
	yd = _yDest - m_yPos;

	// Euclidian Distance
	d = static_cast<int>(sqrt(xd*xd + yd * yd));

	// Manhattan distance
	//d=abs(xd)+abs(yd);

	// Chebyshev distance
	//d=max(abs(xd), abs(yd));

	return(d);
}

bool operator<(const Node & _a, const Node & _b)
{
	return _a.getPriority() > _b.getPriority();
}