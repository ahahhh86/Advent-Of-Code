// https://adventofcode.com/2015/day/18
/*--- Day 18: Like a GIF For Your Yard ---
After the million lights incident, the fire code has gotten stricter: now, at most ten thousand lights are allowed. You
arrange them in a 100x100 grid.
Never one to let you down, Santa again mails you instructions on the ideal lighting configuration. With so few lights,
he says, you'll have to resort to animation.
Start by setting your lights to the included initial configuration (your puzzle input). A # means "on", and a . means "off".
Then, animate your grid in steps, where each step decides the next configuration based on the current one. Each light's
next state (either on or off) depends on its current state and the current states of the eight lights adjacent to it
(including diagonals). Lights on the edge of the grid might have fewer than eight neighbors; the missing ones always
count as "off".
For example, in a simplified 6x6 grid, the light marked A has the neighbors numbered 1 through 8, and the light marked B,
which is on an edge, only has the neighbors marked 1 through 5:
	1B5...
	234...
	......
	..123.
	..8A4.
	..765.

The state a light should have next is based on its current state (on or off) plus the number of neighbors that are on:
	A light which is on stays on when 2 or 3 neighbors are on, and turns off otherwise.
	A light which is off turns on if exactly 3 neighbors are on, and stays off otherwise.

All of the lights update simultaneously; they all consider the same current state before moving to the next.
Here's a few steps from an example configuration of another 6x6 grid:

Initial state:
	.#.#.#
	...##.
	#....#
	..#...
	#.#..#
	####..

After 1 step:
	..##..
	..##.#
	...##.
	......
	#.....
	#.##..

After 2 steps:
	..###.
	......
	..###.
	......
	.#....
	.#....

After 3 steps:
	...#..
	......
	...#..
	..##..
	......
	......

After 4 steps:
	......
	......
	..##..
	..##..
	......
	......

After 4 steps, this example has four lights on.
In your grid of 100x100 lights, given your initial configuration, how many lights are on after 100 steps?
Your puzzle answer was 1061.

--- Part Two ---
You flip the instructions over; Santa goes on to point out that this is all just an implementation of Conway's Game of Life.
At least, it was, until you notice that something's wrong with the grid of lights you bought: four lights, one in each corner,
are stuck on and can't be turned off. The example above will actually run like this:

Initial state:
	##.#.#
	...##.
	#....#
	..#...
	#.#..#
	####.#

After 1 step:
	#.##.#
	####.#
	...##.
	......
	#...#.
	#.####

After 2 steps:
	#..#.#
	#....#
	.#.##.
	...##.
	.#..##
	##.###

After 3 steps:
	#...##
	####.#
	..##.#
	......
	##....
	####.#

After 4 steps:
	#.####
	#....#
	...#..
	.##...
	#.....
	#.#..#

After 5 steps:
	##.###
	.##..#
	.##...
	.##...
	#.#...
	##...#

After 5 steps, this example now has 17 lights on.
In your grid of 100x100 lights, given your initial configuration, but with the four corners always in the on state,
how many lights are on after 100 steps?
Your puzzle answer was 1006.*/



//=== Include ================================================================
#include "Day18.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"
#include "../aoc/TemplateContainer.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Constants ==============================================================
constexpr int grid_size {100};



//=== Types ==================================================================
using lightGrid_t = Grid<signed char>;



//=== Class InputData ========================================================
class LightGrid {
public:
// Constructors / destructor
	LightGrid() = default;
	LightGrid(const LightGrid&) = default;
	LightGrid(LightGrid&&) = default;
	~LightGrid() = default;


	// --- InputData() ---
	explicit LightGrid(std::ifstream input)
	{
		char buffer {};
		auto i {m_input.begin()};

		while (!input.eof()) {
			input >> buffer;
			switch (buffer) {
			case '.':
				*i = 0;
				std::advance(i, 1);
				break;

			case '#':
				*i = 1;
				std::advance(i, 1);
				break;

			case '\r':
			case '\n':
				// Do nothing
				break;

			default:
				THROW_ERROR(invalid_input_file_data);
				break;
			}
		}
		m_new_lights = m_input;
	}


// Operators
	LightGrid& operator=(const LightGrid&) = delete;
	LightGrid& operator=(LightGrid&&) = delete;


// Functions
	// --- accumulateData() ---
	auto accumulateData() const
	{
		return m_input.accumulateData();
	}


	// --- animateLights() ---
	void animateLights()
	{
		int numAdjacentLights {};

		for (lightGrid_t::dimension_t j, i = 0; i < grid_size; ++i) {
			for (j = 0; j < grid_size; ++j) {
				numAdjacentLights = sumAdjacentLights(i, j);
				signed char& currentLight = m_new_lights[lightGrid_t::Position {i, j}]; // just for readability

				if (m_input[lightGrid_t::Position {i, j}] != 0) {
					if ((numAdjacentLights == 2) || (numAdjacentLights == 3)) { // stay alive // @suppress("Avoid magic numbers")
						currentLight = 1;
					} else { // die
						currentLight = 0;
					}
				} else {
					if (numAdjacentLights == 3) { // be born // @suppress("Avoid magic numbers")
						currentLight = 1;
					} else { // stay dead
						currentLight = 0;
					}
				}
			}
		}

		m_input = m_new_lights;
	}


	// --- sumAdjacentLights() ---
	int sumAdjacentLights(lightGrid_t::dimension_t x, lightGrid_t::dimension_t y)
	{
		int result = m_input[lightGrid_t::Position {x, y}] ? -1 : 0; // if middle is alive, reduce by one, else it must have been excluded from loop

		for (int j, i = -1; i < 2; ++i) {
			for (j = -1; j < 2; ++j) {
				try {
					// May cause underflow, but catch should handle this nevertheless
					if (m_input.at(lightGrid_t::Position {x + i, y + j}) != 0) {
						++result;
					}
				} catch (const std::exception&) {
					// Do nothing
				}
			}
		}

		return result;
	}


	// --- turnOnEdges() ---
	void turnOnEdges()
	{
		m_input[Grid<signed char>::Position {0, 0}] = 1;
		m_input[Grid<signed char>::Position {0, grid_size - 1}] = 1;
		m_input[Grid<signed char>::Position {grid_size - 1, 0}] = 1;
		m_input[Grid<signed char>::Position {grid_size - 1, grid_size - 1}] = 1;
	}


private:
// Variables
	lightGrid_t m_input {grid_size, grid_size};
	lightGrid_t m_new_lights {grid_size, grid_size}; // to avoid multiple allocations
};



} /* anonymous namespace */



//=== Class Day18 ============================================================
// --- Day18::solve() ---
void Day18::solve()
{
	try {
		constexpr int loop_end {100};
		LightGrid inputData {m_IO.getInputFile()};
		LightGrid gridPart1 {inputData};
		LightGrid gridPart2 {inputData};
		m_IO.printFileValid();

		for (int i {0}; i < loop_end; ++i) {
			gridPart1.animateLights();
		}
		m_IO.printSolution(gridPart1.accumulateData(), EPart::Part1);

		gridPart2.turnOnEdges();
		for (int i {0}; i < loop_end; ++i) {
			gridPart2.animateLights();
			gridPart2.turnOnEdges();
		}
		m_IO.printSolution(gridPart2.accumulateData(), EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
