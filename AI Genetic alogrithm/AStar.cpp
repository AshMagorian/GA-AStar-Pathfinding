#include "AStar.h"
#include "Node.h"

AStar::AStar()
{
	dx[0] = 1;	dy[0] = 0;
	dx[1] = 1;	dy[1] = 1;
	dx[2] = 0;	dy[2] = 1;
	dx[3] =-1;	dy[3] = 1;
	dx[4] =-1;	dy[4] = 0;
	dx[5] =-1;	dy[5] = -1;
	dx[6] = 0;	dy[6] = -1;
	dx[7] = 1;	dy[7] = -1;

}

AStar::~AStar()
{

}

void AStar::RunProgram(int _w, int _h, std::vector<int> _mapData)
{
	for (int i = 0; i < 20; i++) // initialise all map values to 1
	{
		for (int j = 0; j < 20; j++)
		{
			map[i][j] = 1;
		}
	}

	for (int i = 0; i < _h; i++) // put the map data into the array
	{
		for (int j = 0; j < _w; j++)
		{
			map[j][i] = _mapData.at(j + (_w * i));
			std::cout << map[j][i];
			if (map[j][i] == 2) { xStart = j; yStart = i; }
			if (map[j][i] == 3) { xFinish = j; yFinish = i; }
		}
		std::cout << std::endl;
	}

	std::cout << "Map Size (X,Y): " << _w << "," << _h << std::endl;
	std::cout << "Start: " << xStart << "," << yStart << std::endl;
	std::cout << "Finish: " << xFinish << "," << yFinish << std::endl;

	clock_t start = clock();

	std::string route = pathFind(xStart, yStart, xFinish, yFinish);

	if (route == "")
	{
		std::cout << "An empty route generated!" << std::endl;
	}
	clock_t end = clock();
	double time_elapsed = double(end - start);
	std::cout << "Time to calculate the route (ms): " << time_elapsed << std::endl;

	if (route.length() > 0)
	{
		int j; char c;
		int x = xStart;
		int y = yStart;
		for (int i = 0; i < route.length(); i++)
		{
			c = route.at(i);
			j = atoi(&c);
			x = x + dx[j];
			y = y + dy[j];
			map[x][y] = 4;
		}
		map[x][y] = 3;
	}
}

std::string AStar::pathFind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish)
{
	static std::priority_queue<Node> pq[2]; // list of open (not-yet-tried) nodes
	static int pqi; // pq index
	static Node* n0;
	static Node* m0;
	static int i, j, x, y, xdx, ydy;
	static char c;
	pqi = 0;

	// reset the node maps
	for (y = 0; y < m; y++)
	{
		for (x = 0; x < n; x++)
		{
			closed_nodes_map[x][y] = 0;
			open_nodes_map[x][y] = 0;
		}
	}

	// create the start node and push into list of open nodes
	n0 = new Node(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);
	open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map

	// A* search
	while (!pq[pqi].empty())
	{
		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new Node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
			pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

		x = n0->getxPos(); y = n0->getyPos();

		pq[pqi].pop(); // remove the node from the open list
		open_nodes_map[x][y] = 0;
		// mark it on the closed nodes map
		closed_nodes_map[x][y] = 1;

		// quit searching when the goal state is reached
		//if((*n0).estimate(xFinish, yFinish) == 0)
		if (x == xFinish && y == yFinish)
		{
			// generate the path from finish to start
			// by following the directions
			std::string path = "";
			while (!(x == xStart && y == yStart))
			{
				j = dir_map[x][y];
				c = '0' + (j + dir / 2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			// garbage collection
			delete n0;
			// empty the leftover nodes
			while (!pq[pqi].empty()) pq[pqi].pop();
			return path;
		}

		// generate moves (child nodes) in all possible directions
		for (i = 0; i < dir; i++)
		{
			xdx = x + dx[i]; ydy = y + dy[i];

			if (!(xdx<0 || xdx>n - 1 || ydy<0 || ydy>m - 1 || map[xdx][ydy] == 1
				|| closed_nodes_map[xdx][ydy] == 1))
			{
				// generate a child node
				m0 = new Node(xdx, ydy, n0->getLevel(),
					n0->getPriority());
				m0->nextLevel(i, dir);
				m0->updatePriority(xFinish, yFinish);

				// if it is not in the open list then add into that
				if (open_nodes_map[xdx][ydy] == 0)
				{
					open_nodes_map[xdx][ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					// mark its parent node direction
					dir_map[xdx][ydy] = (i + dir / 2) % dir;
				}
				else if (open_nodes_map[xdx][ydy] > m0->getPriority())
				{
					// update the priority info
					open_nodes_map[xdx][ydy] = m0->getPriority();
					// update the parent direction info
					dir_map[xdx][ydy] = (i + dir / 2) % dir;

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqi].top().getxPos() == xdx &&
						pq[pqi].top().getyPos() == ydy))
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pq[pqi].pop(); // remove the wanted node

					// empty the larger size pq to the smaller one
					if (pq[pqi].size() > pq[1 - pqi].size()) pqi = 1 - pqi;
					while (!pq[pqi].empty())
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); // add the better node instead
				}
				else delete m0; // garbage collection
			}
		}
		delete n0; // garbage collection
	}
	return ""; // no route found
}

void AStar::Present(int _w, int _h)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}
	SDL_Event event;
	const int blockSize = 40;
	bool quit = false;
	SDL_Window *window = SDL_CreateWindow("AI",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		_w*blockSize, _h*blockSize, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL); // create the window

	if (!SDL_GL_CreateContext(window))
	{
		throw std::exception();
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0); // create the renderer
	if (!renderer)
	{
		throw std::exception();
	}

	SDL_Rect block{ 0, 0, blockSize, blockSize }; // create the drawing block

	for (int i = 0; i < _h; i++) // put the map data into window
	{
		for (int j = 0; j < _w; j++)
		{
			switch (map[j][i])
			{
			case 0:
				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 200); // grey if empty space
				break;
			case 1:
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // black if wall
				break;
			case 2:
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red if start
				break;
			case 3:
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green if finish
				break;
			case 4:
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // blue represents the path travelled
			default:
				break;
			}

			block.x = (j % _w) * blockSize; // set block x position
			block.y = (i % _h) * blockSize; // set block y position
			SDL_RenderFillRect(renderer, &block); // draw the block
		}
	}

	SDL_RenderPresent(renderer);

	while (!quit) // display loop
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}


	}
	SDL_DestroyWindow(window);
	SDL_Quit();
}