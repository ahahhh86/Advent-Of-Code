// Day21.h
/* for puzzle see *.cpp */



//=== Preprocessor ===========================================================
#ifndef AOC2015_DAY21_H_
#define AOC2015_DAY21_H_



//=== Include ================================================================
#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"
#include "../aoc/Day00.h"



namespace aoc2015 {
//=== Class Day21 ============================================================
class Day21 final : public aoc::Day00 {
public:
// Inherited function
	void solve() override;

private:
// Variables
	aoc::BasicIO m_IO {aoc::EYears::Year2015, aoc::EDays::Day21};
};



} /* namespace aoc2015 */
#endif /* AOC2015_DAY21_H_ */
