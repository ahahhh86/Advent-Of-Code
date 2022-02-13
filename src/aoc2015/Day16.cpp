// https://adventofcode.com/2015/day/16
/*--- Day 16: Aunt Sue ---
Your Aunt Sue has given you a wonderful gift, and you'd like to send her a thank you card. However, there's a small
problem: she signed it "From, Aunt Sue".
You have 500 Aunts named "Sue".
So, to avoid sending the card to the wrong person, you need to figure out which Aunt Sue (which you conveniently number
1 to 500, for sanity) gave you the gift. You open the present and, as luck would have it, good ol' Aunt Sue got you a
My First Crime Scene Analysis Machine! Just what you wanted. Or needed, as the case may be.
The My First Crime Scene Analysis Machine (MFCSAM for short) can detect a few specific compounds in a given sample, as
well as how many distinct kinds of those compounds there are. According to the instructions, these are what the MFCSAM
can detect:

	children, by human DNA age analysis.
	cats. It doesn't differentiate individual breeds.
	Several seemingly random breeds of dog: samoyeds, pomeranians, akitas, and vizslas.
	goldfish. No other kinds of fish.
	trees, all in one group.
	cars, presumably by exhaust or gasoline or something.
	perfumes, which is handy, since many of your Aunts Sue wear a few kinds.

In fact, many of your Aunts Sue have many of these. You put the wrapping from the gift into the MFCSAM. It beeps
inquisitively at you a few times and then prints out a message on ticker tape:

	children: 3
	cats: 7
	samoyeds: 2
	pomeranians: 3
	akitas: 0
	vizslas: 0
	goldfish: 5
	trees: 3
	cars: 2
	perfumes: 1

You make a list of the things you can remember about each Aunt Sue. Things missing from your list aren't zero - you
simply don't remember the value.
What is the number of the Sue that got you the gift?
Your puzzle answer was 103.

--- Part Two ---
As you're about to send the thank you note, something in the MFCSAM's instructions catches your eye. Apparently, it has
an outdated retroencabulator, and so the output from the machine isn't exact values - some of them indicate ranges.
In particular, the cats and trees readings indicates that there are greater than that many (due to the unpredictable
nuclear decay of cat dander and tree pollen), while the pomeranians and goldfish readings indicate that there are fewer
than that many (due to the modial interaction of magnetoreluctance).
What is the number of the real Aunt Sue?
Your puzzle answer was 405.*/



//=== Include ================================================================
#include "Day16.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
struct IsAuntNum {
	int number {-1};
	bool found {true};
};



//=== Class Mfcsam ===========================================================
class Mfcsam {
public:
// Constructors / destructor
	Mfcsam() = delete;
	Mfcsam(const Mfcsam&) = delete;
	Mfcsam(Mfcsam&&) = delete;
	~Mfcsam() = default;


	// --- Mfcsam() ---
	explicit Mfcsam(std::ifstream input)
	{
		m_tickerTape.insert(std::make_pair("children", 3));		// @suppress("Avoid magic numbers")
		m_tickerTape.insert(std::make_pair("cats", 7));			// @suppress("Avoid magic numbers")
		m_tickerTape.insert(std::make_pair("samoyeds", 2));		// @suppress("Avoid magic numbers")
		m_tickerTape.insert(std::make_pair("pomeranians", 3));	// @suppress("Avoid magic numbers")
		m_tickerTape.insert(std::make_pair("akitas", 0));		// @suppress("Avoid magic numbers")
		m_tickerTape.insert(std::make_pair("vizslas", 0));		// @suppress("Avoid magic numbers")
		m_tickerTape.insert(std::make_pair("goldfish", 5));		// @suppress("Avoid magic numbers")
		m_tickerTape.insert(std::make_pair("trees", 3));		// @suppress("Avoid magic numbers")
		m_tickerTape.insert(std::make_pair("cars", 2));			// @suppress("Avoid magic numbers")
		m_tickerTape.insert(std::make_pair("perfumes", 1));		// @suppress("Avoid magic numbers")

		std::string buffer {""};
		while (!input.eof()) {		// TODO: m_input as vector of class
			std::getline(input, buffer);
			m_input.push_back(buffer);
		}
	}


// Operators
	Mfcsam& operator=(const Mfcsam&) = delete;
	Mfcsam& operator=(Mfcsam&&) = delete;


// Functions
	// --- getAuntPart1() ---
	int getAuntPart1()
	{
		IsAuntNum result {};
		for (const auto& data : m_input) {
			result = isAuntPart1(data);
			if (result.found) {
				return result.number;
			}
		}

		return -1;
	}


	// --- getAuntPart2() ---
	int getAuntPart2()
	{
		IsAuntNum result {};
		for (const auto& data : m_input) {
			result = isAuntPart2(data);
			if (result.found) {
				return result.number;
			}
		}

		return -1;
	}


private:
// Functions
	// --- isAuntPart1() ---
	IsAuntNum isAuntPart1(const std::string& input) const
	{
		std::stringstream strstream {input};
		std::string strBuffer {};
		IsAuntNum result {};

		strstream >> strBuffer;
		EXPECT(strBuffer == "Sue", "Invalid input. Each line should start with 'Sue '.");

		strstream >> result.number;
		EXPECT(!strstream.fail(), "Invalid input. Aunt is missing her number");

		constexpr auto separator_width {std::string_view(": ").length()}; // ': ' or ', '

		auto compareCompound = [&]() {
			int intBuffer {};

			strstream.ignore(separator_width);
			strstream >> strBuffer;
			strBuffer.pop_back(); // removes ':'
			strstream >> intBuffer;
			EXPECT(!strstream.fail(), "Invalid input. Could not read compound.");

			return m_tickerTape.at(strBuffer) == intBuffer;
		};

		while (!strstream.eof()) {
			if (!compareCompound()) {
				result.found = false;
				return result;
			}
		}

		return result;
	}


	// --- isAuntPart2() ---
	IsAuntNum isAuntPart2(const std::string& input) const
	{
		std::stringstream strstream {input};
		std::string strBuffer {""};
		int intBuffer {};
		IsAuntNum result {};

		strstream >> strBuffer;
		EXPECT(strBuffer == "Sue", "Invalid input. Each line should start with 'Sue '.");

		strstream >> result.number;
		EXPECT(!strstream.fail(), "Invalid input. Aunt is missing her number");

		constexpr auto separator_width {std::string_view(": ").length()}; // ': ' or ', '

		auto compareCompound = [&]() {
			strstream.ignore(separator_width);
			strstream >> strBuffer;
			strBuffer.pop_back(); // removes ':'
			strstream >> intBuffer;
			EXPECT(!strstream.fail(), "Invalid input. Could not read compound.");

			if (strBuffer == "cats" || strBuffer == "trees" ) {
				return m_tickerTape.at(strBuffer) < intBuffer;
			} else if (strBuffer == "pomeranians" || strBuffer == "goldfish") {
				return m_tickerTape.at(strBuffer) > intBuffer;
			} else {
				return m_tickerTape.at(strBuffer) == intBuffer;
			}
		};

		while (!strstream.eof()) {
			if (!compareCompound()) {
				result.found = false;
				return result;
			}
		}

		return result;
	}


// Variables
	std::vector<std::string> m_input {};
	std::map<std::string, int> m_tickerTape {};
};



} /* anonymous namespace */



//=== Class Day16 ============================================================
// --- Day16::solve() ---
void Day16::solve()
{
	try {
		Mfcsam mfcsam {m_IO.getInputFile()};
		m_IO.printFileValid();

		m_IO.printSolution(mfcsam.getAuntPart1(), EPart::Part1);
		m_IO.printSolution(mfcsam.getAuntPart2(), EPart::Part2);

	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
