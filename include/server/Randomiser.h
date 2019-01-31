#pragma once

#include <random>

class Randomiser {
private:
	std::random_device m_RandomDevice;
	std::default_random_engine m_Generator;

	Randomiser() : m_Generator(m_RandomDevice()) {}
	~Randomiser() = default;

public:
	static Randomiser &getInstance() {
		static Randomiser instance;

		return instance;
	}

	template<typename T>
	T GetIntegerRandomNumber(const T &p_Min, const T &p_Max) {
		std::uniform_int_distribution<T> distribution((T)p_Min, (T)p_Max);
		return distribution(m_Generator);
	}

	template<typename T>
	T GetRealRandomNumber(const T &p_Min, const T &p_Max) {
		std::uniform_real_distribution<T> distribution((T)p_Min, (T)p_Max);
		return distribution(m_Generator);
	}

	// Delete the copy and assignment operators.
	Randomiser(Randomiser const&) = delete; //!< Copy operator.
	Randomiser& operator= (Randomiser const&) = delete; //!< Assignment operator. 
};