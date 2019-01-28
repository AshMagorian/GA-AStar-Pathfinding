#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm()
{
	successfulChromasomeNo = 0;
	gaFinish = false;
	chromosomeSize = 5;
	crossoverProbability = 0.7f;
	mutationProbability = 0.05f;
	numberOfIterations = 0;
	cNumber = 8;
	map.resize(20, std::vector<int>(20));
}

GeneticAlgorithm::~GeneticAlgorithm()
{

}

void GeneticAlgorithm::RunProgram(int _w, int _h, std::vector<int> _mapData)
{
	for (int i = 0; i < 20; i++) // initialise all map values to 1
	{
		for (int j = 0; j < 20; j++)
		{
			map[j][i] = 1;
		}
	}

	for (int i = 0; i < _h; i++) // put the map data into the vector
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

	clock_t start = clock();

	chromosomes.resize(cNumber);
	Chromosome *tmpChromosome = new Chromosome(chromosomeSize);

	for (int i = 0; i < cNumber; i++) // Generates the first generation of chromosomes
	{
		chromosomes.at(i) = *tmpChromosome;
		chromosomes.at(i).SetRandomValues();
		chromosomes.at(i).CalculateEndPos(xStart, yStart, xFinish, yFinish, map, _w, _h);
		if (chromosomes.at(i).GetFinish() == true)
		{
			gaFinish = true;
			successfulChromasomeNo = i;
		}
	}

	while (gaFinish == false)  // Main loop of the alogrithm
	{
		std::vector<Chromosome> tempChromosomes;  // Temporory vector to store the children chromosomes
		while (tempChromosomes.size() < cNumber)
		{
			float totalFitness = 0.0f;
			int parent1 = 0;
			int parent2 = -1;

			for (int i = 0; i < cNumber; i++) //calculating the total fitness of all the chromosomes
			{
				totalFitness += chromosomes.at(i).GetFitness();
			}
			for (int i = 0; i < cNumber; i++)
			{
				chromosomes.at(i).setBreedingChance(totalFitness); // sets all the breeding chances of the chromosomes
			}
			std::random_device generator;
			std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
			float rand = distribution(generator) * 100;
			float tmp = 0.0f;
			for (int i = 0; i < cNumber; i++)
			{
				if ((rand >= tmp) && rand < (tmp + chromosomes.at(i).getBreedingChance())) // selecting which chromosome to be the parent
				{
					parent1 = i;
					break;
				}
				else
				{
					tmp += chromosomes.at(i).getBreedingChance();
				}
			}
			while (parent2 == -1)  // Loops to stop both parents being the same
			{
				rand = distribution(generator) * 100;
				tmp = 0.0f;
				for (int i = 0; i < cNumber; i++)
				{
					if (rand >= tmp && rand < (tmp + chromosomes.at(i).getBreedingChance()) && i != parent1)
					{
						parent2 = i;
						break;
					}
					else
					{
						tmp += chromosomes.at(i).getBreedingChance();
					}
				}
			}

			//Crossover

			rand = distribution(generator);
			tmpChromosome = new Chromosome(chromosomeSize);
			direction tempDirection = N;

			if (rand < crossoverProbability)
			{
				rand = int(distribution(generator) * chromosomeSize);
				for (int i = 0; i < chromosomeSize; i++) // Crosses over at a random point
				{
					if (i < rand)
					{
						tmpChromosome->SetValue(i, chromosomes.at(parent1).GetValue(i));
					}
					else
					{
						tmpChromosome->SetValue(i, chromosomes.at(parent2).GetValue(i));
					}
				}
				tempChromosomes.push_back(*tmpChromosome);


				for (int i = 0; i < chromosomeSize; i++)
				{
					if (i < rand)
					{
						tmpChromosome->SetValue(i, chromosomes.at(parent2).GetValue(i));
					}
					else
					{
						tmpChromosome->SetValue(i, chromosomes.at(parent1).GetValue(i));
					}
				}

				tempChromosomes.push_back(*tmpChromosome);
			}
			else // The children will be exactly the same as the parents
			{
				for (int i = 0; i < chromosomeSize; i++)
				{

					tmpChromosome->SetValue(i, chromosomes.at(parent1).GetValue(i));

				}
				tempChromosomes.push_back(*tmpChromosome);
				for (int i = 0; i < chromosomeSize; i++)
				{

					tmpChromosome->SetValue(i, chromosomes.at(parent1).GetValue(i));

				}
				tempChromosomes.push_back(*tmpChromosome);
			}



		}

		//Mutation

		std::random_device generator;
		std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
		for (int i = 0; i < cNumber; i++)
		{
			for (int j = 0; j < chromosomeSize; j++)
			{
				float rand = distribution(generator);
				if (rand < mutationProbability) // Loops through every value of each chromosome and checks to see if it will be mutated
				{
					std::random_device m_generator;
					std::uniform_int_distribution<int> m_distribution(0, 7); // Changes to a random value
					int r = m_distribution(m_generator);
					if (r == 0) { tempChromosomes.at(i).SetValue(j, N); }
					else if (r == 1) { tempChromosomes.at(i).SetValue(j, NE); }
					else if (r == 2) { tempChromosomes.at(i).SetValue(j, E); }
					else if (r == 3) { tempChromosomes.at(i).SetValue(j, SE); }
					else if (r == 4) { tempChromosomes.at(i).SetValue(j, S); }
					else if (r == 5) { tempChromosomes.at(i).SetValue(j, SW); }
					else if (r == 6) { tempChromosomes.at(i).SetValue(j, W); }
					else if (r == 7) { tempChromosomes.at(i).SetValue(j, NW); }
				}
			}
		}

		for (int i = 0; i < cNumber; i++)
		{
			for (int j = 0; j < chromosomeSize; j++)
			{
				chromosomes.at(i).SetValue(j, tempChromosomes.at(i).GetValue(j)); // sets the child chromosomes as the main chromosomes
			}

			chromosomes.at(i).CalculateEndPos(xStart, yStart, xFinish, yFinish, map, _w, _h);
			if (chromosomes.at(i).GetFinish() == true) // Checks if any of the chromosomes have reached the end
			{
				gaFinish = true;
				successfulChromasomeNo = i;
			}
		}

		numberOfIterations++; // Counts how many generations have passed

		if (numberOfIterations % 100 == 0 && numberOfIterations < 10000) // adds an extra move to each chromosome every 100 iterations (Caps after 10000 iterations)
		{
			chromosomeSize++;
			for (int i = 0; i < cNumber; i++)
			{
				chromosomes.at(i).AddValue();
			}
		}

		if (numberOfIterations % 10000 == 0) // Pauses the alogrithm every 10000 iterations
		{
			std::cout << "\nPath not found after " << numberOfIterations << " iterations. Are you sure you want to keep going?" << std::endl;
			system("PAUSE");
		}
	}
	clock_t end = clock();
	std::cout << "A successful chromosome was found after " << numberOfIterations + 1 << " generations!" << std::endl;
	double time_elapsed = double(end - start);
	std::cout << "Time to calculate the route (ms): " << time_elapsed << std::endl;

	chromosomes.at(successfulChromasomeNo).SetFinish(false);
	delete tmpChromosome;
}

void GeneticAlgorithm::Present(int _w, int _h)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	const int blockSize = 40;

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
			default:
				break;
			}

			block.x = (j % _w) * blockSize; // set block x position
			block.y = (i % _h) * blockSize; // set block y position
			SDL_RenderFillRect(renderer, &block); // draw the block
		}
	}

	bool quit = false;
	SDL_Event event;
	int posX, posY; // Positons to draw the blue path blocks
	int i = 0;
	while (!quit) // display loop
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // blue represents the path travelled

		if (i < chromosomeSize)
		{
			if (chromosomes.at(successfulChromasomeNo).GetFinish() == false)
			{
				chromosomes.at(successfulChromasomeNo).CalculatePositionStep(i, &posX, &posY, xStart, yStart, xFinish, yFinish, map, _w, _h);
				block.x = (posX % _w) * blockSize; // set block x position
				block.y = (posY % _h) * blockSize; // set block y position
				SDL_RenderFillRect(renderer, &block); // draw the block
				SDL_Delay(100);
			}
		}

		i++;

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}