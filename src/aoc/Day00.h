// Day00.h
/* basic interface class for all puzzles */



//=== Preprocessor ===========================================================
#ifndef AOC_DAY00_H_
#define AOC_DAY00_H_



//=== Include ================================================================
#include <memory>



namespace aoc {
//=== Class Day00 ============================================================
class Day00 {
public:
// Constructors / destructor
	Day00() = default;
	Day00(const Day00&) = delete;
	Day00(Day00&&) = delete;
	virtual ~Day00() noexcept = default;

// Operators
	Day00& operator=(const Day00&) = delete;
	Day00& operator=(Day00&&) = delete;

// Functions
	virtual void solve() = 0;
};
using pPuzzle = std::unique_ptr<Day00>;



} /* namespace aoc */
#endif /* AOC_DAY00_H_ */
