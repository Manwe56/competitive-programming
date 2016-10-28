#include "gtest/gtest.h"

#include "competitive/programming/genetic/GeneticAlgorithm.hpp"

using competitive::programming::genetic::GeneticAlgorithm;

#include <iostream>
#include <random>

namespace {
	static int m_generatorValue = 0;
	std::mt19937 m_pseudoRandom(0);

	class Combination {
	public:
		Combination(int first, int second, int third, int fourth): m_first(first), m_second(second), m_third(third), m_fourth(fourth) {
		}

		bool operator<(const Combination& other) const {
			if (m_first == other.m_first) {
				if (m_second == other.m_second) {
					if (m_third == other.m_third) {
						return m_fourth < other.m_fourth;
					}
					return m_third < other.m_third;
				}
				return m_second < other.m_second;
			}
			return m_first < other.m_first;
		}

		bool operator==(const Combination& other) const {
			return m_first == other.m_first && m_second == other.m_second && m_third == other.m_third && m_fourth == other.m_fourth;
		}

		static Combination newInstance() {
			m_generatorValue = (m_generatorValue + 1) % 9;
			return Combination(m_generatorValue, m_generatorValue, m_generatorValue, m_generatorValue);
		}

		double evaluate(const Combination& toBeFound) const {
			double result = 0;

			if (m_first == toBeFound.m_first) {
				result += 10 + m_first;
			}
			if (m_second == toBeFound.m_second) {
				result += 10 + m_second;
			}
			if (m_third == toBeFound.m_third) {
				result += 10 + m_third;
			}
			if (m_fourth == toBeFound.m_fourth) {
				result += 10 + m_fourth;
			}

			return result;
		}
		Combination merge(const Combination& other) const {
			return Combination(
				randomBoolean() ? m_first : other.m_first, 
				randomBoolean() ? m_second : other.m_second, 
				randomBoolean() ? m_third : other.m_third,
				randomBoolean() ? m_fourth : other.m_fourth);
		}

		Combination mutate() const {
			return Combination(m_first, m_second, m_third, m_fourth + 1);
		}
		std::ostream& operator<<(std::ostream &os)
		{
			os << "C[" << m_first << m_second << m_third << m_fourth << "]";
			return os;
		}
	private:
		bool randomBoolean() const {
			return m_pseudoRandom()%2==0;
		}
	private:
		int m_first;
		int m_second;
		int m_third;
		int m_fourth;
	};
}

//Random stuff are not easy to test :D
//Combination could be only found with combination (generator generates only identical values) and mutations (9 could not be reached)

TEST(GeneticAlgorithm, Combination) {
	Combination toBeFound(0, 3, 7, 9);
	GeneticAlgorithm<Combination> algo(
		[toBeFound](const Combination &c) {return c.evaluate(toBeFound); },
		[]() {return Combination::newInstance(); },
		[](const Combination& first, const Combination& second) {return first.merge(second); },
		[](const Combination& c) {return c.mutate(); });

	algo.initialize(9);
	algo.iterate(10, 5, 20, 20, 20);
	
	ASSERT_EQ(toBeFound, algo.best());
}
