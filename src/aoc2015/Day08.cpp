// https://adventofcode.com/2015/day/8
/* --- Day 8: Matchsticks ---
Space on the sleigh is limited this year, and so Santa will be bringing his list as a digital copy. He needs to know how
much space it will take up when stored.
It is common in many programming languages to provide a way to escape special characters in strings. For example, C,
JavaScript, Perl, Python, and even PHP handle special characters in very similar ways.
However, it is important to realize the difference between the number of characters in the code representation of the
string literal and the number of characters in the in-memory string itself.

For example:
	"" is 2 characters of code (the two double quotes), but the string contains zero characters.
	"abc" is 5 characters of code, but 3 characters in the string data.
	"aaa\"aaa" is 10 characters of code, but the string itself contains six "a" characters and a single, escaped quote
	character, for a total of 7 characters in the string data.
	"\x27" is 6 characters of code, but the string itself contains just one - an apostrophe ('), escaped using hexadecimal notation.

Santa's list is a file that contains many double-quoted string literals, one on each line. The only escape sequences used
are \\ (which represents a single backslash), \" (which represents a lone double-quote character), and \x plus two
hexadecimal characters (which represents a single character with that ASCII code).
Disregarding the whitespace in the file, what is the number of characters of code for string literals minus the number
of characters in memory for the values of the strings in total for the entire file?
For example, given the four strings above, the total number of characters of string code (2 + 5 + 10 + 6 = 23) minus the
total number of characters in memory for string values (0 + 3 + 7 + 1 = 11) is 23 - 11 = 12.
Your puzzle answer was 1333.

--- Part Two ---
Now, let's go the other way. In addition to finding the number of characters of code, you should now encode each code
representation as a new string and find the number of characters of the new encoded representation, including the
surrounding double quotes.
For example:
	"" encodes to "\"\"", an increase from 2 characters to 6.
	"abc" encodes to "\"abc\"", an increase from 5 characters to 9.
	"aaa\"aaa" encodes to "\"aaa\\\"aaa\"", an increase from 10 characters to 16.
	"\x27" encodes to "\"\\x27\"", an increase from 6 characters to 11.

Your task is to find the total number of characters to represent the newly encoded strings minus the number of characters
of code in each original string literal. For example, for the strings above, the total encoded length (6 + 9 + 16 + 11 = 42)
minus the characters in the original code representation (23, just like in the first part of this puzzle) is 42 - 23 = 19.
Your puzzle answer was 2046. */



//=== Include ================================================================
#include "Day08.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
using strVector_t = std::vector<std::string>;



//=== Functions ==============================================================
// --- sizeDifferencePart1() ---
auto sizeDifferencePart1(std::string_view input)
{
	input.remove_prefix(1);
	input.remove_suffix(1);
	constexpr int min_result {2};    // +2 from first and last "


	const auto countStr([&](std::string_view findStr) {
		strViewSize_t pos {0};
		int result {0};

		while (true) {
			pos = input.find(findStr, pos);
			if (pos == std::string_view::npos) {
				break;
			}
			++result;
			pos += findStr.length();    // pos after findStr
		};

		return result;
	});


	// Find \x##
	const auto countHex([&]() {
		constexpr int additional_length {3};

		strViewSize_t pos {0};
		int result {0};

		while (true) {
			pos = input.find("\\x", pos);
			if (pos == std::string_view::npos) {
				break;
			}
			pos += 2;    // pos after "\x"

			if ((pos + 1 < input.length()) && std::isxdigit(input[pos]) && std::isxdigit(input[pos + 1])) {
				pos += 2;    // pos after "\x##"
				result += additional_length;
			}
		};

		return result;
	});

	return min_result + countStr("\\\\") + countStr("\\\"") + countHex();
}



// --- sizeDifferencePart2() ---
auto sizeDifferencePart2(std::string_view input)
{
	input.remove_prefix(1);
	input.remove_suffix(1);
	constexpr int min_result {4}; // +4 from first and last "

	const auto countChr([&](const char findChr) {
		return std::count(input.cbegin(), input.cend(), findChr);
	});

	return min_result + countChr('\\') + countChr('\"');
}



// --- readStrVector() ---
strVector_t readStrVector(std::ifstream file)
{
	strVector_t result {};
	std::string buffer {};

	while (!file.eof()) {
		std::getline(file, buffer);
		result.push_back(buffer);
	}

	return result;
}



} /* anonymous namespace */



//=== Class Day08 ============================================================
// --- Day08::solve() ---
void Day08::solve()
{
	try {
		const strVector_t input {readStrVector(m_IO.getInputFile())};
		m_IO.printFileValid();

		const auto getSum = [&](const auto func) {
			return std::accumulate(std::next(input.cbegin()), input.cend(), func(*input.cbegin()), [&](auto& sum, const auto& data) {
				return sum + func(data);
			});
		};

		m_IO.printSolution(getSum(sizeDifferencePart1), EPart::Part1);
		m_IO.printSolution(getSum(sizeDifferencePart2), EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
