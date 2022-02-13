// https://adventofcode.com/2015/day/10
/* --- Day 10: Elves Look, Elves Say ---
Today, the Elves are playing a game called look-and-say. They take turns making sequences by reading aloud the previous
sequence and using that reading as the next sequence. For example, 211 is read as "one two, two ones", which becomes 1221 (1 2, 2 1s).
Look-and-say sequences are generated iteratively, using the previous value as input for the next step. For each step,
take the previous value, and replace each run of digits (like 111) with the number of digits (3) followed by the digit itself (1).

For example:
	1 becomes 11 (1 copy of digit 1).
	11 becomes 21 (2 copies of digit 1).
	21 becomes 1211 (one 2 followed by one 1).
	1211 becomes 111221 (one 1, one 2, and two 1s).
	111221 becomes 312211 (three 1s, two 2s, and one 1).

Starting with the digits in your puzzle input, apply this process 40 times. What is the length of the result?
Your puzzle answer was 252594.

--- Part Two ---
Neat, right? You might also enjoy hearing John Conway talking about this sequence (that's Conway of Conway's Game of Life fame).
Now, starting again with the digits in your puzzle input, apply this process 50 times. What is the length of the new result?
Your puzzle answer was 3579328. */



//=== Include ================================================================
#include "Day10.h"

#include <iterator>
#include <string>
#include <string_view>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Functions ==============================================================
// --- lookAndTell() ------------------
std::string lookAndTell(std::string_view input)
{
	const auto loop_end {input.cend()};
	const auto input_length {input.length()};
	std::string result {};
	result.reserve(input_length * 2);

	int count {1};
	char buffer {input[0]};

	for (auto i {std::next(input.cbegin())}; i != loop_end; std::advance(i, 1)) {
		if (*i == buffer) {
			++count;
		} else {
			result.append(std::to_string(count));
			result.push_back(buffer);

			count = 1;
			buffer = *i;
		}
	}
	result.append(std::to_string(count));
	result.push_back(buffer);

	return result;
}



} /* anonymous namespace */



//=== Class Day10 ============================================================
// --- Day10::solve() ---
void Day10::solve()
{
	try {
		const std::string input {m_IO.getInputString()};
		EXPECT(input.find_first_not_of("0123456789") == std::string::npos, "Invalid input only digits are allowed.");
		m_IO.printFileValid();


		std::string buffer {input};
		constexpr int loop_end_part1 {40};
		constexpr int loop_end_part2 {50};
		int i {0};

		for (; i < loop_end_part1; ++i) {
			buffer = lookAndTell(buffer);
		}
		m_IO.printSolution(buffer.length(), EPart::Part1);

		for (; i < loop_end_part2; ++i) {
			buffer = lookAndTell(buffer);
		}
		m_IO.printSolution(buffer.length(), EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
