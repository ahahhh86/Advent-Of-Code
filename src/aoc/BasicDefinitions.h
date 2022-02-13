// BasicDefinitions.h
/* some very general definitions */



//=== Preprocessor ===========================================================
#ifndef AOC_BASICDEFINITIONS_H_
#define AOC_BASICDEFINITIONS_H_



//=== Include ================================================================
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>



//=== Preprocessor ===========================================================
/* Using a macros although they are often considered evil */

// Throw an error, if the condition is false
#define EXPECT(condition, msg) (condition ? 0 : throw std::runtime_error(std::string(__FILE__) + " @Line " + std::to_string(__LINE__) + ": " + msg))

// Throws an erro
#define THROW_ERROR(msg) (throw std::runtime_error(std::string(__FILE__) + " @Line " + std::to_string(__LINE__) + ": " + msg))



namespace aoc {
//=== Enums ==================================================================
enum class EYears {
	Year2015 = 2015, Year2016, Year2017, Year2018, Year2019, Year2020, Year2021
};

enum class EDays {
	Day01 = 1, Day02, Day03, Day04, Day05, Day06, Day07, Day08, Day09, Day10,
	Day11, Day12, Day13, Day14, Day15, Day16, Day17, Day18, Day19, Day20,
	Day21, Day22, Day23, Day24, Day25
};

enum class EPart {
	Part1 = 1, Part2
};



//=== Types =================================================================
using strSize_t = std::string::size_type;
using strViewSize_t = std::string_view::size_type;



//=== Consants ===============================================================
const std::string invalid_input_file_data {"Invalid input. Please check input file data."};



} /* namespace aoc */
#endif /* AOC_BASICDEFINITIONS_H_ */
