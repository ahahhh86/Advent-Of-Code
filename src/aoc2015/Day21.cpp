// https://adventofcode.com/2015/day/21
/*--- Day 21: RPG Simulator 20XX ---
Little Henry Case got a new video game for Christmas. It's an RPG, and he's stuck on a boss. He needs to know what
equipment to buy at the shop. He hands you the controller.
In this game, the player (you) and the enemy (the boss) take turns attacking. The player always goes first. Each attack
reduces the opponent's hit points by at least 1. The first character at or below 0 hit points loses.
Damage dealt by an attacker each turn is equal to the attacker's damage score minus the defender's armor score. An
attacker always does at least 1 damage. So, if the attacker has a damage score of 8, and the defender has an armor score
of 3, the defender loses 5 hit points. If the defender had an armor score of 300, the defender would still lose 1 hit point.
Your damage score and armor score both start at zero. They can be increased by buying items in exchange for gold. You
start with no items and have as much gold as you need. Your total damage or armor is equal to the sum of those stats
from all of your items. You have 100 hit points.

Here is what the item shop is selling:
	Weapons:    Cost  Damage  Armor
	Dagger        8     4       0
	Shortsword   10     5       0
	Warhammer    25     6       0
	Longsword    40     7       0
	Greataxe     74     8       0

	Armor:      Cost  Damage  Armor
	Leather      13     0       1
	Chainmail    31     0       2
	Splintmail   53     0       3
	Bandedmail   75     0       4
	Platemail   102     0       5

	Rings:      Cost  Damage  Armor
	Damage +1    25     1       0
	Damage +2    50     2       0
	Damage +3   100     3       0
	Defense +1   20     0       1
	Defense +2   40     0       2
	Defense +3   80     0       3

You must buy exactly one weapon; no dual-wielding. Armor is optional, but you can't use more than one. You can buy 0-2
rings (at most one for each hand). You must use any items you buy. The shop only has one of each item, so you can't buy,
for example, two rings of Damage +3.
For example, suppose you have 8 hit points, 5 damage, and 5 armor, and that the boss has 12 hit points, 7 damage,
and 2 armor:
	The player deals 5-2 = 3 damage; the boss goes down to 9 hit points.
	The boss deals 7-5 = 2 damage; the player goes down to 6 hit points.
	The player deals 5-2 = 3 damage; the boss goes down to 6 hit points.
	The boss deals 7-5 = 2 damage; the player goes down to 4 hit points.
	The player deals 5-2 = 3 damage; the boss goes down to 3 hit points.
	The boss deals 7-5 = 2 damage; the player goes down to 2 hit points.
	The player deals 5-2 = 3 damage; the boss goes down to 0 hit points.

In this scenario, the player wins! (Barely.)
You have 100 hit points. The boss's actual stats are in your puzzle input. What is the least amount of gold you can
spend and still win the fight?
Your puzzle answer was 111.

--- Part Two ---
Turns out the shopkeeper is working with the boss, and can persuade you to buy whatever items he wants. The other rules
still apply, and he still only has one of each item.
What is the most amount of gold you can spend and still lose the fight?
Your puzzle answer was 188.*/



//=== Include ================================================================
#include "Day21.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Constants ==============================================================
constexpr int own_max_hp {100};

constexpr int stat_count {3};
constexpr int stat_cost_slot {0};
constexpr int stat_damage_slot {1};
constexpr int stat_armor_slot {2};

constexpr int weapon_count {5};
constexpr int weapons_stats[weapon_count][stat_count] = {
		// Cost, Damage, Armor
		{ 8, 4, 0}, // Dagger
		{10, 5, 0}, // Shortsword
		{25, 6, 0}, // Warhammer
		{40, 7, 0}, // Longsword
		{74, 8, 0}  // Greataxe
};

constexpr int armor_count {6};
constexpr int armor_stats[armor_count][stat_count] = {
		// Cost, Damage, Armor
		{  0, 0, 0}, // None
		{ 13, 0, 1}, // Leather
		{ 31, 0, 2}, // Chainmail
		{ 53, 0, 3}, // Splintmail
		{ 75, 0, 4}, // Bandedmail
		{102, 0, 5}  // Platemail
};

constexpr int ring_count {8};
constexpr int ring_stats[ring_count][stat_count] = {
		// Cost, Damage, Armor
		{  0, 0, 0}, // None left hand
		{  0, 0, 0}, // None right hand
		{ 25, 1, 0}, // Damage +1
		{ 50, 2, 0}, // Damage +2
		{100, 3, 0}, // Damage +3
		{ 20, 0, 1}, // Defense +1
		{ 40, 0, 2}, // Defense +2
		{ 80, 0, 3}  // Defense +3
};



//=== Types ==================================================================
struct Equipment {
	int weapon {0};
	int armor {0};
	int leftRing {0};
	int rightRing {0};
};



struct SBossData {
	int hitPoints {0};
	int damage {0};
	int armor {0};
};



//=== Functions ==============================================================
// --- readBossData() ---
SBossData readBossData(std::ifstream input)
{
	SBossData result {};

	constexpr auto hit_points_width {std::string_view("Hit Points: ").length()};
	input.ignore(hit_points_width);
	input >> result.hitPoints;
	constexpr auto damage_width {std::string_view("Damage: ").length()};
	input.ignore(damage_width);
	input >> result.damage;
	constexpr auto armor_width {std::string_view("Armor: ").length()};
	input.ignore(armor_width);
	input >> result.armor;

	EXPECT(!input.fail(), invalid_input_file_data);
	return result;
}



// --- calulateBossDamage() ---
int calulateBossDamage(const Equipment& eq, int bossArmor) {
	constexpr int slot {stat_damage_slot};
	const int dmg = weapons_stats[eq.weapon][slot] + ring_stats[eq.leftRing][slot] + ring_stats[eq.rightRing][slot];
	return std::max(1, dmg - bossArmor);
}



// --- calulateSelfDamage() ---
int calulateSelfDamage(const Equipment& eq, int bossDamage) {
	constexpr int slot {stat_armor_slot};
	const int def = armor_stats[eq.armor][slot] + ring_stats[eq.leftRing][slot] + ring_stats[eq.rightRing][slot];
	return std::max(1, bossDamage - def);
}



// --- _calculateCost() ---
int calculateCost(const Equipment& eq) {
	constexpr int slot {stat_cost_slot};
	return weapons_stats[eq.weapon][slot] + armor_stats[eq.armor][slot] + ring_stats[eq.leftRing][slot] + ring_stats[eq.rightRing][slot];
}



// --- _canWin() ---
bool canWin(const Equipment& eq, const SBossData& boss) {
	const int boss_dmg = calulateBossDamage(eq, boss.armor);	// How much damage do I to the boss
	const int self_dmg = calulateSelfDamage(eq, boss.damage);	// How much damage does the boss to me
	const int turns {std::max(own_max_hp / self_dmg, 1)}; 		// How many turns do I have until I die (at least one)
	const int inflicted_damage {turns * boss_dmg}; 				// How much damage can I inflict before I die

	return inflicted_damage >= boss.hitPoints;
}



} /* anonymous namespace */



//=== Class Day21 ============================================================
// --- Day21::solve() ---
void Day21::solve() {
	try {
		const SBossData boss_status {readBossData(m_IO.getInputFile())};
		m_IO.printFileValid();

		Equipment eq {};
		int minWinCost {std::numeric_limits<int>::max()};
		int maxLoseCost {0};
		int cost {};

		for (eq.weapon = 0; eq.weapon < weapon_count; ++eq.weapon) {
			for (eq.armor = 0; eq.armor < armor_count; ++eq.armor) {
				for (eq.leftRing = 0; eq.leftRing < ring_count; ++eq.leftRing) {
					for (eq.rightRing = 1; eq.rightRing < ring_count; ++eq.rightRing) { // 0 and 1 are empty
						if ((eq.leftRing != eq.rightRing)) {
							cost = calculateCost(eq);
							if (canWin(eq, boss_status)) {
								minWinCost = std::min(minWinCost, cost);
							} else {
								maxLoseCost = std::max(maxLoseCost, cost);
							}
						}
					}
				}
			}
		}

		m_IO.printSolution(minWinCost, EPart::Part1);
		m_IO.printSolution(maxLoseCost, EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
