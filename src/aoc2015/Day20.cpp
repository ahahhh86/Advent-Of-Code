// https://adventofcode.com/2015/day/20
/*--- Day 20: Infinite Elves and Infinite Houses ---
To keep the Elves busy, Santa has them deliver some presents by hand, door-to-door. He sends them down a street with
infinite houses numbered sequentially: 1, 2, 3, 4, 5, and so on.
Each Elf is assigned a number, too, and delivers presents to houses based on that number:
	The first Elf (number 1) delivers presents to every house: 1, 2, 3, 4, 5, ....
	The second Elf (number 2) delivers presents to every second house: 2, 4, 6, 8, 10, ....
	Elf number 3 delivers presents to every third house: 3, 6, 9, 12, 15, ....

There are infinitely many Elves, numbered starting with 1. Each Elf delivers presents equal to ten times his or her
number at each house.
So, the first nine houses on the street end up like this:
	House 1 got 10 presents.
	House 2 got 30 presents.
	House 3 got 40 presents.
	House 4 got 70 presents.
	House 5 got 60 presents.
	House 6 got 120 presents.
	House 7 got 80 presents.
	House 8 got 150 presents.
	House 9 got 130 presents.

The first house gets 10 presents: it is visited only by Elf 1, which delivers 1 * 10 = 10 presents. The fourth house
gets 70 presents, because it is visited by Elves 1, 2, and 4, for a total of 10 + 20 + 40 = 70 presents.
What is the lowest house number of the house to get at least as many presents as the number in your puzzle input?
Your puzzle answer was 665280.

--- Part Two ---
The Elves decide they don't want to visit an infinite number of houses. Instead, each Elf will stop after delivering
presents to 50 houses. To make up for it, they decide to deliver presents equal to eleven times their number at each house.
With these changes, what is the new lowest house number of the house to get at least as many presents as the number
in your puzzle input?
Your puzzle answer was 705600.*/



//=== Include ================================================================
#include "Day20.h"

#include <fstream>
#include <iostream>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
using uint_t = unsigned int;



//=== Functions ==============================================================
// --- calculatePresents() ---
// WARNING: takes (on my computer) about 10 min. to find the right solution
// CHECK: How to make faster
uint_t calculatePresents(unsigned int houseIndex)
{
	constexpr uint_t multiplicator {10};		// to save some operations, just do this once at the end
	const uint_t loop_end {houseIndex / 2};		// elves between houseIndex/2 and houseIndex will not stop here
	uint_t result = 1 + houseIndex;				// elves 1 and houseIndex will always stop here

	for (uint_t i {2}; i <= loop_end; ++i) {
		if (houseIndex % i == 0) {
			result += i;
		}
	}

	return result * multiplicator;
}



// --- calculatePresentsPart2() ---
uint_t calculatePresentsPart2(unsigned int houseIndex)
{
	constexpr uint_t max_presents_per_elf {50};
	constexpr uint_t multiplicator {11};						// to save some operations, just do this once at the end
	const uint_t loop_end {houseIndex / max_presents_per_elf};
	uint_t result = houseIndex;									// Assume houseIndex > 50, skip first house

	for (uint_t i {houseIndex / 2}; i >= loop_end; --i) {
		if (houseIndex % i == 0) {
			result += i;
		}
	}

	return result * multiplicator;
}



} /* anonymous namespace */



//=== Class Day20 ============================================================
// --- Day20::solve() ---
void Day20::solve()
{
	try {
		uint_t input {};
		m_IO.getInputFile() >> input;
		EXPECT(input != 0, "Invalid input. Please check input file.");
		m_IO.printFileValid();

		uint_t houseIndex {2};
		//uint_t houseIndex {665270}; // get headstart for debugging
		while (calculatePresents(houseIndex) <= input) {
			++houseIndex;
		}
		m_IO.printSolution(houseIndex, EPart::Part1);

		while (calculatePresentsPart2(houseIndex) <= input) {
			++houseIndex;
		}
		m_IO.printSolution(houseIndex, EPart::Part2);

	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
