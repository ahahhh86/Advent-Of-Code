// https://adventofcode.com/2015/day/6
/* --- Day 6: Probably a Fire Hazard ---
Because your neighbors keep defeating you in the holiday house decorating contest year after year, you've decided to
deploy one million lights in a 1000x1000 grid.
Furthermore, because you've been especially nice this year, Santa has mailed you instructions on how to display the ideal
lighting configuration.
Lights in your grid are numbered from 0 to 999 in each direction; the lights at each corner are at 0,0, 0,999, 999,999, and 999,0.
The instructions include whether to turn on, turn off, or toggle various inclusive ranges given as coordinate pairs. Each
coordinate pair represents opposite corners of a rectangle, inclusive; a coordinate pair like 0,0 through 2,2 therefore refers
to 9 lights in a 3x3 square. The lights all start turned off.
To defeat your neighbors this year, all you have to do is set up your lights by doing the instructions Santa sent you in order.

For example:
	turn on 0,0 through 999,999 would turn on (or leave on) every light.
	toggle 0,0 through 999,0 would toggle the first line of 1000 lights, turning off the ones that were on, and turning on the ones that were off.
	turn off 499,499 through 500,500 would turn off (or leave off) the middle four lights.

After following the instructions, how many lights are lit?
Your puzzle answer was 377891.

--- Part Two ---
You just finish implementing your winning light pattern when you realize you mistranslated Santa's message from Ancient Nordic Elvish.
	The light grid you bought actually has individual brightness controls; each light can have a brightness of zero or more. The lights all start at zero.
	The phrase turn on actually means that you should increase the brightness of those lights by 1.
	The phrase turn off actually means that you should decrease the brightness of those lights by 1, to a minimum of zero.
	The phrase toggle actually means that you should increase the brightness of those lights by 2.
What is the total brightness of all lights combined after following Santa's instructions?

For example:
	turn on 0,0 through 0,0 would increase the total brightness by 1.
	toggle 0,0 through 999,999 would increase the total brightness by 2000000.

Your puzzle answer was 14110788. */



//=== Include ================================================================
#include "Day06.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"
#include "../aoc/TemplateContainer.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
using lightGrid_t = Grid<int>;
enum class ELightChange {invalid, on, off, toggle};



//=== Constants ==============================================================
constexpr lightGrid_t::dimension_t grid_size {1000};



//=== Types ==================================================================
struct SInstruction {
	ELightChange turnLight {ELightChange::invalid};
	lightGrid_t::Position point1 {0, 0};
	lightGrid_t::Position point2 {0, 0};
};
using instructionVector_t = std::vector<SInstruction>;



//=== Functions ==============================================================
// --- readInstruction() ---
SInstruction readInstruction(std::ifstream& file) {
	SInstruction result {};
	std::string strBuffer;

	// is toggle / turn on / turn off
	file >> strBuffer;
	if (strBuffer == "toggle") {
		result.turnLight = ELightChange::toggle;
	} else if (strBuffer == "turn") {
		file >> strBuffer;
		if (strBuffer == "on") {
			result.turnLight = ELightChange::on;
		} else if (strBuffer == "off") {
			result.turnLight = ELightChange::off;
		}
	}
	EXPECT(result.turnLight != ELightChange::invalid, invalid_input_file_data);

	constexpr auto separator_width {std::string_view(",").length()};
	constexpr auto through_width {std::string_view("through ").length()};

	// First point
	file >> result.point1.x;
	file.ignore(separator_width);
	file >> result.point1.y;
	file.ignore(through_width);

	// Second point
	file >> result.point2.x;
	file.ignore(separator_width);
	file >> result.point2.y;

	// is data valid?
	EXPECT(!file.fail(), invalid_input_file_data);
	EXPECT(std::max({result.point1.x, result.point1.y, result.point2.x, result.point2.y}) < grid_size, invalid_input_file_data);

	return result;
}



// --- readInstructionVector() ---
instructionVector_t readInstructionVector(std::ifstream file)
{
	instructionVector_t result {};
	while (!file.eof()) {
		result.push_back(readInstruction(file));
	}
	return result;
}



//=== Class LightGrid ========================================================
// Abstract grid of lights
class LightGrid: public lightGrid_t {
public:
// Constructors / destructor
	// --- LightGrid() ---
	LightGrid() : lightGrid_t(grid_size, grid_size)
	{
		// Do nothing
	}

	LightGrid(const LightGrid&) = delete;
	LightGrid(LightGrid&&) = delete;
	virtual ~LightGrid() = default;


// Operators
	LightGrid& operator=(const LightGrid&) = delete;
	LightGrid& operator=(LightGrid&&) = delete;


// Functions
	// --- executeInstructions() ---
	auto executeInstructions(const instructionVector_t& vec)
	{
		std::for_each(vec.cbegin(), vec.cend(), [&](const auto& data) {
			changeLights(data);
		});

		return accumulateData();
	}


protected:
// Functions (depending on the part of the puzzle)
	virtual void turnOn(const Position) = 0;
	virtual void turnOff(const Position) = 0;
	virtual void toggle(const Position) = 0;


private:
// Functions
	// --- changeLights() ---
	void changeLights(const SInstruction& input)
	{
		Position pos {};
		const auto forAllLights = [&](const auto func){
			for (pos.x = input.point1.x; pos.x <= input.point2.x; ++pos.x) {
				for (pos.y = input.point1.y; pos.y <= input.point2.y; ++pos.y) {
					func();
				}
			}
		};


		switch (input.turnLight) {
		case ELightChange::on:
			forAllLights([&](){turnOn(pos);});
			break;

		case ELightChange::off:
			forAllLights([&](){turnOff(pos);});
			break;

		case ELightChange::toggle:
			forAllLights([&](){toggle(pos);});
			break;

		default:
			THROW_ERROR(invalid_input_file_data); // Should already been handled at readInstruction(), but switch should have a default
		};
	}
};



// Implement methods specific to Part 1
class LightGridPart1 final : public LightGrid {
protected:
// Functions depending on the part of the puzzle
	// --- turnOn() ---
	void turnOn(const Position pos) override
	{
		operator [](pos) = 1;
	}


	// --- turnOff() ---
	void turnOff(const Position pos) override
	{
		operator [](pos) = 0;
	}


	// --- toggle() ---
	void toggle(const Position pos) override
	{
		data_t& light {operator [](pos)}; // just to make the next line easier to read
		light = (light > 0) ? 0 : 1;
	}
};



// Implement methods specific to Part 2
class LightGridPart2 final : public LightGrid {
protected:
// Functions depending on the part of the puzzle
	// --- turnOn() ---
	void turnOn(const Position pos) override
	{
		operator [](pos) += 1;
	}


	// --- turnOff() ---
	void turnOff(const Position pos) override
	{
		data_t& light {operator [](pos)}; // just to make the next line easier to read
		light = std::max(0, light - 1);
	}


	// --- toggle() ---
	void toggle(const Position pos) override
	{
		operator [](pos) += 2;
	}
};



} /* anonymous namespace */



//=== Class Day06 ============================================================
// --- Day06::solve() ---
void Day06::solve()
{
	try {
		const instructionVector_t input {readInstructionVector(m_IO.getInputFile())};
		m_IO.printFileValid();

		LightGridPart1 lights1 {};
		m_IO.printSolution(lights1.executeInstructions(input), EPart::Part1);

		LightGridPart2 lights2 {};
		m_IO.printSolution(lights2.executeInstructions(input), EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}

} /* namespace aoc2015 */
