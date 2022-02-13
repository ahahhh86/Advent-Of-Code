// https://adventofcode.com/2015/day/12
/* --- Day 12: JSAbacusFramework.io ---
Santa's Accounting-Elves need help balancing the books after a recent order. Unfortunately, their accounting software
uses a peculiar storage format. That's where you come in.
They have a JSON document which contains a variety of things: arrays ([1,2,3]), objects ({"a":1, "b":2}), numbers, and
strings. Your first job is to simply find all of the numbers throughout the document and add them together.

For example:
	[1,2,3] and {"a":2,"b":4} both have a sum of 6.
	[[[3]]] and {"a":{"b":4},"c":-1} both have a sum of 3.
	{"a":[-1,1]} and [-1,{"a":1}] both have a sum of 0.
	[] and {} both have a sum of 0.

You will not encounter any strings containing numbers.
What is the sum of all numbers in the document?
Your puzzle answer was 156366.

--- Part Two ---
Uh oh - the Accounting-Elves have realized that they double-counted everything red.
Ignore any object (and all of its children) which has any property with the value "red". Do this only for objects ({...}), not arrays ([...]).

	[1,2,3] still has a sum of 6.
	[1,{"c":"red","b":2},3] now has a sum of 4, because the middle object is ignored.
	{"d":"red","e":[1,2,3,4],"f":5} now has a sum of 0, because the entire structure is ignored.
	[1,"red",5] has a sum of 6, because "red" in an array has no effect.

Your puzzle answer was 96852. */



//=== Include ================================================================
#include "Day12.h"

#include <string>
#include <string_view>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Functions ==============================================================
// --- countNumbers() ------------------
int countNumbers(std::string_view input)
{
	const auto loop_end(input.cend());
	int result {0};

	// Warning: i may advance inside of the loop
	// Assuming input does not end with a number, should end with closed brace
	for (auto i {input.cbegin()}, j {i}; i != loop_end; std::advance(i, 1)) {
		if (std::isdigit(*i) || *i == '-') {
			j = std::next(i);
			while (std::isdigit(*j)) {
				std::advance(j, 1);
			}
			result += std::stoi(std::string(i, j));
			i = j;
		}
	}

	return result;
}



// --- findOpenBrace() ------------------
auto findOpenBrace(std::string_view input, strViewSize_t index)
{
	strViewSize_t pos_open_brace {};
	strViewSize_t pos_close_brace {};
	int count {1};

	do {
		pos_open_brace = input.find_last_of('{', index);
		pos_close_brace = input.find_last_of('}', index);

		if ((pos_open_brace < pos_close_brace) && (pos_close_brace != std::string_view::npos)) {
			++count;
			index = pos_close_brace - 1;
		} else {
			--count;
			index = pos_open_brace - 1;
		}
	} while (count > 0);

	return pos_open_brace;
}



// --- Day12::findCloseBrace() ------------------
auto findCloseBrace(std::string_view input, strViewSize_t index)
{
	strViewSize_t pos_open_brace {};
	strViewSize_t pos_close_brace {};
	int count {1};

	do {
		pos_open_brace = input.find('{', index);
		pos_close_brace = input.find('}', index);

		if ((pos_open_brace < pos_close_brace) && (pos_open_brace != std::string_view::npos)) {
			++count;
			index = pos_open_brace + 1;
		} else {
			--count;
			index = pos_close_brace + 1;
		}
	} while (count > 0);

	return pos_close_brace;
}



// --- removeRed() ------------------
std::string removeRed(std::string_view input)
{
	constexpr std::string_view val_red {":\"red\""};

	std::string result {input};
	auto pos_red {input.find(val_red)};
	strViewSize_t pos_open_brace {};
	strViewSize_t pos_close_brace {};

	while (pos_red != std::string_view::npos) {
		pos_open_brace = findOpenBrace(result, pos_red);
		pos_close_brace = findCloseBrace(result, pos_red);
		result = result.erase(pos_open_brace, pos_close_brace - pos_open_brace + 1);

		pos_red = result.find(val_red);
	}

	return result;
}



}/* anonymous namespace */



//=== Class Day12 ============================================================
// --- Day12::solve() ---
void Day12::solve()
{
	try {
		const std::string input {m_IO.getInputString()};
		m_IO.printFileValid();

		m_IO.printSolution(countNumbers(input), EPart::Part1);
		m_IO.printSolution(countNumbers(removeRed(input)), EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



}/* namespace aoc2015 */
