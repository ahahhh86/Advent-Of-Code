// https://adventofcode.com/2015/day/13
/* --- Day 13: Knights of the Dinner Table ---
In years past, the holiday feast with your family hasn't gone so well. Not everyone gets along! This year, you resolve,
will be different. You're going to find the optimal seating arrangement and avoid all those awkward conversations.
You start by writing up a list of everyone invited and the amount their happiness would increase or decrease if they were
to find themselves sitting next to each other person. You have a circular table that will be just big enough to fit
everyone comfortably, and so each person will have exactly two neighbors.
For example, suppose you have only four attendees planned, and you calculate their potential happiness as follows:
	Alice would gain 54 happiness units by sitting next to Bob.
	Alice would lose 79 happiness units by sitting next to Carol.
	Alice would lose 2 happiness units by sitting next to David.
	Bob would gain 83 happiness units by sitting next to Alice.
	Bob would lose 7 happiness units by sitting next to Carol.
	Bob would lose 63 happiness units by sitting next to David.
	Carol would lose 62 happiness units by sitting next to Alice.
	Carol would gain 60 happiness units by sitting next to Bob.
	Carol would gain 55 happiness units by sitting next to David.
	David would gain 46 happiness units by sitting next to Alice.
	David would lose 7 happiness units by sitting next to Bob.
	David would gain 41 happiness units by sitting next to Carol.

Then, if you seat Alice next to David, Alice would lose 2 happiness units (because David talks so much), but David would
gain 46 happiness units (because Alice is such a good listener), for a total change of 44.
If you continue around the table, you could then seat Bob next to Alice (Bob gains 83, Alice gains 54). Finally, seat
Carol, who sits next to Bob (Carol gains 60, Bob loses 7) and David (Carol gains 55, David gains 41). The arrangement
ooks like this:
		 +41 +46
	+55   David    -2
	Carol       Alice
	+60    Bob    +54
		 -7  +83

After trying every other seating arrangement in this hypothetical scenario, you find that this one is the most optimal,
with a total change in happiness of 330.
What is the total change in happiness for the optimal seating arrangement of the actual guest list?
Your puzzle answer was 733.

--- Part Two ---
In all the commotion, you realize that you forgot to seat yourself. At this point, you're pretty apathetic toward the
whole thing, and your happiness wouldn't really go up or down regardless of who you sit next to. You assume everyone else
would be just as ambivalent about sitting next to you, too.
So, add yourself to the list, and give all happiness relationships that involve you a score of 0.
What is the total change in happiness for the optimal seating arrangement that actually includes yourself?
Your puzzle answer was 725. */



//=== Include ================================================================
#include "Day13.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"
#include "../aoc/TemplateContainer.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
using happiness_t = int;
using guestPair_t = std::pair<std::string, std::string>;
using locationListData_t = std::list<std::string>;

using happinessMap_t = DoubleKeyMap<std::string, happiness_t>;
using guestList_t = UniqueList<std::string>;



//=== Types ==================================================================
struct SInputData {
	guestPair_t guestPair {std::make_pair("", "")};
	happiness_t happiness {0};
};



struct SMapAndList {// happinessMap and guestList are created together
	happinessMap_t map {};
	guestList_t list {};
};



//=== Functions ==============================================================
// --- readInputData() ---
SInputData readInputData(std::ifstream& file)
{
	SInputData result {};
	std::string buffer {};
	file >> result.guestPair.first;
	constexpr auto would_width {std::string_view("would ").length()};
	file.ignore(would_width);
	file >> buffer;

	file >> result.happiness;
	EXPECT(!file.fail(), invalid_input_file_data);

	if (buffer == "lose") {
		result.happiness *= -1;
	} else if (buffer != "gain") {
		THROW_ERROR(invalid_input_file_data);
	}

	constexpr auto skip_width {std::string_view("happiness units by sitting next to ").length()};
	file.ignore(skip_width);
	file >> result.guestPair.second;
	result.guestPair.second.pop_back(); // remove dot

	return result;
}



// --- readMapAndList() ---
SMapAndList readMapAndList(std::ifstream file)
{
	SMapAndList result {};

	while (!file.eof()) {
		SInputData buffer {readInputData(file)};
		result.map.insert(buffer.guestPair, buffer.happiness);
		result.list.addData(buffer.guestPair.first);
		result.list.addData(buffer.guestPair.second);
	}

	return result;
}



// --- getHappiness() ---
auto getHappiness(const SMapAndList& data)
{
	const auto loop_end {std::prev(data.list.cend())};
	happiness_t result {data.map.find(std::make_pair(*data.list.cbegin(), *loop_end)) + data.map.find(
			std::make_pair(*loop_end, *data.list.cbegin()))};

	for (auto i {data.list.cbegin()}, j {i}; i != loop_end; i = j) {
		j = std::next(i);
		result += data.map.find(std::make_pair(*i, *j));
		result += data.map.find(std::make_pair(*j, *i));
	};

	return result;
}



} /* anonymous namespace */



//=== Class Day13 ============================================================
// --- Day13::solve() ---
void Day13::solve()
{
	try {
		SMapAndList input {readMapAndList(m_IO.getInputFile())};
		m_IO.printFileValid();

		const auto calculateHappiness = [&]() {
			happiness_t maxHappiness {std::numeric_limits<happiness_t>::min()};
			do {
				const auto happiness {getHappiness(input)};
				maxHappiness = std::max(maxHappiness, happiness);
			} while (input.list.nextPermutation(1));   // at a circular table it should not matter where we start counting
			return maxHappiness;
		};

		m_IO.printSolution(calculateHappiness(), EPart::Part1);


		const std::string me {"me"};
		std::for_each(input.list.cbegin(), input.list.cend(), [&](const auto& data) {
			input.map.insert(std::make_pair(me, data), 0);
		});
		input.list.addData(me);

		m_IO.printSolution(calculateHappiness(), EPart::Part2);

	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
