// AdventOfCode.cpp
/* Main TODO: comment */

// General TO DOs
// CHECK: Why std::string {std::string} is flagged as an error in eclipse, but compiles and runs, while () or = initialisation is fine



//=== Preprocessor ===========================================================
//#define NDEBUG
//#define FILE_OUTPUT



//=== Include ================================================================
#include <fstream>
#include <iostream>

#include "aoc/Aoc.h"
#include "aoc/BasicDefinitions.h"

#include <vector>
#include <algorithm>



//=== main() =================================================================
int main() {
	std::ios_base::sync_with_stdio(false);


#ifdef FILE_OUTPUT // TODO: Rework, own class, maybe use command line argument as switch
	std::cout << "Using file output." << std::endl;
	std::streambuf* stdCout {std::cout.rdbuf()};

	std::ofstream out {"AdventOfCodeSolutions.txt"};
	std::cout.rdbuf(out.rdbuf());
#endif // #ifdef FILE_OUTPUT


	std::cout << "Advent of Code (adventofcode.com):\n";
	// TODO: Loop through all puzzles
	aoc::solvePuzzle(aoc::EYears::Year2015, aoc::EDays::Day23);


#ifdef FILE_OUTPUT
	std::cout.rdbuf(stdCout);
#endif // #ifdef FILE_OUTPUT



#ifdef NDEBUG // Prevent closing if opening the *.exe
	std::cout << "\nDONE <Press enter to exit>";
	std::string buffer {""};
	std::getline(std::cin, buffer);
#endif //#ifdef NDEBUG

	return 0;
}
//============================================================================