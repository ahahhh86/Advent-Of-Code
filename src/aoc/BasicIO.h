// BasicIo.h
/* Class to get input data and to write the result */

//=== Preprocessor ===========================================================
#ifndef AOC_BASICIO_H_
#define AOC_BASICIO_H_



//=== Include ================================================================
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>

#include "BasicDefinitions.h"
#include "Timer.h"



namespace aoc {
//=== Class BasicIO ============================================================
class BasicIO final {
public:
// Constructors / destructor
	BasicIO() = delete;
	BasicIO(const BasicIO&) = delete;
	BasicIO(BasicIO&&) = delete;
	~BasicIO() = default;

	BasicIO(const EYears year, const EDays day);


// Operators
	BasicIO& operator=(const BasicIO&) = delete;
	BasicIO& operator=(BasicIO&&) = delete;


// Functions
	std::ifstream getInputFile() const;
	std::string getInputString() const;		// only reads to the first whitespace
	void printFileValid() const;
	template<typename T> void printSolution(const T solution, const EPart part) const;
	void printError(std::string_view error) const;


private:
// Functions
	void timeStamp() const;


// Variables
	Timer m_timer {};		// To calculate timing of the code
	const EYears m_year;	// Which puzzle is solved?
	const EDays m_day;		// Which puzzle is solved?
};



// --- BasicIO::solution() ---
template<typename T>
void BasicIO::printSolution(const T solution, const EPart part) const
{
	constexpr int solution_width {16};

	std::cout << "\tPart " << static_cast<int>(part) << " solution: " << std::setw(solution_width) << solution;
	timeStamp();
}



} /* namespace aoc */
#endif /* AOC_BASICIO_H_ */
