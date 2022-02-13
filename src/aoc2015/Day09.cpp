// https://adventofcode.com/2015/day/9
/* --- Day 9: All in a Single Night ---
Every year, Santa manages to deliver all of his presents in a single night.
This year, however, he has some new locations to visit; his elves have provided him the distances between every pair of
locations. He can start and end at any two (different) locations he wants, but he must visit each location exactly once.
What is the shortest distance he can travel to achieve this?

For example, given the following distances:
	London to Dublin = 464
	London to Belfast = 518
	Dublin to Belfast = 141

The possible routes are therefore:
	Dublin -> London -> Belfast = 982
	London -> Dublin -> Belfast = 605
	London -> Belfast -> Dublin = 659
	Dublin -> Belfast -> London = 659
	Belfast -> Dublin -> London = 605
	Belfast -> London -> Dublin = 982

The shortest of these is London -> Dublin -> Belfast = 605, and so the answer is 605 in this example.
What is the distance of the shortest route?
Your puzzle answer was 117.

--- Part Two ---
The next year, just to show off, Santa decides to take the route with the longest distance instead.
He can still start and end at any two (different) locations he wants, and he still must visit each location exactly once.
For example, given the distances above, the longest route would be 982 via (for example) Dublin -> London -> Belfast.
What is the distance of the longest route?
Your puzzle answer was 909. */



//=== Include ================================================================
#include "Day09.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <string>
#include <utility>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"
#include "../aoc/TemplateContainer.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
using distance_t = unsigned int;
using locationPair_t = std::pair<std::string, std::string>;
using locationListData_t = std::list<std::string>;

using distanceMap_t = DoubleKeyMap<std::string, distance_t>;
using locationList_t = UniqueList<std::string>;



struct SInputData {
	locationPair_t locationPair {std::make_pair("", "")};
	distance_t distance {0};
};



struct SMapAndList {// distanceMap and locationList are created together
	distanceMap_t map {};
	locationList_t list {};
};



//=== Functions ==============================================================
// --- readInputData() ---
SInputData readInputData(std::ifstream& file)
{
	SInputData result {};

	file >> result.locationPair.first;
	constexpr auto to_width {std::string_view("to ").length()};
	file.ignore(to_width);
	file >> result.locationPair.second;
	constexpr auto equals_width {std::string_view("= ").length()};
	file.ignore(equals_width);
	file >> result.distance;

	EXPECT(!file.fail(), invalid_input_file_data);
	return result;
}



// --- readMapAndList() ---
SMapAndList readMapAndList(std::ifstream file)
{
	SMapAndList result {};

	while (!file.eof()) {
		SInputData buffer {readInputData(file)};
		result.map.insert(buffer.locationPair, buffer.distance);
		result.list.addData(buffer.locationPair.first);
		result.list.addData(buffer.locationPair.second);
	}

	return result;
}



// --- getTravelDistance() ---
auto getTravelDistance(const SMapAndList& data)
{
	const auto loop_end {data.list.cend()};
	distance_t result {0};

	for (auto i {data.list.cbegin()}, j {std::next(i)}; j != loop_end; std::advance(i, 1), std::advance(j, 1)) {
		result += data.map.find(std::make_pair(*i, *j));
	};

	return result;
}



} /* anonymous namespace */



//=== Class Day09 ============================================================
// --- Day09::solve() ---
void Day09::solve()
{
	try {
		SMapAndList input {readMapAndList(m_IO.getInputFile())};
		m_IO.printFileValid();

		distance_t minDistance {std::numeric_limits<distance_t>::max()};
		distance_t maxDistance {std::numeric_limits<distance_t>::min()};
		do {
			const auto travelDistance {getTravelDistance(input)};
			minDistance = std::min(minDistance, travelDistance);
			maxDistance = std::max(maxDistance, travelDistance);
		} while (input.list.nextPermutation());

		m_IO.printSolution(minDistance, EPart::Part1);
		m_IO.printSolution(maxDistance, EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
