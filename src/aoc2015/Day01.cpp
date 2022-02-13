// https://adventofcode.com/2015/day/1
/* --- Day 1: Not Quite Lisp ---
Santa was hoping for a white Christmas, but his weather machine's "snow" function is powered by stars, and he's fresh out!
To save Christmas, he needs you to collect fifty stars by December 25th.
Collect stars by helping Santa solve puzzles. Two puzzles will be made available on each day in the Advent calendar;
the second puzzle is unlocked when you complete the first. Each puzzle grants one star. Good luck!
Here's an easy puzzle to warm you up.
Santa is trying to deliver presents in a large apartment building, but he can't find the right floor - the directions he
got are a little confusing. He starts on the ground floor (floor 0) and then follows the instructions one character at a time.
An opening parenthesis, (, means he should go up one floor, and a closing parenthesis, ), means he should go down one floor.
The apartment building is very tall, and the basement is very deep; he will never find the top or bottom floors.

For example:
	(()) and ()() both result in floor 0.
	((( and (()(()( both result in floor 3.
	))((((( also results in floor 3.
	()) and ))( both result in floor -1 (the first basement level).
	))) and )())()) both result in floor -3.

To what floor do the instructions take Santa?
Your puzzle answer was 280.

--- Part Two ---
Now, given the same instructions, find the position of the first character that causes him to enter the basement
(floor -1). The first character in the instructions has position 1, the second character has position 2, and so on.

For example:
	) causes him to enter the basement at character position 1.
	()()) causes him to enter the basement at character position 5.

What is the position of the character that causes Santa to first enter the basement?
Your puzzle answer was 1797. */



//=== Include ================================================================
#include "Day01.h"

#include <algorithm>
#include <limits>
#include <string>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Constants ==============================================================
constexpr int basement_level {-1};
constexpr strSize_t invalid_level {std::numeric_limits<strSize_t>::max()};



//=== Class Elevator =========================================================
class Elevator {
public:
// Constructors / destructor
	Elevator() = delete;
	Elevator(const Elevator&) = delete;
	Elevator(Elevator&&) = delete;
	~Elevator() = default;


	// --- Elevator() ---
	explicit Elevator(std::string input) : m_input {input}
	{
		EXPECT(m_input.find_first_not_of("()") == std::string::npos, invalid_input_file_data);
	}


// Operators
	Elevator& operator=(const Elevator&) = delete;
	Elevator& operator=(Elevator&&) = delete;


// Functions
	// --- changeFloors() ---
	void changeFloors()
	{
		const strSize_t loop_end {m_input.length()};

		for (strSize_t i {0}; i < loop_end; ++i) {
			changeFloorOnce(m_input[i]);

			if ((m_atBasement == invalid_level) && (m_level == basement_level)) {
				m_atBasement = i + 1;	// String is counted starting at '1', so add one to adjust
			}
		}
	}


// --- getAtBasement() ---
	auto getAtBasement() const
	{
		return m_atBasement;
	}


// --- getLevel() ---
	auto getLevel() const
	{
		return m_level;
	}


private:
// Functions
	// --- changeFloorOnce() ---
	void changeFloorOnce(const char input)
	{
		switch (input) {
		case '(':
			++m_level;
			break;

		case ')':
			--m_level;
			break;

		default:
			THROW_ERROR(invalid_input_file_data); // Should already been handled at constructor, but switch should have a default
		}
	}


// Variables
	const std::string m_input;					// Input data from file
	int m_level {0};							// Current level of the elevator
	strSize_t m_atBasement {invalid_level};		// First time the elevator stops at the basement
};



} /* anonymous namespace */



//=== Class Day01 ============================================================
// --- Day01::solve() ---
void Day01::solve()
{
	try {
		Elevator elevator {m_IO.getInputString()};
		m_IO.printFileValid();


		elevator.changeFloors();
		m_IO.printSolution(elevator.getLevel(), EPart::Part1);
		m_IO.printSolution(elevator.getAtBasement(), EPart::Part2);


	} catch (const std::exception& e) {
		m_IO.printError(e.what());
	}
}



} /* namespace aoc2015 */
