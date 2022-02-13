// Aoc.cpp
/* main function to select a puzzle to solve */



//=== Include ================================================================
#include "Aoc.h"

#include <iostream>
#include <stdexcept>

#include "BasicDefinitions.h"

#include "../aoc2015/Aoc2015.h"



namespace aoc {
//=== Function ===============================================================
void solvePuzzle(const aoc::EYears year, const aoc::EDays day)
{
	try {
		switch (year) {
		case EYears::Year2015:
			aoc2015::solvePuzzle2015(day);
			break;

		default:
			throw std::runtime_error("This year was not (yet) solved.");
		}

	} catch (std::exception& e) {
		std::cout << "Year " << static_cast<int>(year) << " Day " << static_cast<int>(day)
				<< " Error: could not solve.\n\t" << e.what() << '\n';
	}
	std::cout << '\n';
}



} /* namespace aoc */
