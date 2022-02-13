// https://adventofcode.com/2015/day/15
/* --- Day 15: Science for Hungry People ---
Today, you set out on the task of perfecting your milk-dunking cookie recipe. All you have to do is find the right balance of ingredients.
Your recipe leaves room for exactly 100 teaspoons of ingredients. You make a list of the remaining ingredients you could use to finish the recipe (your puzzle input) and their properties per teaspoon:
	capacity (how well it helps the cookie absorb milk)
	durability (how well it keeps the cookie intact when full of milk)
	flavor (how tasty it makes the cookie)
	texture (how it improves the feel of the cookie)
	calories (how many calories it adds to the cookie)
You can only measure ingredients in whole-teaspoon amounts accurately, and you have to be accurate so you can reproduce your results in the future. The total score of a cookie can be found by adding up each of the properties (negative totals become 0) and then multiplying together everything except calories.

For instance, suppose you have these two ingredients:
	Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8
	Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3

Then, choosing to use 44 teaspoons of butterscotch and 56 teaspoons of cinnamon (because the amounts of each ingredient must add up to 100) would result in a cookie with the following properties:
	A capacity of 44*-1 + 56*2 = 68
	A durability of 44*-2 + 56*3 = 80
	A flavor of 44*6 + 56*-2 = 152
	A texture of 44*3 + 56*-1 = 76

Multiplying these together (68 * 80 * 152 * 76, ignoring calories for now) results in a total score of 62842880, which happens to be the best score possible given these ingredients. If any properties had produced a negative total, it would have instead become zero, causing the whole score to multiply to zero.
Given the ingredients in your kitchen and their properties, what is the total score of the highest-scoring cookie you can make?
Your puzzle answer was 222870.

--- Part Two ---
Your cookie recipe becomes wildly popular! Someone asks if you can make another recipe that has exactly 500 calories per cookie (so they can use it as a meal replacement). Keep the rest of your award-winning process the same (100 teaspoons, same ingredients, same scoring system).
For example, given the ingredients above, if you had instead selected 40 teaspoons of butterscotch and 60 teaspoons of cinnamon (which still adds to 100), the total calorie count would be 40*8 + 60*3 = 500. The total score would go down, though: only 57600000, the best you can do in such trying circumstances.
Given the ingredients in your kitchen and their properties, what is the total score of the highest-scoring cookie you can make with a calorie total of 500?
Your puzzle answer was 117936. */



//=== Include ================================================================
#include "Day15.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Constants ==============================================================
constexpr int spoon_count {100};



//=== Types ==================================================================
struct SIngredientStat {
	std::string name {""};
	int capacity {0};
	int durability {0};
	int flavor {0};
	int texture {0};
	int calories {0};
};



struct SIngredients {
	int sugar {0};
	int sprinkles {0};
	int candy {0};
	int chocolate {0};
};



//=== Functions ==============================================================
SIngredientStat readIngredientStat(std::ifstream& file)
{
	SIngredientStat result {};

	std::string buffer {""};
	file >> result.name;
	result.name.pop_back(); // remove colon
	constexpr auto capacity_width {std::string_view("capacity ").length()};
	file.ignore(capacity_width);
	file >> result.capacity;
	constexpr auto durability_width {std::string_view(", durability ").length()};
	file.ignore(durability_width);
	file >> result.durability;
	constexpr auto flavor_width {std::string_view(", flavor ").length()};
	file.ignore(flavor_width);
	file >> result.flavor;
	constexpr auto texture_width {std::string_view(", texture ").length()};
	file.ignore(texture_width);
	file >> result.texture;
	constexpr auto calories_width {std::string_view(", calories ").length()};
	file.ignore(calories_width);
	file >> result.calories;
	EXPECT(!file.fail(), "Invalid input. Please check the input data.");

	return result;
}



//=== Class Recipes ==========================================================
class Recipes {
public:
// Constructors / destructor
	Recipes() = delete;
	Recipes(const Recipes&) = delete;
	Recipes(Recipes&&) = delete;
	~Recipes() = default;


	// --- Recipes() ---
	explicit Recipes(std::ifstream input) :
			m_sugarProperties {readIngredientStat(input)},
			m_sprinklesProperties {readIngredientStat(input)},
			m_candyProperties {readIngredientStat(input)},
			m_chocolateProperties {readIngredientStat(input)}
	{
	}


// Operators
	Recipes& operator=(const Recipes&) = delete;
	Recipes& operator=(Recipes&&) = delete;


// Functions
	// --- calculateScore() ---
	int calculateScore(const SIngredients ingr) const
	{
		EXPECT((ingr.sugar + ingr.sprinkles + ingr.candy + ingr.chocolate) == spoon_count, "You have to use 100 spoons.");

		const int capacity_score = std::max(
				0,
				m_sugarProperties.capacity * ingr.sugar + m_sprinklesProperties.capacity * ingr.sprinkles + m_candyProperties.capacity * ingr.candy + m_chocolateProperties.capacity * ingr.chocolate);
		const int durability_score = std::max(
				0,
				m_sugarProperties.durability * ingr.sugar + m_sprinklesProperties.durability * ingr.sprinkles + m_candyProperties.durability * ingr.candy + m_chocolateProperties.durability * ingr.chocolate);
		const int flavor_score = std::max(
				0,
				m_sugarProperties.flavor * ingr.sugar + m_sprinklesProperties.flavor * ingr.sprinkles + m_candyProperties.flavor * ingr.candy + m_chocolateProperties.flavor * ingr.chocolate);
		const int texture_score = std::max(
				0,
				m_sugarProperties.texture * ingr.sugar + m_sprinklesProperties.texture * ingr.sprinkles + m_candyProperties.texture * ingr.candy + m_chocolateProperties.texture * ingr.chocolate);

		return capacity_score * durability_score * flavor_score * texture_score;
	}


	// --- calculateCalories() ---
	int calculateCalories(const SIngredients ingr) const
	{
		EXPECT((ingr.sugar + ingr.sprinkles + ingr.candy + ingr.chocolate) == spoon_count, "You have to use 100 spoons.");

		return m_sugarProperties.calories * ingr.sugar + m_sprinklesProperties.calories * ingr.sprinkles + m_candyProperties.calories * ingr.candy + m_chocolateProperties.calories * ingr.chocolate;
	}


private:
// Variables
	// Assuming the same properties for each input // TODO: as vector
	const SIngredientStat m_sugarProperties;
	const SIngredientStat m_sprinklesProperties;
	const SIngredientStat m_candyProperties;
	const SIngredientStat m_chocolateProperties;
};



} /* anonymous namespace */



//=== Class Day15 ============================================================
// --- Day15::solve() ---
void Day15::solve()
{
	try {
		Recipes recipes {m_IO.getInputFile()};
		m_IO.printFileValid();

		constexpr int needed_calories {500};
		SIngredients ingr {};
		int score {};
		int max_score {0};
		int max_score_calories {0};

		for (ingr.sugar = 0; ingr.sugar < spoon_count; ++ingr.sugar) {
			for (ingr.sprinkles = 0; ingr.sprinkles < (spoon_count - ingr.sugar); ++ingr.sprinkles) {
				for (ingr.candy = 0; ingr.candy < (spoon_count - ingr.sugar - ingr.sprinkles); ++ingr.candy) {
					ingr.chocolate = spoon_count - ingr.sugar - ingr.sprinkles - ingr.candy;
					score = recipes.calculateScore(ingr);

					max_score = std::max(max_score, score);
					if (recipes.calculateCalories(ingr) == needed_calories) {
						max_score_calories = std::max(max_score_calories, score);
					}
				}
			}
		}

		m_IO.printSolution(max_score, EPart::Part1);
		m_IO.printSolution(max_score_calories, EPart::Part2);


	} catch (const std::exception& err) {
		m_IO.printError(err.what());
	}
}



} /* namespace aoc2015 */
