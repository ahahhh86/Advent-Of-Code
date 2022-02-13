// https://adventofcode.com/2015/day/4
/* --- Day 4: The Ideal Stocking Stuffer ---
Santa needs help mining some AdventCoins (very similar to bitcoins) to use as gifts for all the economically
forward-thinking little girls and boys.
To do this, he needs to find MD5 hashes which, in hexadecimal, start with at least five zeroes. The input to the MD5
hash is some secret key (your puzzle input, given below) followed by a number in decimal. To mine AdventCoins, you must
find Santa the lowest positive number (no leading zeroes: 1, 2, 3, ...) that produces such a hash.

For example:
	If your secret key is abcdef, the answer is 609043, because the MD5 hash of abcdef609043 starts with five zeroes
	(000001dbbfa...), and it is the lowest such number to do so.

If your secret key is pqrstuv, the lowest number it combines with to make an MD5 hash starting with five zeroes is
1048970; that is, the MD5 hash of pqrstuv1048970 looks like 000006136ef....

Your puzzle answer was 254575.

--- Part Two ---
Now find one that starts with six zeroes.
Your puzzle answer was 1038736. */



//=== Include ================================================================
#include "Day04.h"

#include <limits>
#include <string>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"
#include "../aoc/md5.h"



namespace aoc2015 {
using namespace aoc;



//=== Class Day04 ============================================================
// --- Day04::solve() ---
void Day04::solve()
{
	try {
		constexpr auto max_loop {std::numeric_limits<unsigned int>::max()};
		constexpr std::string_view zeroes_part1 {"00000"};
		constexpr std::string_view zeroes_part2 {"000000"};
		const std::string secretKey {m_IO.getInputString()};
		m_IO.printFileValid();


		unsigned int i {0};
		std::string hash {};
		// CHECK: if C++20 use starts_with() instead of compare() == 0
		const auto calculateZeroes = [&](std::string_view zeroes){
			for (; i < max_loop; ++i) {
				hash = md5(secretKey + std::to_string(i));
				if (hash.compare(0, zeroes.length(), zeroes) == 0) {
					break;
				}
			}
		};


		calculateZeroes(zeroes_part1);
		m_IO.printSolution(i, EPart::Part1);

		// Do not reset i, if there were not at least five zeroes, there were not six
		calculateZeroes(zeroes_part2);
		m_IO.printSolution(i, EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
