// https://adventofcode.com/2015/day/11
/* --- Day 11: Corporate Policy ---
Santa's previous password expired, and he needs help choosing a new one.
To help him remember his new password after the old one expires, Santa has devised a method of coming up with a password
based on the previous one. Corporate policy dictates that passwords must be exactly eight lowercase letters (for security
reasons), so he finds his new password by incrementing his old password string repeatedly until it is valid.
Incrementing is just like counting with numbers: xx, xy, xz, ya, yb, and so on. Increase the rightmost letter one step;
if it was z, it wraps around to a, and repeat with the next letter to the left until one doesn't wrap around.
Unfortunately for Santa, a new Security-Elf recently started, and he has imposed some additional password requirements:
	Passwords must include one increasing straight of at least three letters, like abc, bcd, cde, and so on, up to xyz. They cannot skip letters; abd doesn't count.
	Passwords may not contain the letters i, o, or l, as these letters can be mistaken for other characters and are therefore confusing.
	Passwords must contain at least two different, non-overlapping pairs of letters, like aa, bb, or zz.

For example:
	hijklmmn meets the first requirement (because it contains the straight hij) but fails the second requirement requirement (because it contains i and l).
	abbceffg meets the third requirement (because it repeats bb and ff) but fails the first requirement.
	abbcegjk fails the third requirement, because it only has one double letter (bb).
	The next password after abcdefgh is abcdffaa.
	The next password after ghijklmn is ghjaabcc, because you eventually skip all the passwords that start with ghi..., since i is not allowed.

Given Santa's current password (your puzzle input), what should his next password be?
Your puzzle answer was vzbxxyzz.

--- Part Two ---
Santa's password expired again. What's the next one?
Your puzzle answer was vzcaabcc. */



//=== Include ================================================================
#include "Day11.h"

#include <algorithm>
#include <string>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Constants ==============================================================
constexpr strSize_t password_length {8};



//=== Class Password =========================================================
class Password {
public:
// Constructors / destructor
	Password() = delete;
	Password(const Password&) = delete;
	Password(Password&&) = delete;
	~Password() = default;


	// --- Password() ---
	explicit Password(std::string source) : m_data {source}
	{
		EXPECT(m_data.length() == password_length, invalid_input_file_data);

		const auto numInvalidChrs = std::count_if(m_data.cbegin(), m_data.cend(), [](const auto chr) {
			return !std::islower(chr);
		});
		EXPECT(numInvalidChrs == 0, invalid_input_file_data);
	}


// Operators
	Password& operator=(const Password&) = delete;
	Password& operator=(Password&&) = delete;


	// --- operator++() ---
	Password& operator++()
	{
		do {
			incPassword(password_length - 1);
		} while (!isValid());

		return *this;
	}


	// --- std::string() ---
	operator std::string() const
	{
		return m_data;
	}


private:
// Functions
	// --- incPassword() ---
	void incPassword(const strSize_t index)
	{
		EXPECT(index < password_length, "Index out of bounds.");

		switch (m_data[index]) {
		case 'h':
		case 'k':
		case 'n':	// i, o and l are not allowed
			m_data[index] += 2;
			break;

		case 'z':	// roll over
			m_data[index] = 'a';
			if (index > 0) {
				incPassword(index - 1);
			}
			break;

		default:
			++m_data[index];
			break;
		}
	}


	// --- isValid() ---
	bool isValid() const
	{
		const auto isStraight = [&]() {
			constexpr int straight_needed {3};

			char buffer {m_data[0]};
			int count {1};

			for (const auto chr : m_data) {
				count = (buffer + 1 == chr) ? count + 1 : 1;
				buffer = chr;
				if (count == straight_needed) {
					return true;
				}
			}

			return false;
		};


		const auto isDouble = [&]() {
			char buffer {m_data[0]};
			int count {0};
			strSize_t i {1};

			while (i < password_length) {
				if (buffer == m_data[i]) {
					++count;
					++i;
				}

				if (count > 1) {
					return true;
				}

				if (i < password_length) {
					buffer = m_data[i];
				}

				++i;
			}

			return false;
		};



		return isStraight() && isDouble();
	}


// Variables
	std::string m_data {""};
};



} /* anonymous namespace */



//=== Class Day11 ============================================================
// --- Day11::solve() ---
void Day11::solve()
{
	try {
		Password password {m_IO.getInputString()};
		m_IO.printFileValid();

		++password;
		m_IO.printSolution(static_cast<std::string>(password), EPart::Part1);
		++password;
		m_IO.printSolution(static_cast<std::string>(password), EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
