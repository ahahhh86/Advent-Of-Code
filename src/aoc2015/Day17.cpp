// https://adventofcode.com/2015/day/17
/*--- Day 17: No Such Thing as Too Much ---
The elves bought too much eggnog again - 150 liters this time. To fit it all into your refrigerator, you'll need to move
it into smaller containers. You take an inventory of the capacities of the available containers.
For example, suppose you have containers of size 20, 15, 10, 5, and 5 liters. If you need to store 25 liters, there are
four ways to do it:
	15 and 10
	20 and 5 (the first 5)
	20 and 5 (the second 5)
	15, 5, and 5

Filling all containers entirely, how many different combinations of containers can exactly fit all 150 liters of eggnog?
Your puzzle answer was 1638.

--- Part Two ---
While playing with all the containers in the kitchen, another load of eggnog arrives! The shipping and receiving
department is requesting as many containers as you can spare.
Find the minimum number of containers that can exactly fit all 150 liters of eggnog. How many different ways can you
fill that number of containers and still hold exactly 150 litres?
In the example above, the minimum number of containers was two. There were three ways to use that many containers, and
so the answer there would be 3.
Your puzzle answer was 17.*/



//=== Include ================================================================
#include "Day17.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
using vecSize_t = std::vector<std::string>::size_type;



//=== Constants ==============================================================
constexpr int eggnog_volume {150};



//=== Class InputData ========================================================
class InputData {
public:
// Constructors / destructor
	InputData() = delete;
	InputData(const InputData&) = delete;
	InputData(InputData&&) = delete;
	~InputData() = default;


	// --- InputData() ---
	explicit InputData(std::ifstream& input)
	{
		int buffer {};

		while (!input.eof()) {
			input >> buffer;
			m_input.push_back(buffer);
		}

		EXPECT(!input.fail(), "Invalid input. Please check input data.");
	}


// Operators
	InputData& operator=(const InputData&) = delete;
	InputData& operator=(InputData&&) = delete;


// Getter
	// --- getPermutations ---
	int getPermutations() const
	{
		return m_permutations;
	}


	// --- _getPermutationsPart2 ---
	int getPermutationsPart2()
	{
		return m_permutationsPart2;
	}


// Functions
	// --- permutate() ---
	void permutate(int freeVolume, const vecSize_t startPos = 0, int usedContainers = 0)
	{
		if ((startPos >= m_input.size()) || (freeVolume <= 0)) {
			return;
		}

		const vecSize_t new_startPos {startPos + 1};
		permutate(freeVolume, new_startPos, usedContainers); // do not use this container

		++usedContainers;
		freeVolume -= m_input[startPos];
		permutate(freeVolume, new_startPos, usedContainers);
		if (freeVolume == 0) {
			++m_permutations;
			m_minContainers = std::min(m_minContainers, usedContainers);
		}
	}


	// --- _getPermutationsPart2 ---
	void permutatePart2(int freeVolume, const vecSize_t startPos = 0, int usedContainers = 0)
	{
		if ((usedContainers >= m_minContainers) || (startPos >= m_input.size()) || (freeVolume <= 0)) {
			return;
		}

		const vecSize_t new_startPos = startPos + 1;
		permutatePart2(freeVolume, new_startPos, usedContainers); // do not use this container

		++usedContainers;
		freeVolume -= m_input[startPos];
		permutatePart2(freeVolume, new_startPos, usedContainers);
		if (freeVolume == 0) {
			++m_permutationsPart2;
		}

	}


private:
// Variables
	std::vector<int> m_input {};
	int m_permutations {0};
	int m_permutationsPart2 {0};
	int m_minContainers {std::numeric_limits<int>::max()};
};



} /* anonymous namespace */



//=== Class Day17 ============================================================
// --- Day17::solve() ---
void Day17::solve()
{
	try {
		std::ifstream inputFile {m_IO.getInputFile()};
		InputData inputData {inputFile};
		m_IO.printFileValid();

		inputData.permutate(eggnog_volume);
		m_IO.printSolution(inputData.getPermutations(), EPart::Part1);
		inputData.permutatePart2(eggnog_volume);
		m_IO.printSolution(inputData.getPermutationsPart2(), EPart::Part2);

	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
