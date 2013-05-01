/*  This file is part of Scarlett.

    Scarlett is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Scarlett is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Scarlett.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

namespace Numeric
{
	class rational
	{
		int a, b;

		public:
			rational(int a_, int b_):
				a(a_), b(b_) {}

			int denominator() const { return b; }
			int numerator() const { return a; }
		
			// should find gcd and put sign in <a> always.
			void simplify() {}

			rational &operator+=(rational const &o)
			{
				a = a * o.b + o.a * b;
				b = o.b * b;
				simplify();
				return *this;
			}

			rational &operator-=(rational const &o)
			{
				a = a * o.b - o.a * b;
				b = o.b * b;
				simplify();
				return *this;
			}

			rational &operator*=(rational const &o)
			{
				a *= o.a;
				b *= o.b;
				simplify();
				return *this;
			}

			rational &operator/=(rational const &o)
			{
				a *= o.b;
				b *= o.a;
				simplify();
				return *this;
			}

			rational operator+(rational const &o) const
				{ rational a(*this); return a += o; }
			rational operator-(rational const &o) const
				{ rational a(*this); return a -= o; }
			rational operator*(rational const &o) const
				{ rational a(*this); return a *= o; }
			rational operator/(rational const &o) const
				{ rational a(*this); return a /= o; }

			operator double() const { return double(a) / b; }
	};

	inline std::ostream &operator<<(std::ostream &out, rational q)
	{
		out << q.numerator() << "/" << q.denominator();
		return out;
	}
}

// vim:sw=4:ts=4:tw=72
