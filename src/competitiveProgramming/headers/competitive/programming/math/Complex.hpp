#ifndef __COMPLEX_INCLUDED__
#define __COMPLEX_INCLUDED__

/**
* @author Manwe
*
* Class representing a complex number.
* This complex number has a real and imaginary part
*/

#include <iostream>

namespace competitive {
	namespace programming {
		namespace math {
			class Complex {
			public:
				/**
				* Construct a complex from a real and imaginary value
				* @param real
				* 	assigned to the real part of the complex number
				* @param imaginary
				*  assigned to the imaginary part of the complex number
				*/
				Complex(double real, double imaginary): m_real(real), m_imaginary(imaginary) {
				}
				double getReal() const {
					return m_real;
				}
				double getImaginary() const {
					return m_imaginary;
				}
				/**
				* @return
				* true if the complex number has no imaginary value (equals 0)
				*/
				bool isReal() {
					return m_imaginary == 0;
				}

				bool operator==(const Complex& other) const {
					return m_real == other.m_real && m_imaginary == other.m_imaginary;
				}
			private:
				double m_real;
				double m_imaginary;
			};

			std::ostream& operator<<(std::ostream &os, const Complex& complex)
			{
				os << "C[" << complex.getReal() << ", " << complex.getImaginary() << "i]";
				return os;
			}
		}
	}
}

#endif