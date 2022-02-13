// https://adventofcode.com/2015/day/14
/* --- Day 14: Reindeer Olympics ---
This year is the Reindeer Olympics! Reindeer can fly at high speeds, but must rest occasionally to recover their energy.
Santa would like to know which of his reindeer is fastest, and so he has them race.
Reindeer can only either be flying (always at their top speed) or resting (not moving at all), and always spend whole
seconds in either state.

For example, suppose you have the following Reindeer:
	Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
	Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds.

After one second, Comet has gone 14 km, while Dancer has gone 16 km. After ten seconds, Comet has gone 140 km, while
Dancer has gone 160 km. On the eleventh second, Comet begins resting (staying at 140 km), and Dancer continues on for a
total distance of 176 km. On the 12th second, both reindeer are resting. They continue to rest until the 138th second,
when Comet flies for another ten seconds. On the 174th second, Dancer flies for another 11 seconds.
In this example, after the 1000th second, both reindeer are resting, and Comet is in the lead at 1120 km (poor Dancer has
only gotten 1056 km by that point). So, in this situation, Comet would win (if the race ended at 1000 seconds).
Given the descriptions of each reindeer (in your puzzle input), after exactly 2503 seconds, what distance has the winning
reindeer traveled?
Your puzzle answer was 2640.

--- Part Two ---
Seeing how reindeer move in bursts, Santa decides he's not pleased with the old scoring system.
Instead, at the end of each second, he awards one point to the reindeer currently in the lead. (If there are multiple
reindeer tied for the lead, they each get one point.) He keeps the traditional 2503 second time limit, of course, as
doing otherwise would be entirely ridiculous.
Given the example reindeer from above, after the first second, Dancer is in the lead and gets one point. He stays in the
lead until several seconds into Comet's second burst: after the 140th second, Comet pulls into the lead and gets his
first point. Of course, since Dancer had been in the lead for the 139 seconds before that, he has accumulated 139 points
by the 140th second.
After the 1000th second, Dancer has accumulated 689 points, while poor Comet, our old champion, only has 312. So, with
the new scoring system, Dancer would win (if the race ended at 1000 seconds).
Again given the descriptions of each reindeer (in your puzzle input), after exactly 2503 seconds, how many points does
the winning reindeer have?
Your puzzle answer was 1102. */



//=== Include ================================================================
#include "Day14.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
using speed_t = unsigned int;
using distance_t = unsigned int;
using time_t = unsigned int;



//=== Class Reindeer ========================================================
class Reindeer {
public:
// Getter
	// --- getTraveledDistance() ---
	auto getTraveledDistance() const
	{
		return m_traveledDistance;
	}


// Functions
	// --- fly() ---
	void fly()
	{
		if (m_flyTime > 0) {
			--m_flyTime;
			m_traveledDistance += m_maxSpeed;
		} else {
			--m_restTime;
			if (m_restTime == 0) {
				m_restTime = m_maxRestTime;
				m_flyTime = m_maxFlyTime;
			}
		}
	}


	// --- reset() ---
	void reset()
	{
		m_flyTime = m_maxFlyTime;
		m_restTime = m_maxRestTime;
		m_traveledDistance = 0;
		score = 0;
	}


// Variables
	unsigned int score {0}; // TODO: own vector to track score


// Friends
	friend Reindeer readReindeer(std::ifstream&);


private:
// Variables
	std::string m_name {""};
	speed_t m_maxSpeed {0};
	time_t m_maxFlyTime {0};
	time_t m_maxRestTime {0};
	time_t m_flyTime {0};
	time_t m_restTime {0};
	distance_t m_traveledDistance {0};
};
using reindeerVector_t = std::vector<Reindeer>;



Reindeer readReindeer(std::ifstream& file)// insert into reindeer constructor
{
	Reindeer result {};

	file >> result.m_name;
	constexpr auto fly_width {std::string_view("can fly ").length()};
	file.ignore(fly_width);
	file >> result.m_maxSpeed;
	constexpr auto km_width {std::string_view("km/s for ").length()};
	file.ignore(km_width);
	file >> result.m_maxFlyTime;
	constexpr auto rest_width {std::string_view("seconds, but then must rest for ").length()};
	file.ignore(rest_width);
	file >> result.m_maxRestTime;
	constexpr auto end_width {std::string_view("seconds.\r\n").length()};
	file.ignore(end_width);
	EXPECT(!file.fail(), invalid_input_file_data);

	result.m_flyTime = result.m_maxFlyTime;
	result.m_restTime = result.m_maxRestTime;

	return result;
}



reindeerVector_t readReindeerVector(std::ifstream file)
{
	reindeerVector_t result {};

	while (!file.eof()) {
		result.push_back(readReindeer(file));
	}

	return result;
}



}/* anonymous namespace */



//=== Class Day14 ============================================================
// --- Day14::solve() ---
void Day14::solve() // TODO: refactor
{
	try {
		reindeerVector_t reindeers {readReindeerVector(m_IO.getInputFile())};
		m_IO.printFileValid();

		constexpr time_t raceTime {2503};

		for (time_t i {0}; i < raceTime; ++i) {
			std::for_each(reindeers.begin(), reindeers.end(), [](auto& rdeer) { // TODO: refactor
				rdeer.fly();
			});
		}

		const auto getMaxDistance = [&]() {
			distance_t result {0};
			std::for_each(reindeers.cbegin(), reindeers.cend(), [&](const auto& rdeer) {
						result = std::max(result, rdeer.getTraveledDistance());
					});
			return result;
		};
		m_IO.printSolution(getMaxDistance(), EPart::Part1);

		std::for_each(reindeers.begin(), reindeers.end(), [](auto& rdeer) {
			rdeer.reset();
		});

		for (time_t i {0}; i < raceTime; ++i) {
			std::for_each(reindeers.begin(), reindeers.end(), [](auto& rdeer) {
				rdeer.fly();
			});
			const distance_t maxDistance {getMaxDistance()};
			std::for_each(reindeers.begin(), reindeers.end(), [&](auto& rdeer) {
				if (rdeer.getTraveledDistance() == maxDistance) {
					++rdeer.score;
				}
			});
		}
		const auto getMaxScore = [&]() {
			distance_t result {0};
			std::for_each(reindeers.cbegin(), reindeers.cend(), [&](const auto& rdeer) {
						result = std::max(result, rdeer.score);
					});
			return result;
		};
		m_IO.printSolution(getMaxScore(), EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
