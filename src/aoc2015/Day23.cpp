// https://adventofcode.com/2015/day/23
/*--- Day 23: Opening the Turing Lock ---
Little Jane Marie just got her very first computer for Christmas from some unknown benefactor. It comes with instructions
and an example program, but the computer itself seems to be malfunctioning. She's curious what the program does, and
would like you to help her run it.
The manual explains that the computer supports two registers and six instructions (truly, it goes on to remind the
reader, a state-of-the-art technology). The registers are named a and b, can hold any non-negative integer, and begin
with a value of 0. The instructions are as follows:
	hlf r sets register r to half its current value, then continues with the next instruction.
	tpl r sets register r to triple its current value, then continues with the next instruction.
	inc r increments register r, adding 1 to it, then continues with the next instruction.
	jmp offset is a jump; it continues with the instruction offset away relative to itself.
	jie r, offset is like jmp, but only jumps if register r is even ("jump if even").
	jio r, offset is like jmp, but only jumps if register r is 1 ("jump if one", not odd).

All three jump instructions work with an offset relative to that instruction. The offset is always written with a
prefix + or - to indicate the direction of the jump (forward or backward, respectively). For example, jmp +1 would simply
continue with the next instruction, while jmp +0 would continuously jump back to itself forever.
The program exits when it tries to run an instruction beyond the ones defined.
For example, this program sets a to 2, because the jio instruction causes it to skip the tpl instruction:
	inc a
	jio a, +2
	tpl a
	inc a

What is the value in register b when the program in your puzzle input is finished executing?
Your puzzle answer was 170.

--- Part Two ---
The unknown benefactor is very thankful for releasi-- er, helping little Jane Marie with her computer. Definitely not to
distract you, what is the value in register b after the program is finished executing if register a starts as 1 instead?
Your puzzle answer was 247.*/



//=== Include ================================================================
#include "Day23.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"
#include "../aoc/TemplateContainer.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
using register_t = unsigned int;

enum class EInstruction {hlf, tpl, inc, jmp, jie, jio};
enum class ERegister {a, b};



struct SInputData {
	EInstruction instruction {EInstruction::inc};
	ERegister registr {ERegister::a}; // register is a keyword
	int value {0};
};
using InputVector_t = std::vector<SInputData>;



struct SComputerData {
	register_t a {0};
	register_t b {0};
	InputVector_t::size_type index {0};
};



//=== Functions ==============================================================
// --- readInputData() ---
SInputData readInputData(std::ifstream& input)
{
	SInputData result {};
	std::string buffer {};

	input >> buffer;
	if (buffer == "hlf") {
		result.instruction = EInstruction::hlf;
	} else if (buffer == "tpl") {
		result.instruction = EInstruction::tpl;
	} else if (buffer == "inc") {
		result.instruction = EInstruction::inc;
	} else if (buffer == "jmp") {
		result.instruction = EInstruction::jmp;
	} else if (buffer == "jie") {
		result.instruction = EInstruction::jie;
	} else if (buffer == "jio") {
		result.instruction = EInstruction::jio;
	} else {
		THROW_ERROR(invalid_input_file_data);
	}


	const auto getRegister = [&](std::string_view str) {
		if (str == "a") {
			return ERegister::a;
		} else if (str == "b") {
			return ERegister::b;
		} else {
			THROW_ERROR(invalid_input_file_data);
		}
	};


	switch (result.instruction) {
	case EInstruction::jmp:
		input >> result.value;
		break;

	case EInstruction::hlf:
	case EInstruction::tpl:
	case EInstruction::inc:
		input >> buffer;
		result.registr = getRegister(buffer);
		break;

	case EInstruction::jie:
	case EInstruction::jio:
		input >> buffer;
		buffer.pop_back(); // remove ','
		result.registr = getRegister(buffer);
		input >> result.value;
		break;

	default:
		THROW_ERROR(invalid_input_file_data); // Should already been handled, but switch should have a default
		break;
	}

	EXPECT(!input.fail(), invalid_input_file_data);
	return result;
}



// --- readInputData() ---
InputVector_t readInputVector(std::ifstream input)
{
	InputVector_t result {};

	while (!input.eof()) {
		result.push_back(readInputData(input));
	}

	return result;
}



// --- getNextLine() ---
SComputerData getNextLine(const SComputerData& data, const InputVector_t& vec)
{
	SComputerData result {data};
	const SInputData& input {vec[data.index]};
	register_t& reg {input.registr == ERegister::a ? result.a : result.b};

	switch (input.instruction) {
	case EInstruction::jmp:
		result.index += input.value;
		break;

	case EInstruction::hlf:
		reg /= 2;
		++result.index;
		break;

	case EInstruction::tpl:
		reg *= 3; // @suppress("Avoid magic numbers")
		++result.index;
		break;

	case EInstruction::inc:
		++reg;
		++result.index;
		break;

	case EInstruction::jie:
		if (reg % 2 == 0) {
			result.index += input.value;
		} else {
			++result.index;
		}
		break;

	case EInstruction::jio:
		if (reg == 1) {
			result.index += input.value;
		} else {
			++result.index;
		}
		break;

	default:
		THROW_ERROR(invalid_input_file_data); // Should already been handled, but switch should have a default
		break;
	}

	return result;
}



} /* anonymous namespace */



//=== Class Day23 ============================================================
// --- Day23::solve() ---
void Day23::solve() {
	try {
		InputVector_t inputData {readInputVector(m_IO.getInputFile())};
		m_IO.printFileValid();

		SComputerData values {};
		const auto loop_end {inputData.size()};

		while (values.index < loop_end) {
			values = getNextLine(values, inputData);
		}

		m_IO.printSolution(values.b, EPart::Part1);

		values = {1, 0, 0};
		while (values.index < loop_end) {
			values = getNextLine(values, inputData);
		}
		m_IO.printSolution(values.b, EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
