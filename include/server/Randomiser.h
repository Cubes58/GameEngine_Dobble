#pragma once

#include <random>

#define RandomiserInstance Randomiser::Instance()

class Randomiser {
private:
	std::random_device m_RandomDevice;
	std::mt19937 m_Generator;
	//std::default_random_engine m_Generator;

	static constexpr unsigned int s_m_MaxNumberOfAttempts = 35;

	Randomiser() : m_Generator(m_RandomDevice()) {}
	~Randomiser() = default;

public:
	static Randomiser &Instance() {
		static Randomiser s_Instance;

		return s_Instance;
	}

	template<typename T>
	T GetUniformIntegerRandomNumber(const T &p_Min, const T &p_Max) {
		std::uniform_int_distribution<T> distribution((T)p_Min, (T)p_Max);
		return distribution(m_Generator);
	}

	template<typename T>
	T GetUniformRealRandomNumber(const T &p_Min, const T &p_Max) {
		std::uniform_real_distribution<T> distribution((T)p_Min, (T)p_Max);
		return distribution(m_Generator);
	}

	template<typename T>
	T GetNormalRandomNumber(const T &p_Min, const T &p_Max) {
		static unsigned int numberOfAttempts(0);
		std::normal_distribution<T> distribution((T)((p_Min + p_Max) / 2), (T)((p_Max - p_Min) / 6));

		if (numberOfAttempts >= s_m_MaxNumberOfAttempts)
			return T(0);

		T value = 0;
		do {
			value = distribution(m_Generator);

			distribution.reset();
			++numberOfAttempts;
		} while (value < p_Min || value > p_Max);

		// Reset the number of attempts, if a random number within the range is generated.
		numberOfAttempts = 0;
		return value;
	}

	std::default_random_engine Generator() const {
		return m_Generator;
	}

	// Delete the copy and assignment operators.
	Randomiser(Randomiser const&) = delete; //!< Copy operator.
	Randomiser& operator= (Randomiser const&) = delete; //!< Assignment operator. 
};