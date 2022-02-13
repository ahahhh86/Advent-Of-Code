// https://adventofcode.com/2015/day/7
/* --- Day 7: Some Assembly Required ---
This year, Santa brought little Bobby Tables a set of wires and bitwise logic gates! Unfortunately, little Bobby is a
little under the recommended age range, and he needs help assembling the circuit.
Each wire has an identifier (some lowercase letters) and can carry a 16-bit signal (a number from 0 to 65535). A signal
is provided to each wire by a gate, another wire, or some specific value. Each wire can only get a signal from one source,
but can provide its signal to multiple destinations. A gate provides no signal until all of its inputs have a signal.
The included instructions booklet describes how to connect the parts together: x AND y -> z means to connect wires x and y
to an AND gate, and then connect its output to wire z.

For example:
	123 -> x means that the signal 123 is provided to wire x.
	x AND y -> z means that the bitwise AND of wire x and wire y is provided to wire z.
	p LSHIFT 2 -> q means that the value from wire p is left-shifted by 2 and then provided to wire q.
	NOT e -> f means that the bitwise complement of the value from wire e is provided to wire f.

Other possible gates include OR (bitwise OR) and RSHIFT (right-shift). If, for some reason, you'd like to emulate the
circuit instead, almost all programming languages (for example, C, JavaScript, or Python) provide operators for these gates.
For example, here is a simple circuit:
	123 -> x
	456 -> y
	x AND y -> d
	x OR y -> e
	x LSHIFT 2 -> f
	y RSHIFT 2 -> g
	NOT x -> h
	NOT y -> i

After it is run, these are the signals on the wires:
	d: 72
	e: 507
	f: 492
	g: 114
	h: 65412
	i: 65079
	x: 123
	y: 456

In little Bobby's kit's instructions booklet (provided as your puzzle input), what signal is ultimately provided to wire a?
Your puzzle answer was 3176.

--- Part Two ---
Now, take the signal you got on wire a, override wire b to that signal, and reset the other wires (including wire a).
What new signal is ultimately provided to wire a?
Your puzzle answer was 14710. */



//=== Include ================================================================
#include "Day07.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
using signal_t = uint16_t;
using signalMap_t = std::unordered_map<std::string, signal_t>;
using signalInput_t = std::variant<signal_t, std::string>;



enum class EGate {assign, Not, And, Or, lShift, rShift};

struct SInstruction {
	EGate gate {EGate::assign};
	signalInput_t input1 {""};
	signalInput_t input2 {""};
	std::string output {""};
};
using instructionVector_t = std::vector<SInstruction>;



//=== Functions ==============================================================
// --- readInstruction() ---
SInstruction readInstruction(const std::string& str)
{
	SInstruction result {};

	// Which gate is used
	if (str.find(" AND ") != std::string::npos) {
		result.gate = EGate::And;
	} else if (str.find(" OR ") != std::string::npos) {
		result.gate = EGate::Or;
	} else if (str.find(" RSHIFT ") != std::string::npos) {
		result.gate = EGate::rShift;
	} else if (str.find(" LSHIFT ") != std::string::npos) {
		result.gate = EGate::lShift;
	} else if (str.find("NOT ") == 0) { // NOT can only be at the start of the string
		result.gate = EGate::Not;
	} else {
		result.gate = EGate::assign;
	}

	// interpret input data
	std::istringstream strStream {str};

	// if possible get int data, else get the name of the wire
	const auto readInput = [&]() {
		signal_t intData {};
		strStream >> intData;
		if (!strStream.fail()) {
			return signalInput_t {intData};
		}

		strStream.clear(std::ios_base::failbit & std::ios::badbit);
		std::string strData {};
		strStream >> strData;
		return signalInput_t {strData};
	};

	constexpr auto space_width {std::string_view(" ").length()};
	constexpr auto not_width {std::string_view("NOT ").length()};
	constexpr auto operator_width {std::string_view("LSHIFT ").length()};	// Skip 'AND ', 'OR ', 'LSHIFT ' or 'RSHIFT '

	switch (result.gate) {
	// AND, OR, RSHIFT and LSHIFT have the same syntax
	case EGate::And:
	case EGate::Or:
	case EGate::rShift:
	case EGate::lShift:
		result.input1 = readInput();
		strStream.ignore(space_width);
		strStream.ignore(operator_width, ' '); // Skip 'AND ', 'OR ', 'LSHIFT ' or 'RSHIFT '
		result.input2 = readInput();
		break;

		// NOT and ASSIGN have similar syntax
	case EGate::Not:
		strStream.ignore(not_width);
		[[fallthrough]];    // Only difference of NOT and ASSIGN syntax is the 'NOT' at the start
	case EGate::assign:
		result.input1 = readInput();
		break;

	default:
		THROW_ERROR(invalid_input_file_data); // Should already been handled, but switch should have a default
	}


	// same syntax for all gates
	constexpr auto arrow_width {std::string_view("-> ").length()};
	strStream.ignore(arrow_width);
	strStream >> result.output;
	EXPECT(!strStream.fail(), invalid_input_file_data);

	return result;
}



// --- readInstructionVector() ---
instructionVector_t readInstructionVector(std::ifstream file)
{
	instructionVector_t result {};

	std::string buffer {};
	while (!file.eof()) {
		std::getline(file, buffer);
		result.push_back(readInstruction(buffer));
	}

	return result;
}



//=== Class BobbysKit ========================================================
class BobbysKit {
public:
// Functions
	// --- clear() ---
	void clear()
	{
		m_signals.clear();
	}


	// --- setSignal() ---
	bool setSignal(const SInstruction& data)
	{
		if (m_signals.find(data.output) != m_signals.cend()) { // Do not override signals
			return false;
		}


		// What is the first signal? If we do not know -> return
		signal_t first_input {};
		try {
			first_input = std::get<signal_t>(data.input1);
		} catch (const std::bad_variant_access&) {
			const auto wire {std::get<std::string>(data.input1)};
			if (m_signals.find(wire) != m_signals.cend()) {
				first_input = m_signals.at(wire);
			} else {
				return false;
			}
		}


		// If the gate is not Assign or not: what is the second signal?
		signal_t second_input {};
		if (!(data.gate == EGate::assign || data.gate == EGate::Not)) {
			try {
				second_input = std::get<signal_t>(data.input2);
			} catch (const std::bad_variant_access&) {
				const auto wire {std::get<std::string>(data.input2)};
				if (m_signals.find(wire) != m_signals.cend()) {
					second_input = m_signals.at(wire);
				} else {
					return false;
				}
			}
		}


		// Calculate output
		signal_t outputSignal {0};
		switch (data.gate) {
		case EGate::assign:
			outputSignal = first_input;
			break;
		case EGate::Not:
			outputSignal = ~first_input;
			break;

		case EGate::And:
			outputSignal = first_input & second_input;
			break;
		case EGate::Or:
			outputSignal = first_input | second_input;
			break;
		case EGate::lShift:
			outputSignal = first_input << second_input;
			break;
		case EGate::rShift:
			outputSignal = first_input >> second_input;
			break;

		default:
			THROW_ERROR(invalid_input_file_data); // Should already been handled, but switch should have a default
		}

		m_signals.insert({data.output, outputSignal});
		return true;
	}


	// --- setSignal() ---
	void setSignalB(signal_t input)
	{
		m_signals.insert({"b", input});
	}


	// --- executeSignals() ---
	signal_t executeSignals(const instructionVector_t& input)
	{
		do {
			int countSignalsChanged {0};
			std::for_each(input.cbegin(), input.cend(), [&](const auto& data) {
				if (setSignal(data)) {
					++countSignalsChanged;
				}
			});

			if (countSignalsChanged == 0) {
				THROW_ERROR("Created infinite loop.");
			}
		} while (m_signals.find("a") == m_signals.cend());

		return m_signals["a"];
	}


private:
// Variables
	signalMap_t m_signals {};
};



} /* anonymous namespace */



//=== Class Day07 ============================================================
// --- Day07::solve() ---
void Day07::solve()
{
	try {
		const instructionVector_t input {readInstructionVector(m_IO.getInputFile())};
		m_IO.printFileValid();

		BobbysKit kit {};
		const signal_t signalA {kit.executeSignals(input)};
		m_IO.printSolution(signalA, EPart::Part1);

		kit.clear();
		kit.setSignalB(signalA);
		m_IO.printSolution(kit.executeSignals(input), EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
