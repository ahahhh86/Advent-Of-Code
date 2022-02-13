// Aoc2015.cpp

//=== Include ================================================================
#include "Aoc2015.h"

#include <memory>
#include <stdexcept>

#include "Day01.h"
#include "Day02.h"
#include "Day03.h"
#include "Day04.h"
#include "Day05.h"
#include "Day06.h"
#include "Day07.h"
#include "Day08.h"
#include "Day09.h"
#include "Day10.h"
#include "Day11.h"
#include "Day12.h"
#include "Day13.h"
#include "Day14.h"
#include "Day15.h"
#include "Day16.h"
#include "Day17.h"
#include "Day18.h"
#include "Day19.h"
#include "Day20.h"
#include "Day21.h"
#include "Day23.h"

#include "../aoc/BasicDefinitions.h"
#include "../aoc/Day00.h"



namespace aoc2015 {
using namespace aoc;
namespace {


//=== Function ===============================================================
// defining an 'evil' macro, to shorten text
#define CASE_DAY(Num) case EDays::Day##Num: return std::make_unique<Day##Num>(); break

// --- getPuzzle() ---
pPuzzle getPuzzle(const EDays day)
{
	switch (day) {
		CASE_DAY(01);
		CASE_DAY(02);
		CASE_DAY(03);
		CASE_DAY(04);
		CASE_DAY(05);
		CASE_DAY(06);
		CASE_DAY(07);
		CASE_DAY(08);
		CASE_DAY(09);
		CASE_DAY(10);
		CASE_DAY(11);
		CASE_DAY(12);
		CASE_DAY(13);
		CASE_DAY(14);
		CASE_DAY(15);
		CASE_DAY(16);
		CASE_DAY(17);
		CASE_DAY(18);
		CASE_DAY(19);
		CASE_DAY(20);
		CASE_DAY(21);
		CASE_DAY(23);

		default:
			THROW_ERROR("This puzzle was not (yet) solved.");
	}

	return NULL; // not reached, but removes error message from eclipse
}
// do not use 'evil' macro anymore
#undef CASE_DAY



} /* anonymous namespace */



// --- solvePuzzle() ---
void solvePuzzle2015(const EDays day)
{
	getPuzzle(day)->solve();
}



} /* namespace aoc2015 */
