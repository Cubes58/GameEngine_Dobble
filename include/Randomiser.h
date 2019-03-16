/**
@file Randomiser.h
@brief Randomly generates numbers, within requested ranges.
*/
#pragma once

#include <random>

#define RandomiserInstance Randomiser::Instance()	//!< Direct access to the Randomiser singleton instance.

/*! \class Randomiser
	\brief Randomly generates numbers, within requested ranges.
*/

class Randomiser {
private:
	std::mt19937 m_Generator{std::random_device{}()};	//!< Random number engine.

	static constexpr unsigned int s_m_MaxNumberOfAttempts = 35;	//!< Number of attempts, to generate a number within a certain range, before giving up.

	Randomiser() = default;	//!< Constructor.
	~Randomiser() = default;	// Destructor.

public:
	/*!
		\brief Gets the Randomiser instance.
		\return Returns the Randomiser instance.
	*/
	static Randomiser &Instance() {
		static Randomiser s_Instance;

		return s_Instance;
	}

	/*!
		\brief Generates a random integer, within a requested range, uniformly (uniform distribution).
		\tparam T The type of number being randomly generated.
		\param p_Min The minimum value the random number can have.
		\param p_Max The maximum value the random number can have.
		\return Returns the randomly generated number.
	*/
	template<typename T>
	T GetUniformIntegerRandomNumber(const T &p_Min, const T &p_Max) {
		std::uniform_int_distribution<T> distribution((T)p_Min, (T)p_Max);
		return distribution(m_Generator);
	}

	/*!
		\brief Generates a random real number, within a requested range, uniformly (uniform distribution).
		\tparam T The type of number being randomly generated.
		\param p_Min The minimum value the random number can have.
		\param p_Max The maximum value the random number can have.
		\return Returns the randomly generated number.
	*/
	template<typename T>
	T GetUniformRealRandomNumber(const T &p_Min, const T &p_Max) {
		std::uniform_real_distribution<T> distribution((T)p_Min, (T)p_Max);
		return distribution(m_Generator);
	}

	/*!
		\brief Generates a random integer, within a requested range, with a normal distribution.
		\tparam T The type of number being randomly generated.
		\param p_Min The minimum value the random number can have.
		\param p_Max The maximum value the random number can have.
		\return Returns the randomly generated number.
	*/
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

	/*!
		\brief Gets the random number generator/engine.
		\return Returns random number generator/engine.
	*/
	std::mt19937 Generator() const {
		return m_Generator;
	}

	// Delete the copy and assignment operators.
	Randomiser(Randomiser const&) = delete; //!< Copy operator.
	Randomiser& operator= (Randomiser const&) = delete; //!< Assignment operator. 
};