// Grid.h
/* Some general container variants */



//=== Preprocessor ===========================================================
#ifndef AOC_CONTAINER_H_
#define AOC_CONTAINER_H_



//=== Include ================================================================
#include <algorithm>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <utility>
#include <vector>

#include "BasicDefinitions.h"



namespace aoc {
//=== Class Grid ============================================================
/* Two dimensional static array implemented using a vector. */
// TODO: make dynamic, problem: need to change positions when necessary
// CHECK: can be made compatible with bool?
// TODO: maybe add count_if
template<typename data_type>
class Grid {
public:
// Types
	using data_t = data_type;
	using dimension_t = unsigned int;
	using accumulate_t = long long;

	struct Position {
		dimension_t x {0};
		dimension_t y {0};
	};


// Constructors / destructor
	Grid() = delete;
	Grid(const Grid&) = default;
	Grid(Grid&&) = default;
	virtual ~Grid() = default;


	// --- Grid() ---
	Grid(const dimension_t gridWidth, const dimension_t gridHeight) :
			m_width {gridWidth}, m_height {gridHeight}, m_data(gridWidth * gridHeight)
	{
		EXPECT(gridWidth != 0 && gridHeight != 0, "Can not create a grid with at least one dimension = 0.");
	}


// Operators
	Grid& operator=(const Grid& other) = default;
	Grid& operator=(Grid&& other) = default;


	// --- operator[]() ---
	data_t& operator[](const Position pos)
	{
		return m_data[pos.x * m_width + pos.y];
	}


// Getter
	// --- getHeight() ---
	dimension_t getHeight() const
	{
		return m_height;
	}


	// --- getHeight() ---
	dimension_t getWidth() const
	{
		return m_width;
	}


// Functions
	// --- at() ---
	data_t& at(const Position pos)
	{
		EXPECT((pos.x < m_width) && (pos.y < m_height), "Grid out of range.");
		return m_data[pos.x * m_width + pos.y];
	}


	// --- accumulateData() ---
	accumulate_t accumulateData() const
	{
		const auto start_data {static_cast<accumulate_t>(m_data[0])};    // prevents accumulate from overflow for small types (e.g.: data_t = signed char)
		return std::accumulate(std::next(m_data.cbegin()), m_data.cend(), start_data);
	}


	// --- begin() ---
	auto begin()
	{
		return m_data.begin();
	}


	// --- end() ---
	auto end()
	{
		return m_data.end();
	}


private:
// Variables
	dimension_t m_width;		// Width of the data array
	dimension_t m_height;		// Height of the data array
	std::vector<data_t> m_data;		// Data container
};



//=== Class DoubleKeyMap =====================================================
template<typename key_type, typename value_type>
class DoubleKeyMap {
public:
// Types
	using key_t = key_type;
	using keyPair_t = std::pair<key_type, key_type>;
	using value_t = value_type;


// Functions
	// --- insert() ---
	void insert(const keyPair_t& key, const value_t& value)
	{
		m_mapData.insert(std::make_pair(key, value));
	}


	// --- find() ---
	auto find(const keyPair_t& key) const
	{
		if (m_mapData.find(key) != m_mapData.cend()) {
			return m_mapData.at(key);
		} else {
			// key is const, so we can not directly swap(key.first, key.second)
			const keyPair_t swapped_key {std::make_pair(key.second, key.first)};
			return m_mapData.at(swapped_key);
		}
	}


private:
// Variables
	std::map<keyPair_t, value_t> m_mapData {};
};



//=== Class DoubleKeyMap =====================================================
template<typename data_type>
class UniqueList {
public:
// Types
	using data_t = data_type;


// Functions
	// --- addData() ---
	void addData(const data_t& data)
	{
		m_listData.push_front(data);
		m_listData.sort();    // Need to sort, to make unique. Also a sorted list is required to loop through 'all' permutations
		m_listData.unique();
	}


	// --- nextPermutation() ---
	auto nextPermutation(const std::ptrdiff_t offset = 0)
	{
		return std::next_permutation(std::next(m_listData.begin(), offset), m_listData.end());
	}


// Getter
	// --- cbegin() ---
	auto cbegin() const
	{
		return m_listData.cbegin();
	}


	// --- cend() ---
	auto cend() const
	{
		return m_listData.cend();
	}


private:
// Variables
	std::list<data_t> m_listData {};
};



} /* namespace aoc */
#endif /* AOC_CONTAINER_H_ */
