// http://www.codeproject.com/KB/cpp/largenumber.aspx
//LargeNumber.h
#ifndef __LARGENUMBER_H__
#define __LARGENUMBER_H__

#include <vector>
#include <string>

using namespace std;

class CLargeNumber
{
public:
	//Default Constructor
	CLargeNumber() : m_cSign(1) { m_oNumber.push_back(0); }

	//Copy Constructor - Default OK
	//CLargeNumber(CLargeNumber const& rcoLargeNumber) : m_cSign(rcoLargeNumber.m_cSign), m_oNumber(rcoLargeNumber.m_oNumber) {}

	//Constructor From a Number
	CLargeNumber(int iNumber);
	//Constructor From a String
	CLargeNumber(string const& rostrNumber);
	//Constructor From Members
	CLargeNumber(char cSign, vector<char> const& rcoNumber) : m_cSign(cSign), m_oNumber(rcoNumber) {}

	//Assignment Operator - Default OK
	//CLargeNumber& operator=(CLargeNumber const& roLN);

protected:
	//Auxiliary class Functions:
	//Build from unsigned long
	static void Build(unsigned uN, vector<char>& rvN);
	//Build from string
	static void Build(string const& rostrNumber, vector<char>& rvN);
	//Cleaning
	static void Clean(vector<char>& rvN);
	//Comparison Function
	static int Compare(vector<char> const& rcvN1, vector<char> const& rcvN2);
	//Addition
	static void Add(vector<char> const& rcvN1, vector<char> const& rcvN2, vector<char>& rvNRes);
	//Subtraction
	static void Subtract(vector<char> const& rcvN1, vector<char> const& rcvN2, vector<char>& rvNRes);
	//Product with one digit
	static void Multiply(vector<char> const& rcvN, char c, vector<char>& rvNRes);
	//Shift Left
	static void ShiftLeft(vector<char>& rvN, int iLeft);
	//Multiplication
	static void Multiply(vector<char> const& rcvN1, vector<char> const& rcvN2, vector<char>& rvNRes);
	//Get the Position of the most significant Digit
	static int Position(vector<char> const& rcvN);
	//Compute a Power of 10
	static void Pow10(unsigned uPow, vector<char>& rvNRes);
	//Division
	static void Divide(vector<char> const& rcvN1, vector<char> const& rcvN2, vector<char>& rvQ, vector<char>& rvR);

public:
	//Transform to a string
	string ToString() const;
	//Operators
	//Equality Operator
	bool operator==(CLargeNumber const& roLN);
	//Inequality Operator
	bool operator!=(CLargeNumber const& roLN);
	CLargeNumber& operator-();
	bool operator<(CLargeNumber const& roLN) const;
	bool operator>(CLargeNumber const& roLN) const;
	bool operator<=(CLargeNumber const& roLN) const;
	bool operator>=(CLargeNumber const& roLN) const;	
	CLargeNumber operator+(CLargeNumber const& roLN) const;
	CLargeNumber operator-(CLargeNumber const& roLN) const;
	CLargeNumber operator*(CLargeNumber const& roLN) const;
	CLargeNumber operator/(CLargeNumber const& roLN) const;
	CLargeNumber operator%(CLargeNumber const& roLN) const;
	CLargeNumber& operator+=(CLargeNumber const& roLN);
	CLargeNumber& operator-=(CLargeNumber const& roLN);
	CLargeNumber& operator*=(CLargeNumber const& roLN);
	CLargeNumber& operator/=(CLargeNumber const& roLN);
	CLargeNumber& operator%=(CLargeNumber const& roLN);
	//Convertion operator
	operator int() const;
	//Square Root
	CLargeNumber SquareRoot() const;

private:
	//-1 - Negative, +1 - Positive or zero
	char m_cSign;
	vector<char> m_oNumber;
};

#endif //__LARGENUMBER_H__

