// BasicIo.cpp

//=== Include ================================================================
#include "BasicIO.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "BasicDefinitions.h"
#include "Timer.h"



namespace aoc {
//=== Class BasicIO ============================================================
// --- BasicIO::BasicIO() ---
BasicIO::BasicIO(const EYears year, const EDays day) : m_year(year), m_day(day)
{
	std::cout << "Year " << static_cast<int>(m_year) << " Day " << static_cast<int>(m_day) << ":\n";
}



// --- BasicIO::getInputFile() ---
std::ifstream BasicIO::getInputFile() const
{
	using namespace std::string_literals;

	const std::string day_num {m_day < aoc::EDays::Day10
		? "0"s + std::to_string(static_cast<int>(m_day)) // File names should be 'Day01', ...
		: std::to_string(static_cast<int>(m_day))};
	const std::string file_name {".\\input\\"s + std::to_string(static_cast<int>(m_year)) + "\\" + "Day"s + day_num + ".txt"s};

	std::ifstream result(file_name);
	EXPECT(result.is_open(), "Could not open file: "s + file_name);
	return result;
}



// --- BasicIO::getInputString() ---
std::string BasicIO::getInputString() const
{
	std::string result {};
	getInputFile() >> result;
	return result;
}



// --- BasicIO::printFileValid() ---
void BasicIO::printFileValid() const
{
	std::cout << "\tLoaded and validated input file: ";
	timeStamp();
}



// --- BasicIO::printError() ---
void BasicIO::printError(std::string_view error) const
{
	std::cout << "\tCould not solve puzzle.\n\t" << error << '\n';
}



// --- BasicIO::timeStamp() ---
void BasicIO::timeStamp() const
{
	constexpr int float_precision {3};
	constexpr int float_width {8};
	constexpr int time_width {24};

	std::cout << std::fixed << std::setprecision(float_precision) << std::setw(time_width)
			<< "Time: " << std::setw(float_width) << m_timer.elapsed() << " s\n";
}



} /* namespace aoc */
