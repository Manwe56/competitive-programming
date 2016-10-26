#ifndef __QUADRATIC_EQUATION_INCLUDED__
#define __QUADRATIC_EQUATION_INCLUDED__

#include "competitive/programming/math/Complex.hpp"

/**
* @author Manwe
*
* class to solve quadratic equations
*
*/

#include <math.h>

namespace competitive {
	namespace programming {
		namespace math {
			class QuadraticEquation {
			public:
				/**
				* class solving quadratic equations:
				* Finding x where a*x*x+b*x+c=0
				* 
				* solving is done within the constructor.
				*/
				QuadraticEquation(double a, double b, double c) :m_solutionsCount(0), m_firstRoot(0,0), m_secondRoot(0,0){
					if (a == 0) {
						if (b != 0) {
							m_solutionsCount = 1;
							m_firstRoot = Complex(-c / b, 0);
						}
						else {
							// no solution!
						}
					}
					else {
						if (a<0) {//to have smallest root first
							a = -a;
							b = -b;
							c = -c;
						}
						double delta = b*b - 4 * a*c;

						if (delta<0) {
							double deltaSqrt = sqrt(-delta);
							
							m_solutionsCount = 2;
							m_firstRoot = Complex(-b / (2 * a), -deltaSqrt / (2 * a));
							m_secondRoot = Complex(-b / (2 * a), deltaSqrt / (2 * a));
						}
						else if (delta>0) {
							double deltaSqrt = sqrt(delta);

							m_solutionsCount = 2;
							m_firstRoot = Complex((-b - deltaSqrt) / (2 * a), 0);
							m_secondRoot = Complex((-b + deltaSqrt) / (2 * a), 0);
						}
						else {
							m_solutionsCount = 1;
							m_firstRoot = Complex(-b / (2 * a), 0);
						}
					}
				}
				/**
				* return the number of valid root found (could be 0, 1 or 2)
				*/
				int getSolutionsCount()const {
					return m_solutionsCount;
				}
				/**
				* return the first root found
				* double check before use that there is at least 1 solution!
				*/
				const Complex& getFirstRoot() const {
					return m_firstRoot;
				}
				/**
				* return the first root found
				* double check before use that there is 2 solutions!
				*/
				const Complex& getSecondRoot() const {
					return m_secondRoot;
				}

			private:
				int m_solutionsCount;
				Complex m_firstRoot;
				Complex m_secondRoot;
			};
		}
	}
}

#endif
