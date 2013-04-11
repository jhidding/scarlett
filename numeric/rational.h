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
