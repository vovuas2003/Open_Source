﻿#include <iostream>
#include "mpi.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
using namespace std;

struct LongDouble {
public:
	int divDigits = 1000;
	int sqrtDigits = 100;

	int sign;
	std::vector<int> digits;
	long exponent;

	void initFromString(const std::string& s);
	void removeZeroes();
	void normalize();

	int get_size() {
		return digits.size();
	}


	LongDouble();
	LongDouble(const LongDouble& x);
	LongDouble(long double value);
	LongDouble(const std::string& s);

	LongDouble& operator=(const LongDouble& x);

	bool operator>(const LongDouble& x) const;
	bool operator<(const LongDouble& x) const;
	bool operator>=(const LongDouble& x) const;
	bool operator<=(const LongDouble& x) const;
	bool operator==(const LongDouble& x) const;
	bool operator!=(const LongDouble& x) const;

	LongDouble operator-() const;

	LongDouble operator+(const LongDouble& x) const;
	LongDouble operator-(const LongDouble& x) const;
	LongDouble operator*(const LongDouble& x) const;
	LongDouble operator/(const LongDouble& x) const;

	LongDouble& operator+=(const LongDouble& x);
	LongDouble& operator-=(const LongDouble& x);
	LongDouble& operator*=(const LongDouble& x);
	LongDouble& operator/=(const LongDouble& x);

	LongDouble operator++(int);
	LongDouble operator--(int);

	LongDouble& operator++();
	LongDouble& operator--();

	LongDouble inverse() const;
	LongDouble sqrt() const;
	LongDouble pow(const LongDouble& n) const;
	LongDouble abs() const;

	bool isInteger() const;
	bool isEven() const;
	bool isOdd() const;
	bool isZero() const;

	friend std::ostream& operator<<(std::ostream& os, const LongDouble& value);
};


long long factorial(long long n) {
    long long res = 1;
    for (long long i = 1; i <= n; i++) {
        res = res * i;
    }
    return res;
}

int main(int argc, char *argv[]) {
	int numtasks, myrank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Status status;
	int nums = 1;
	nums = strtol(argv[1], NULL, 10);
	char* arr = new char[nums + 2]();
	arr[0] = '1';
	for (int i = 1; i < nums + 1; i++) {
		arr[i] = '0';
	}
	arr[nums + 1] = '\0';
	string str = string(arr);
	LongDouble eps(str);
	LongDouble exp(0);
	eps = eps.inverse();
	int n = 1;
	if (myrank == 0) {
		LongDouble one(1);
		while ((one > eps)) {
			n += 1;
			one = one / n;
		}
		n += 3;
	}
	LongDouble one(1);
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = myrank; i <= n; i += numtasks) {
		exp += one / factorial(i);
	}
	int size = 0;
	if (numtasks == 1) {
		for (int i = 0; i < exp.exponent; i++) {
			cout << exp.digits[i];
		}
		cout << '.';
		for (int i = exp.exponent; i < exp.exponent + nums;i++) {
			cout << exp.digits[i];
		}
		cout << endl;
	} else {
		if (myrank == 0) {
			for (int j = 1; j < numtasks;j++) {
				MPI_Recv(&size, 1, MPI_INT, j, 0, MPI_COMM_WORLD, &status);
				char* arr1 = new char[size];
				MPI_Recv(arr1, size, MPI_CHAR, j, 0, MPI_COMM_WORLD, &status);
				string str1 = string(arr1);
				LongDouble exp1(str1);
				exp += exp1;
			}
			for (int i = 0; i < exp.exponent; i++) {
				cout << exp.digits[i];
			}
			cout << '.';
			for (int i = exp.exponent; i < exp.exponent + nums;i++) {
				cout << exp.digits[i];
			}
			cout << endl;
		} else {
			size = exp.digits.size();
			size = size + 2;
			MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			char* arr1 = new char[size];
			for (int i = 0; i < exp.exponent; i++) {
				arr1[i] = (char)(exp.digits[i] + '0');
			}
			arr1[exp.exponent] = '.';
			for (int i = exp.exponent; i < exp.digits.size();i++) {
				arr1[i + 1] = (char)(exp.digits[i] + '0');
			}
			arr1[size - 1] = '\0';
			MPI_Send(arr1, size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			delete[] arr1;
		}
	}
	delete[] arr;
	MPI_Finalize();
        return 0;
}

void LongDouble::initFromString(const string& s) {
	size_t index;

	if (s[0] == '-') {
		sign = -1;
		index = 1;
	}
	else {
		sign = 1;
		index = 0;
	}

	exponent = s.length() - index;

	while (index < s.length()) {
		if (s[index] == '.')
			exponent = sign == 1 ? index : index - 1;
		else
			digits.push_back(s[index] - '0');

		index++;
	}
}

void LongDouble::removeZeroes() {
	size_t n = max((long)1, exponent);

	while (digits.size() > n && digits[digits.size() - 1] == 0)
		digits.erase(digits.end() - 1);

	while (digits.size() > 1 && digits[0] == 0) {
		digits.erase(digits.begin());
		exponent--;
	}

	while (digits.size() > 1 && digits[digits.size() - 1] == 0)
		digits.erase(digits.end() - 1);

	if (isZero()) {
		exponent = 1;
		sign = 1;
	}

	normalize();
}

void LongDouble::normalize() {
	size_t start = max(exponent, (long)0);
	size_t realDigits = digits.size() - start;

	if (realDigits >= divDigits) {
		size_t count = 0;
		size_t maxCount = 0;

		size_t i = start;

		while (i < digits.size()) {
			count = 0;

			while (i < digits.size() && digits[i] == 9) {
				count++;
				i++;
			}

			if (count > maxCount)
				maxCount = count;

			i++;
		}

		if (maxCount > divDigits * 4 / 5) {
			i = digits.size() - 1;

			do {
				count = 0;

				while (i > 0 && digits[i] != 9)
					i--;

				while (i > 0 && digits[i] == 9) {
					count++;
					i--;
				}
			} while (count != maxCount);

			digits.erase(digits.begin() + i + 1, digits.end());
			digits[i]++;
		}
	}
}

LongDouble::LongDouble() {
	sign = 1;
	digits = vector<int>(1, 0);
	exponent = 1;
}

LongDouble::LongDouble(const LongDouble& x) {
	sign = x.sign;
	exponent = x.exponent;
	digits = vector<int>(x.digits.size());

	for (size_t i = 0; i < x.digits.size(); i++)
		digits[i] = x.digits[i];
}

LongDouble::LongDouble(long double value) {
	stringstream ss;
	ss << setprecision(15) << value;

	initFromString(ss.str());
	removeZeroes();
}

LongDouble::LongDouble(const string& s) {
	initFromString(s);
	removeZeroes();
}

LongDouble& LongDouble::operator=(const LongDouble& x) {
	if (this == &x)
		return *this;

	sign = x.sign;
	exponent = x.exponent;
	digits = vector<int>(x.digits.size());

	for (size_t i = 0; i < x.digits.size(); i++)
		digits[i] = x.digits[i];

	return *this;
}

bool LongDouble::operator>(const LongDouble& x) const {
	if (sign != x.sign)
		return sign > x.sign;

	if (exponent != x.exponent)
		return (exponent > x.exponent) ^ (sign == -1);

	vector<int> d1(digits);
	vector<int> d2(x.digits);
	size_t size = max(d1.size(), d2.size());

	while (d1.size() != size)
		d1.push_back(0);

	while (d2.size() != size)
		d2.push_back(0);

	for (size_t i = 0; i < size; i++)
		if (d1[i] != d2[i])
			return (d1[i] > d2[i]) ^ (sign == -1);

	return false;
}

bool LongDouble::operator<(const LongDouble& x) const {
	return !(*this > x || *this == x);
}

bool LongDouble::operator>=(const LongDouble& x) const {
	return *this > x || *this == x;
}

bool LongDouble::operator<=(const LongDouble& x) const {
	return *this < x || *this == x;
}

bool LongDouble::operator==(const LongDouble& x) const {
	if (sign != x.sign)
		return false;

	if (exponent != x.exponent)
		return false;

	if (digits.size() != x.digits.size())
		return false;

	for (size_t i = 0; i < digits.size(); i++)
		if (digits[i] != x.digits[i])
			return false;

	return true;
}

bool LongDouble::operator!=(const LongDouble& x) const {
	return !(*this == x);
}

LongDouble LongDouble::operator-() const {
	LongDouble res(*this);
	res.sign = -sign;

	return res;
}

LongDouble LongDouble::operator+(const LongDouble& x) const {
	if (sign == x.sign) {
		long exp1 = exponent;
		long exp2 = x.exponent;
		long exp = max(exp1, exp2);

		vector<int> d1(digits);
		vector<int> d2(x.digits);

		while (exp1 != exp) {
			d1.insert(d1.begin(), 0);
			exp1++;
		}

		while (exp2 != exp) {
			d2.insert(d2.begin(), 0);
			exp2++;
		}

		size_t size = max(d1.size(), d2.size());

		while (d1.size() != size)
			d1.push_back(0);

		while (d2.size() != size)
			d2.push_back(0);

		size_t len = 1 + size;

		LongDouble res;

		res.sign = sign;
		res.digits = vector<int>(len, 0);

		for (size_t i = 0; i < size; i++)
			res.digits[i + 1] = d1[i] + d2[i];

		for (size_t i = len - 1; i > 0; i--) {
			res.digits[i - 1] += res.digits[i] / 10;
			res.digits[i] %= 10;
		}

		res.exponent = exp + 1;
		res.removeZeroes();

		return res;
	}

	if (sign == -1)
		return x - (-(*this));

	return *this - (-x);
}

LongDouble LongDouble::operator-(const LongDouble& x) const {
	if (sign == 1 && x.sign == 1) {
		bool cmp = *this > x;

		long exp1 = cmp ? exponent : x.exponent;
		long exp2 = cmp ? x.exponent : exponent;
		long exp = max(exp1, exp2);

		vector<int> d1(cmp ? digits : x.digits);
		vector<int> d2(cmp ? x.digits : digits);

		while (exp1 != exp) {
			d1.insert(d1.begin(), 0);
			exp1++;
		}

		while (exp2 != exp) {
			d2.insert(d2.begin(), 0);
			exp2++;
		}

		size_t size = max(d1.size(), d2.size());

		while (d1.size() != size)
			d1.push_back(0);

		while (d2.size() != size)
			d2.push_back(0);

		size_t len = 1 + size;

		LongDouble res;

		res.sign = cmp ? 1 : -1;
		res.digits = vector<int>(len, 0);

		for (size_t i = 0; i < size; i++)
			res.digits[i + 1] = d1[i] - d2[i];

		for (size_t i = len - 1; i > 0; i--) {
			if (res.digits[i] < 0) {
				res.digits[i] += 10;
				res.digits[i - 1]--;
			}
		}

		res.exponent = exp + 1;
		res.removeZeroes();

		return res;
	}

	if (sign == -1 && x.sign == -1)
		return (-x) - (-(*this));

	return *this + (-x);
}

LongDouble LongDouble::operator*(const LongDouble& x) const {
	size_t len = digits.size() + x.digits.size();

	LongDouble res;

	res.sign = sign * x.sign;
	res.digits = vector<int>(len, 0);
	res.exponent = exponent + x.exponent;

	for (size_t i = 0; i < digits.size(); i++)
		for (size_t j = 0; j < x.digits.size(); j++)
			res.digits[i + j + 1] += digits[i] * x.digits[j];

	for (size_t i = len - 1; i > 0; i--) {
		res.digits[i - 1] += res.digits[i] / 10;
		res.digits[i] %= 10;
	}

	res.removeZeroes();

	return res;
}

LongDouble LongDouble::operator/(const LongDouble& x) const {
	LongDouble res = *this * x.inverse();

	size_t intPart = max((long)0, exponent);

	if (intPart > res.digits.size() - 1)
		return res;

	size_t i = res.digits.size() - 1 - intPart;
	size_t n = max((long)0, res.exponent);

	if (i > n && res.digits[i] == 9) {
		while (i > n && res.digits[i] == 9)
			i--;

		if (res.digits[i] == 9) {
			res.digits.erase(res.digits.begin() + n, res.digits.end());
			res = res + res.sign;
		}
		else {
			res.digits.erase(res.digits.begin() + i + 1, res.digits.end());
			res.digits[i]++;
		}
	}

	return res;
}

LongDouble& LongDouble::operator+=(const LongDouble& x) {
	return (*this = *this + x);
}

LongDouble& LongDouble::operator-=(const LongDouble& x) {
	return (*this = *this - x);
}

LongDouble& LongDouble::operator*=(const LongDouble& x) {
	return (*this = *this * x);
}

LongDouble& LongDouble::operator/=(const LongDouble& x) {
	return (*this = *this / x);
}

LongDouble LongDouble::operator++(int) {
	LongDouble res(*this);
	*this = *this + 1;

	return res;
}

LongDouble LongDouble::operator--(int) {
	LongDouble res(*this);
	*this = *this - 1;

	return res;
}

LongDouble& LongDouble::operator++() {
	return (*this = *this + 1);
}

LongDouble& LongDouble::operator--() {
	return (*this = *this - 1);
}

LongDouble LongDouble::inverse() const {
	if (isZero())
		throw string("LongDouble LongDouble::inverse() - division by zero!");

	LongDouble x(*this);
	x.sign = 1;

	LongDouble d("1");

	LongDouble res;
	res.sign = sign;
	res.exponent = 1;
	res.digits = vector<int>();

	while (x < 1) {
		x.exponent++;
		res.exponent++;
	}

	while (d < x)
		d.exponent++;

	res.exponent -= d.exponent - 1;

	size_t numbers = 0;
	size_t intPart = max((long)0, res.exponent);
	size_t maxNumbers = divDigits + intPart;

	do {
		int div = 0;

		while (d >= x) {
			div++;
			d -= x;
		}

		d.exponent++;
		d.removeZeroes();

		res.digits.push_back(div);
		numbers++;
	} while (!d.isZero() && numbers < maxNumbers);

	return res;
}

LongDouble LongDouble::sqrt() const {
	if (sign == -1)
		throw string("LongDouble LongDouble::sqrt() - number is negative");

	if (isZero())
		return 0;

	LongDouble x0;
	LongDouble p("0.5");
	LongDouble xk("0.5");
	LongDouble eps;
	eps.digits = vector<int>(1, 1);
	eps.exponent = 1 - sqrtDigits;

	do {
		x0 = xk;
		xk = p * (x0 + *this / x0);
	} while ((x0 - xk).abs() > eps);

	xk.digits.erase(xk.digits.begin() + max((long)0, xk.exponent) + sqrtDigits, xk.digits.end());
	xk.removeZeroes();

	return xk;
}

LongDouble LongDouble::pow(const LongDouble& n) const {
	if (!n.isInteger())
		throw string("LongDouble LongDouble::power(const LongDouble& n) - n is not integer!");

	LongDouble res("1");
	LongDouble a = n.sign == 1 ? *this : this->inverse();
	LongDouble power = n.abs();

	while (power > 0) {
		if (power.isOdd())
			res *= a;

		a *= a;
		power /= 2;

		if (!power.isInteger())
			power.digits.erase(power.digits.end() - 1);
	}

	return res;
}

LongDouble LongDouble::abs() const {
	LongDouble res(*this);
	res.sign = 1;

	return res;
}

bool LongDouble::isInteger() const {
	if (exponent < 0)
		return false;

	return digits.size() <= (size_t)exponent;
}

bool LongDouble::isEven() const {
	if (!isInteger())
		return false;

	if (digits.size() == (size_t)exponent)
		return digits[digits.size() - 1] % 2 == 0;

	return true;
}

bool LongDouble::isOdd() const {
	if (!isInteger())
		return false;

	if (digits.size() == (size_t)exponent)
		return digits[digits.size() - 1] % 2 == 1;

	return false;
}

bool LongDouble::isZero() const {
	return digits.size() == 1 && digits[0] == 0;
}

ostream& operator<<(ostream& os, const LongDouble& value) {
	if (value.sign == -1)
		os << '-';

	if (value.exponent > 0) {
		size_t i = 0;
		size_t e = value.exponent;

		while (i < value.digits.size() && i < e)
			os << value.digits[i++];

		while (i < e) {
			os << "0";
			i++;
		}

		if (i < value.digits.size()) {
			os << ".";

			while (i < value.digits.size())
				os << value.digits[i++];
		}
	}
	else if (value.exponent == 0) {
		os << "0.";

		for (size_t i = 0; i < value.digits.size(); i++)
			os << value.digits[i];
	}
	else {
		os << "0.";

		for (long i = 0; i < -value.exponent; i++)
			os << "0";

		for (size_t i = 0; i < value.digits.size(); i++)
			os << value.digits[i];
	}

	return os;
}
