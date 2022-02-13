// Timer.h
/* class to measure the time to solve a puzzle. */



//=== Preprocessor ===========================================================
#ifndef AOC_TIMER_H_
#define AOC_TIMER_H_



//=== Include ================================================================
#include <chrono>
#include <ratio>



namespace aoc {
//=== Class Timer ============================================================
// source: https://www.learncpp.com/cpp-tutorial/timing-your-code/
// small class, probably never changed, so definition is at header
class Timer {
public:
// Types
	using time_t = double;


// Functions
	// --- reset() ---
	void reset()
	{
		m_beg = clock_t::now();
	}

	// --- elapsed() ---
	time_t elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}


private:
// Types
	using clock_t = std::chrono::steady_clock;
	using second_t = std::chrono::duration<time_t, std::ratio<1> >;


// Variables
	std::chrono::time_point<clock_t> m_beg {clock_t::now()};
};



} /* namespace aoc */
#endif /* AOC_TIMER_H_ */
