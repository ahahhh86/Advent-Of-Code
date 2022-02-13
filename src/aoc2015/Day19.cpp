// https://adventofcode.com/2015/day/19
/*--- Day 19: Medicine for Rudolph ---

Rudolph the Red-Nosed Reindeer is sick! His nose isn't shining very brightly, and he needs medicine.
Red-Nosed Reindeer biology isn't similar to regular reindeer biology; Rudolph is going to need custom-made medicine.
Unfortunately, Red-Nosed Reindeer chemistry isn't similar to regular reindeer chemistry, either.
The North Pole is equipped with a Red-Nosed Reindeer nuclear fusion/fission plant, capable of constructing any Red-Nosed
Reindeer molecule you need. It works by starting with some input molecule and then doing a series of replacements, one
per step, until it has the right molecule.
However, the machine has to be calibrated before it can be used. Calibration involves determining the number of molecules
that can be generated in one step from a given starting point.
For example, imagine a simpler machine that supports only the following replacements:
	H => HO
	H => OH
	O => HH

Given the replacements above and starting with HOH, the following molecules could be generated:
	HOOH (via H => HO on the first H).
	HOHO (via H => HO on the second H).
	OHOH (via H => OH on the first H).
	HOOH (via H => OH on the second H).
	HHHH (via O => HH).

So, in the example above, there are 4 distinct molecules (not five, because HOOH appears twice) after one replacement from
HOH. Santa's favorite molecule, HOHOHO, can become 7 distinct molecules (over nine replacements: six from H, and three from O).
The machine replaces without regard for the surrounding characters. For example, given the string H2O, the transition
H => OO would result in OO2O.
Your puzzle input describes all of the possible replacements and, at the bottom, the medicine molecule for which you need
to calibrate the machine. How many distinct molecules can be created after all the different ways you can do one replacement
on the medicine molecule?

Your puzzle answer was 518.

--- Part Two ---
Now that the machine is calibrated, you're ready to begin molecule fabrication.
Molecule fabrication always begins with just a single electron, e, and applying replacements one at a time, just like
the ones during calibration.
For example, suppose you have the following replacements:
	e => H
	e => O
	H => HO
	H => OH
	O => HH

If you'd like to make HOH, you start with e, and then make the following replacements:
	e => O to get O
	O => HH to get HH
	H => OH (on the second H) to get HOH

So, you could make HOH after 3 steps. Santa's favorite molecule, HOHOHO, can be made in 6 steps.
How long will it take to make the medicine? Given the available replacements and the medicine molecule in your puzzle
input, what is the fewest number of steps to go from e to the medicine molecule?*/

// TODO: solve part 2
/* find.length < replace.length => if result.length > molecule.length then abort*/



//=== Include ================================================================
#include "Day19.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <string_view>
#include <vector>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
struct SReplacement {
	std::string find {""};
	std::string replace {""};
};
using replacements_t = std::vector<SReplacement>;

struct SInputData {
	replacements_t replacements {};
	std::string medicineMolecule {""};
};



//=== Functions ==============================================================
SInputData readInput(std::ifstream file)
{
	SInputData result {};
	SReplacement buffer {};

	while (true) {
		constexpr auto sep_width {std::string_view("=> ").length()};
		constexpr strSize_t max_find_molecule_length {10}; // the start molecule in my input has 1 or 2 letters, while the medicine molecule has a lot more

		file >> buffer.find;
		if (buffer.find.length() > max_find_molecule_length) {
			break;
		}
		file.ignore(sep_width);
		file >> buffer.replace;
		result.replacements.push_back(buffer);
	}
	EXPECT(!file.fail(), invalid_input_file_data);

	result.medicineMolecule = buffer.find;
	return result;
}



// --- _getReplacements() ---
auto getReplacements(const SInputData& input)
{
	std::list<std::string> distinctMolecules {};
	std::string buffer {};

	std::for_each(input.replacements.cbegin(), input.replacements.cend(), [&](const auto& data){
		const strSize_t findLength {data.find.length()};

		for (auto i {input.medicineMolecule.find(data.find, 0)}; i != std::string::npos; i = input.medicineMolecule.find(data.find, i + findLength)) {
			buffer = input.medicineMolecule;
			buffer.replace(i, findLength, data.replace);
			distinctMolecules.push_back(buffer);
		}
	});

	distinctMolecules.sort();
	distinctMolecules.unique();
	return distinctMolecules.size();
}



// --- _getReplacements() ---
auto buildMedicineMolecule(const SInputData&/* input*/)
{
	//int result {0};



	return "TODO";//result;
}



} /* anonymous namespace */



//=== Class Day19 ============================================================
// --- Day19::solve() ---
void Day19::solve()
{
	try {
		SInputData input {readInput(m_IO.getInputFile())};
		m_IO.printFileValid();

		m_IO.printSolution(getReplacements(input), EPart::Part1);
		m_IO.printSolution(buildMedicineMolecule(input), EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
