// https://adventofcode.com/2015/day/5
/* --- Day 5: Doesn't He Have Intern-Elves For This? ---
Santa needs help figuring out which strings in his text file are naughty or nice.
A nice string is one with all of the following properties:
	It contains at least three vowels (aeiou only), like aei, xazegov, or aeiouaeiouaeiou.
	It contains at least one letter that appears twice in a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
	It does not contain the strings ab, cd, pq, or xy, even if they are part of one of the other requirements.

For example:
	ugknbfddgicrmopn is nice because it has at least three vowels (u...i...o...), a double letter (...dd...), and none of the disallowed substrings.
	aaa is nice because it has at least three vowels and a double letter, even though the letters used by different rules overlap.
	jchzalrnumimnmhp is naughty because it has no double letter.
	haegwjzuvuyypxyu is naughty because it contains the string xy.
	dvszwmarrgswjxmb is naughty because it contains only one vowel.

How many strings are nice?
Your puzzle answer was 258.

--- Part Two ---
Realizing the error of his ways, Santa has switched to a better model of determining whether a string is naughty or nice.
None of the old rules apply, as they are all clearly ridiculous.
Now, a nice string is one with all of the following properties:
It contains a pair of any two letters that appears at least twice in the string without overlapping, like xyxy (xy)
or aabcdefgaa (aa), but not like aaa (aa, but it overlaps).
It contains at least one letter which repeats with exactly one letter between them, like xyx, abcdefeghi (efe), or even aaa.

For example:
	qjhvhtzxzqqjkmpb is nice because is has a pair that appears twice (qj) and a letter that repeats with exactly one letter between them (zxz).
	xxyxx is nice because it has a pair that appears twice and a letter that repeats with one between, even though the letters used by each rule overlap.
	uurcxstgmygtbstg is naughty because it has a pair (tg) but no repeat with a single letter between them.
	ieodomkazucvgmuy is naughty because it has a repeating letter with one between (odo), but no pair that appears twice.

How many strings are nice under these new rules?
Your puzzle answer was 53. */



//=== Include ================================================================
#include "Day05.h"

#include <bits/exception.h>
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Class SantasFile =======================================================
class SantasFile {
public:
// Constructors / destructor
	SantasFile() = delete;
	SantasFile(const SantasFile&) = delete;
	SantasFile(SantasFile&&) = delete;
	~SantasFile() = default;


	// --- SantasFile() ---
	explicit SantasFile(std::ifstream file)
	{
		std::string strBuffer {};
		while (!file.eof()) {
			file >> strBuffer;
			m_input.push_back(strBuffer);
		}
	}


// Operators
	SantasFile& operator=(const SantasFile&) = delete;
	SantasFile& operator=(SantasFile&&) = delete;


// Functions
	// --- countNaughtyPart1() ---
	auto countNaughtyPart1() const
	{
		// contains at least three vowels
		constexpr auto has3Vowels = [](std::string_view input) {
			constexpr int vowels_needed {3};
			int vowelCount {0};

			for (const char chr : input) {
				switch (chr) {
				case 'a':
				case 'e':
				case 'i':
				case 'o':
				case 'u':
					++vowelCount;
					if (vowelCount == vowels_needed) {
						return true;
					}
					break;

				default:
					// do nothing
					break;
				}
			}

			return false;
		};


		// contains at least one letter that appears twice in a row
		constexpr auto hasDoubleLetter = [=](std::string_view input) {
			const auto loop_end {input.cend()};

			for (auto i {input.cbegin()}, j {std::next(i)}; j != loop_end; std::advance(i, 1), std::advance(j, 1)) {
				if (*i == *j) {
					return true;
				}
			}

			return false;
		};


		// does not contain the strings ab, cd, pq, or xy
		constexpr auto hasNoNaughtyString = [=](std::string_view input) {
			const bool naughtyAb = (input.find("ab", 0) != std::string_view::npos);
			const bool naughtyCd = (input.find("cd", 0) != std::string_view::npos);
			const bool naughtyPq = (input.find("pq", 0) != std::string_view::npos);
			const bool naughtyXy = (input.find("xy", 0) != std::string_view::npos);

			return !(naughtyAb || naughtyCd || naughtyPq || naughtyXy);
		};


		// actual function
		return std::count_if(m_input.cbegin(), m_input.cend(), [&](const auto& str) {
			return has3Vowels(str) && hasDoubleLetter(str) && hasNoNaughtyString(str);
		});
	}


	// --- countNaughtyPart2() ---
	auto countNaughtyPart2() const
	{
		// contains a pair of any two letters that appears at least twice
		constexpr auto hasDoubleSequence = [](std::string_view input) {
			constexpr strViewSize_t sequence_length {2};
			const strViewSize_t loop_end {input.length() - 2 * sequence_length + 1};
			std::string_view buffer;

			for (strViewSize_t i {0}; i < loop_end; ++i) {
				buffer = input.substr(i, sequence_length);
				if (input.find(buffer, i + sequence_length) != std::string_view::npos) {
					return true;
				}
			}

			return false;
		};


		// contains at least one letter which repeats with exactly one letter between them
		constexpr auto hasDoubleLetter = [=](std::string_view input) {
			constexpr std::ptrdiff_t letter_distance {2};
			const auto loop_end {input.cend()};

			for (auto i {input.cbegin()}, j {std::next(i, letter_distance)}; j != loop_end; std::advance(i, 1), std::advance(j, 1)) {
				if (*i == *j) {
					return true;
				}
			}

			return false;
		};


		// actual function
		return std::count_if(m_input.begin(), m_input.end(), [&](const auto& str) {
			return hasDoubleSequence(str) && hasDoubleLetter(str);
		});
	}


private:
	// Variables
	std::vector<std::string> m_input {};		// Input data from file
};



} /* anonymous namespace */



//=== Class Day05 ============================================================
// --- Day05::solve() ---
void Day05::solve()
{
	try {
		SantasFile santasFile(m_IO.getInputFile());
		m_IO.printFileValid();

		m_IO.printSolution(santasFile.countNaughtyPart1(), EPart::Part1);
		m_IO.printSolution(santasFile.countNaughtyPart2(), EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
