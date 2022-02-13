// https://adventofcode.com/2015/day/2
/* --- Day 2: I Was Told There Would Be No Math ---
The elves are running low on wrapping paper, and so they need to submit an order for more. They have a list of the
dimensions (length l, width w, and height h) of each present, and only want to order exactly as much as they need.
Fortunately, every present is a box (a perfect right rectangular prism), which makes calculating the required wrapping
paper for each gift a little easier: find the surface area of the box, which is 2*l*w + 2*w*h + 2*h*l. The elves also
need a little extra paper for each present: the area of the smallest side.

For example:
	A present with dimensions 2x3x4 requires 2*6 + 2*12 + 2*8 = 52 square feet of wrapping paper plus 6 square feet of slack, for a total of 58 square feet.
	A present with dimensions 1x1x10 requires 2*1 + 2*10 + 2*10 = 42 square feet of wrapping paper plus 1 square foot of slack, for a total of 43 square feet.

All numbers in the elves' list are in feet. How many total square feet of wrapping paper should they order?
Your puzzle answer was 1598415.

--- Part Two ---
The elves are also running low on ribbon. Ribbon is all the same width, so they only have to worry about the length they
need to order, which they would again like to be exact.
The ribbon required to wrap a present is the shortest distance around its sides, or the smallest perimeter of any one
face. Each present also requires a bow made out of ribbon as well; the feet of ribbon required for the perfect bow is
equal to the cubic feet of volume of the present. Don't ask how they tie the bow, though; they'll never tell.

For example:
	A present with dimensions 2x3x4 requires 2+2+3+3 = 10 feet of ribbon to wrap the present plus 2*3*4 = 24 feet of ribbon for the bow, for a total of 34 feet.
	A present with dimensions 1x1x10 requires 1+1+1+1 = 4 feet of ribbon to wrap the present plus 1*1*10 = 10 feet of ribbon for the bow, for a total of 14 feet.

How many total feet of ribbon should they order?
Your puzzle answer was 3812909. */



//=== Include ================================================================
#include "Day02.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "../aoc/BasicDefinitions.h"
#include "../aoc/BasicIO.h"



namespace aoc2015 {
using namespace aoc;
namespace {



//=== Types ==================================================================
using length_t = unsigned int;
using area_t = unsigned int;



//=== Constants ==============================================================
constexpr length_t invalid_length {0};    // Would not make sense to have a present side of 0



//=== Types ==================================================================
struct SPresent {
	length_t x {invalid_length};
	length_t y {invalid_length};
	length_t z {invalid_length};
};
using presentVector_t = std::vector<SPresent>;



//=== Functions ==============================================================
// --- readPresent() ---
SPresent readPresent(std::ifstream& file)
{
	constexpr auto x_width {std::string_view("x").length()};
	SPresent result {};

	file >> result.x;
	file.ignore(x_width);
	file >> result.y;
	file.ignore(x_width);
	file >> result.z;

	EXPECT(!file.fail(), invalid_input_file_data);
	EXPECT(std::min({result.x, result.y, result.z}) > invalid_length, invalid_input_file_data);

	return result;
}



// --- readPresentVector() ---
presentVector_t readPresentVector(std::ifstream file)
{
	presentVector_t result {};
	while (!file.eof()) {
		result.push_back(readPresent(file));
	}
	return result;
}



// --- getWrappingPaper() ---
area_t getWrappingPaper(const SPresent& present)
{
	const area_t area_1 {present.x * present.y};
	const area_t area_2 {present.x * present.z};
	const area_t area_3 {present.y * present.z};
	const area_t add_area {std::min( {area_1, area_2, area_3})};

	return 2 * (area_1 + area_2 + area_3) + add_area;
}



// --- getRibbonLength() ---
length_t getRibbonLength(const SPresent& present)
{
	const length_t perimeter_1 {present.x + present.y};		// actually half perimeter
	const length_t perimeter_2 {present.x + present.z};		// actually half perimeter
	const length_t perimeter_3 {present.y + present.z};		// actually half perimeter
	const length_t smallest_perimeter {2 * std::min( {perimeter_1, perimeter_2, perimeter_3})};
	const length_t bow_length = present.x * present.y * present.z;

	return smallest_perimeter + bow_length;
}



} /* anonymous namespace */



//=== Class Day02 ============================================================
// --- Day02::solve() ---
void Day02::solve()
{
	try {
		const presentVector_t presents {readPresentVector(m_IO.getInputFile())};
		m_IO.printFileValid();

		const auto getSum = [&](const auto func) {
			return std::accumulate(std::next(presents.cbegin()), presents.cend(), func(*presents.cbegin()), [&](auto& sum, const auto& data) {
				return sum + func(data);
			});
		};

		m_IO.printSolution(getSum(getWrappingPaper), EPart::Part1);
		m_IO.printSolution(getSum(getRibbonLength), EPart::Part2);



	} catch (const std::exception& e) {
		m_IO.printError(e.what());
	}
}



} /* namespace aoc2015 */
