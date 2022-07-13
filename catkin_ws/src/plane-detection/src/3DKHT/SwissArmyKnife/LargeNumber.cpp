#include "LargeNumber.h"
#include <stdexcept>
#include <limits.h>

//=====================================================================
//Auxiliary Functions:
//Build from unsigned long
void CLargeNumber::Build(unsigned uN, vector<char>& rvN)
{
	rvN.clear();
	if(0 == uN)
		rvN.push_back(0);
	else
		for( ;uN>0; ) {
			rvN.push_back(uN % 10);
			uN /= 10;
		}
}

//Build from string
void CLargeNumber::Build(string const& rostrNumber, vector<char>& rvN)
{
	rvN.clear();
	for(int i=rostrNumber.size()-1; i>=0; i--) {
		if(rostrNumber[i]<'0' || rostrNumber[i]>'9')
			break;
		else
			rvN.push_back(rostrNumber[i]-'0');
	}
	Clean(rvN);
}

//Cleaning
void CLargeNumber::Clean(vector<char>& rvN)
{
	//Eliminate all leading 0s
	vector<char>::iterator it = rvN.end();
	while (it != rvN.begin()) {
		it--;
		if (*it != 0)
			break;
	}
	rvN.erase(it+1, rvN.end());
}

//Comparison Function
int CLargeNumber::Compare(vector<char> const& rcvN1, vector<char> const& rcvN2)
{
	if(rcvN1.size() == rcvN2.size()) {
		for(int i=rcvN1.size()-1; i>=0; i--) {
			if(rcvN1[i] == rcvN2[i])
				continue;
			return (rcvN1[i] < rcvN2[i]) ? -1 : 1;
		}
		return 0; //are equal
	}
	else
		return (rcvN1.size() < rcvN2.size()) ? -1 : 1;
}

//Addition
void CLargeNumber::Add(vector<char> const& rcvN1, vector<char> const& rcvN2, vector<char>& rvNRes)
{
	rvNRes.clear();
	//Local copies
	vector<char> vN1 = rcvN1;
	vector<char> vN2 = rcvN2;
	int iSize1 = vN1.size();
	int iSize2 = vN2.size();
	int i, iSize;
	//Fill with '0'
	if(iSize1 > iSize2) {
		for(i=iSize2; i<iSize1; i++)
			vN2.push_back(0);
		iSize = iSize1;
	}
	else {
		for(i=iSize1; i<iSize2; i++)
			vN1.push_back(0);
		iSize = iSize2;
	}
	int iC=0, iR;
	for(i=0; i<iSize; i++)
	{
		iR = vN1[i] + vN2[i] + iC;
		if(iR > 9) {
			iR -= 10;
			iC = 1;
		}
		else
			iC = 0;
		rvNRes.push_back(iR);
	}
	if(iC > 0)
		rvNRes.push_back(iC);
}

//Subtraction
void CLargeNumber::Subtract(vector<char> const& rcvN1, vector<char> const& rcvN2, vector<char>& rvNRes)
{
	rvNRes.clear();
	//Local copy
	vector<char> vN1 = rcvN1;
    unsigned i;
	for(i=0; i<rcvN2.size(); i++) {
		if(rcvN2[i] > vN1[i]) {
			vN1[i] += 10;
			for(unsigned j=i+1; j<vN1.size(); j++) {
				if(vN1[j] > 0) {
					vN1[j]--;
					break;
				}
				else
					vN1[j] = 9;
			}
			vector<char>::iterator it = vN1.end();
			it--;
			if(0 == *it)
				vN1.erase(it);
		}
		rvNRes.push_back(vN1[i]-rcvN2[i]);
	}
	for( ;i<vN1.size(); i++)
		rvNRes.push_back(vN1[i]);
	Clean(rvNRes);
}

//Product with one digit
void CLargeNumber::Multiply(vector<char> const& rcvN, char c, vector<char>& rvNRes)
{
	rvNRes.clear();
	char t, s;
	t = 0;
	if(0 == c)
		rvNRes.push_back(0);
	else {
		for(unsigned i=0; i<rcvN.size(); i++) {
			(s = c * rcvN[i] + t) > 9 ? (t = s/10, s %= 10) : (t = 0);
			rvNRes.push_back(s);
		}
		if(t>0)
			rvNRes.push_back(t);
	}
}

//Shift Left
void CLargeNumber::ShiftLeft(vector<char>& rvN, int iLeft)
{
	vector<char>::iterator i;
	for(int j=0; j<iLeft; j++)
	{
		i = rvN.begin();
		rvN.insert(i, 0);
	}
}

//Multiplication
void CLargeNumber::Multiply(vector<char> const& rcvN1, vector<char> const& rcvN2, vector<char>& rvNRes)
{
	vector<char> vZero;
	Build("0", vZero);
	if( 0 == Compare(rcvN1, vZero) || 0 == Compare(rcvN2, vZero)) {
		rvNRes = vZero;
		return;
	}
	rvNRes.clear();
	vector<char> vDummy;
	for(unsigned i=0; i<rcvN2.size(); i++)
		if(rcvN2[i] > 0) {
			Multiply(rcvN1, rcvN2[i], vDummy);
			ShiftLeft(vDummy, i);
			if(0 == i)
				rvNRes = vDummy;
			else {
				vector<char> vDummy1;
				Add(rvNRes, vDummy, vDummy1);
				rvNRes = vDummy1;
			}
		}
}

//Get the Position of the most significant Digit
int CLargeNumber::Position(vector<char> const& rcvN)
{
	int iPos = rcvN.size();
	if(0 == iPos)
		iPos = 1;
	return iPos;
}

//Compute a Power of 10
void CLargeNumber::Pow10(unsigned uPow, vector<char>& rvNRes)
{
	rvNRes.clear();
	rvNRes.push_back(1);
	vector<char>::iterator i;
	for(unsigned j=0; j<uPow; j++) {
		i = rvNRes.begin();
		rvNRes.insert(i, 0);
	}
}

//Division
void CLargeNumber::Divide(vector<char> const& rcvN1, vector<char> const& rcvN2, vector<char>& rvQ, vector<char>& rvR)
{
	rvQ.clear();
	rvR.clear();
	if(Compare(rcvN1, rcvN2) >= 0) {
		vector<char> vDummy;
		vector<char> vTmp = rcvN2;
		int iPos = Position(rcvN1)-Position(rcvN2)-1;
		vector<char> vIncr;
		if(iPos > 0) {
			ShiftLeft(vTmp, iPos);
			Pow10(iPos, vIncr);
		}
		else
			Build(1, vIncr);
		rvR = rcvN1;
		Build(unsigned(0), rvQ);
		while(Compare(rvR, vTmp) >= 0) {
			Add(rvQ, vIncr, vDummy);
			rvQ = vDummy;
			Subtract(rvR, vTmp, vDummy);
			rvR = vDummy;
		}
		if(Compare(rvR, rcvN2) >= 0) {
			vector<char> vQ;
			vector<char> vR;
			Divide(rvR, rcvN2, vQ, vR);
			rvR = vR;
			Add(rvQ, vQ, vDummy);
			rvQ = vDummy;
		}
	}
	else
	{
		Build(unsigned(0), rvQ);
		rvR = rcvN1;
	}
}

//=====================================================================

//Constructor From a Number
CLargeNumber::CLargeNumber(int iNumber)
{
	if(iNumber < 0) {
		m_cSign = -1;
		iNumber = -iNumber;
	}
	else
		m_cSign = 1;
	Build(iNumber, m_oNumber);
}

//Constructor From a String
CLargeNumber::CLargeNumber(string const& rostrNumber)
{
	//Eliminating leading and trailing blanks
	int iEnd;
	iEnd = rostrNumber.size()-1;
	for(; (rostrNumber[iEnd]==' ')&&(iEnd>=0); iEnd--)
		;
	if(iEnd < 0) {
		m_cSign = 1;
		Build(0, m_oNumber);
		return;
	}
	int iBeg;
	for(iBeg=0; ' '==rostrNumber[iBeg]; iBeg++)
		;
	string ostrNumber = rostrNumber.substr(iBeg, iEnd-iBeg+1);
	iBeg = 0;
	if('-' == ostrNumber[0]) {
		m_cSign = -1;
		iBeg = 1;
	}
	else
		m_cSign = 1;
	Build(ostrNumber.c_str()+iBeg, m_oNumber);
}

//Transform to a string
string CLargeNumber::ToString() const
{
	if(0 == m_oNumber.size())
		return "0";
	string ostr;
	if(-1 == m_cSign)
		ostr += '-';
	vector<char>::const_reverse_iterator rIter = m_oNumber.rbegin();
	for(; rIter != m_oNumber.rend(); rIter++)
		ostr += *rIter+'0';
	return ostr;
}

//Equality Operator
bool CLargeNumber::operator==(CLargeNumber const& roLN)
{
	if(m_oNumber==roLN.m_oNumber && m_cSign==roLN.m_cSign)
		return true;
	else
		return false;
}

//Inequality Operator
bool CLargeNumber::operator!=(CLargeNumber const& roLN)
{
	return !(operator==(roLN));
}

CLargeNumber& CLargeNumber::operator-()
{
	m_cSign = -m_cSign;
	return *this;
}

bool CLargeNumber::operator<(CLargeNumber const& roLN) const
{
	if(-1==m_cSign && 1==roLN.m_cSign)
		return true;
	else if(1==m_cSign && -1==roLN.m_cSign)
		return false;
	else if(-1==m_cSign && -1==roLN.m_cSign)
		return (-1==Compare(roLN.m_oNumber, m_oNumber));
	else //if(1==m_cSign && 1==roLN.m_cSign)
		return (-1==Compare(m_oNumber, roLN.m_oNumber));
}

bool CLargeNumber::operator>(CLargeNumber const& roLN) const
{
	if(-1==m_cSign && 1==roLN.m_cSign)
		return false;
	else if(1==m_cSign && -1==roLN.m_cSign)
		return true;
	else if(-1==m_cSign && -1==roLN.m_cSign)
		return (-1==Compare(m_oNumber, roLN.m_oNumber));
	else //if(1==m_cSign && 1==roLN.m_cSign)
		return (-1==Compare(roLN.m_oNumber, m_oNumber));
}

bool CLargeNumber::operator<=(CLargeNumber const& roLN) const
{
	return !operator>(roLN);
}

bool CLargeNumber::operator>=(CLargeNumber const& roLN) const
{
	return !operator<(roLN);
}

CLargeNumber CLargeNumber::operator+(CLargeNumber const& roLN) const
{
	CLargeNumber oLNRes;
	if(1 == m_cSign && 1 == roLN.m_cSign) {
		oLNRes.m_cSign = 1;
		Add(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
	}
	else if(-1 == m_cSign && -1 == roLN.m_cSign) {
		oLNRes.m_cSign = -1;
		Add(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
	}
	else {
		int iComp = Compare(m_oNumber, roLN.m_oNumber);
		if(0 == iComp)
			return CLargeNumber(0L);
		else if(-1 == iComp)
			Subtract(roLN.m_oNumber, m_oNumber, oLNRes.m_oNumber);
		else
			Subtract(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
		if(1 == m_cSign && -1 == roLN.m_cSign) {
			if(-1 == iComp)
				oLNRes.m_cSign = -1;
			else
				oLNRes.m_cSign = 1;
		}
		else {
			if(-1 == iComp)
				oLNRes.m_cSign = 1;
			else
				oLNRes.m_cSign = -1;
		}
	}
	return oLNRes;
}

CLargeNumber CLargeNumber::operator-(CLargeNumber const& roLN) const
{
	CLargeNumber oLNRes;
	if(1 == m_cSign && -1 == roLN.m_cSign) {
		oLNRes.m_cSign = 1;
		Add(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
	}
	else if(-1 == m_cSign && 1 == roLN.m_cSign) {
		oLNRes.m_cSign = -1;
		Add(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
	}
	else {
		int iComp = Compare(m_oNumber, roLN.m_oNumber);
		if(0 == iComp) 
			return CLargeNumber(0);
		else if(-1 == iComp)
			Subtract(roLN.m_oNumber, m_oNumber, oLNRes.m_oNumber);
		else
			Subtract(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
		if(1 == m_cSign && 1 == roLN.m_cSign) {
			if(-1 == iComp)
				oLNRes.m_cSign = -1;
			else
				oLNRes.m_cSign = 1;
		}
		else {
			if(-1 == iComp)
				oLNRes.m_cSign = 1;
			else
				oLNRes.m_cSign = -1;
		}
	}
	return oLNRes;
}

CLargeNumber CLargeNumber::operator*(CLargeNumber const& roLN) const
{
	CLargeNumber oLNRes;
	oLNRes.m_cSign = m_cSign * roLN.m_cSign;
	Multiply(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
	return oLNRes;
}

CLargeNumber CLargeNumber::operator/(CLargeNumber const& roLN) const
{
	if(roLN < CLargeNumber("0"))
		//Throw an exception
		throw overflow_error("ERROR: Overflow in operator/!");
	if(0==Compare(m_oNumber, CLargeNumber("0").m_oNumber))
		return CLargeNumber("0");
	CLargeNumber oLNQ, oLNDummy;
	Divide(m_oNumber, roLN.m_oNumber, oLNQ.m_oNumber, oLNDummy.m_oNumber);
	oLNQ.m_cSign = ((-1==m_cSign)&&(-1==roLN.m_cSign)) || ((1==m_cSign)&&(1==roLN.m_cSign)) ? 1 : -1;
	return oLNQ;
}

CLargeNumber CLargeNumber::operator%(CLargeNumber const& roLN) const
{
	if(roLN < CLargeNumber("0"))
		//Throw an exception
		throw overflow_error("ERROR: Overflow in operator%!");
	if(0==Compare(m_oNumber, CLargeNumber("0").m_oNumber))
		return CLargeNumber("0");
	CLargeNumber oLNDummy, oLNR;
	Divide(m_oNumber, roLN.m_oNumber, oLNDummy.m_oNumber, oLNR.m_oNumber);
	oLNR.m_cSign = m_cSign;
	return oLNR;
}

CLargeNumber& CLargeNumber::operator+=(CLargeNumber const& roLN)
{
	CLargeNumber oLNRes;
	if(1 == m_cSign && 1 == roLN.m_cSign) {
		m_cSign = 1;
		Add(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
	}
	else if(-1 == m_cSign && -1 == roLN.m_cSign) {
		m_cSign = -1;
		Add(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
	}
	else {
		int iComp = Compare(m_oNumber, roLN.m_oNumber);
		if(0 == iComp) {
			*this = CLargeNumber(0);
			return *this;
		}
		else if(-1 == iComp)
			Subtract(roLN.m_oNumber, m_oNumber, oLNRes.m_oNumber);
		else
			Subtract(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
		if(1 == m_cSign && -1 == roLN.m_cSign) {
			if(-1 == iComp)
				m_cSign = -1;
			else
				m_cSign = 1;
		}
		else {
			if(-1 == iComp)
				m_cSign = 1;
			else
				m_cSign = -1;
		}
	}
	m_oNumber = oLNRes.m_oNumber;
	return *this;
}

CLargeNumber& CLargeNumber::operator-=(CLargeNumber const& roLN)
{
	CLargeNumber oLNRes;
	if(1 == m_cSign && -1 == roLN.m_cSign) {
		m_cSign = 1;
		Add(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
	}
	else if(-1 == m_cSign && 1 == roLN.m_cSign) {
		m_cSign = -1;
		Add(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
	}
	else {
		int iComp = Compare(m_oNumber, roLN.m_oNumber);
		if(0 == iComp) {
			*this = CLargeNumber(0);
			return *this;
		}
		else if(-1 == iComp)
			Subtract(roLN.m_oNumber, m_oNumber, oLNRes.m_oNumber);
		else
			Subtract(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
		if(1 == m_cSign && 1 == roLN.m_cSign) {
			if(-1 == iComp)
				m_cSign = -1;
			else
				m_cSign = 1;
		}
		else {
			if(-1 == iComp)
				m_cSign = 1;
			else
				m_cSign = -1;
		}
	}
	m_oNumber = oLNRes.m_oNumber;
	return *this;
}

CLargeNumber& CLargeNumber::operator*=(CLargeNumber const& roLN)
{
	CLargeNumber oLNRes;
	m_cSign = m_cSign * roLN.m_cSign;
	Multiply(m_oNumber, roLN.m_oNumber, oLNRes.m_oNumber);
	*this = oLNRes;
	return *this;
}
	
CLargeNumber& CLargeNumber::operator/=(CLargeNumber const& roLN)
{
	if(roLN<CLargeNumber("0"))
		//Throw an exception
		throw overflow_error("ERROR: Overflow in operator /=!");
	if(0==Compare(m_oNumber, CLargeNumber("0").m_oNumber))
		*this = CLargeNumber("0");
	else {
		CLargeNumber oLNQ, oLNDummy;
		Divide(m_oNumber, roLN.m_oNumber, oLNQ.m_oNumber, oLNDummy.m_oNumber);
		oLNQ.m_cSign = ((-1==m_cSign)&&(-1==roLN.m_cSign)) || ((1==m_cSign)&&(1==roLN.m_cSign)) ? 1 : -1;
		*this = oLNQ;
	}
	return *this;
}

CLargeNumber& CLargeNumber::operator%=(CLargeNumber const& roLN)
{
	if(roLN<CLargeNumber("0"))
		//Throw an exception
		throw overflow_error("ERROR: Overflow in operator%=!");
	if(0==Compare(m_oNumber, CLargeNumber("0").m_oNumber))
		*this = CLargeNumber("0");
	else {
		CLargeNumber oLNDummy, oLNR;
		Divide(m_oNumber, roLN.m_oNumber, oLNDummy.m_oNumber, oLNR.m_oNumber);
		oLNR.m_cSign = m_cSign;
		*this = oLNR;
	}
	return *this;
}

//Convertion operator
CLargeNumber::operator int() const
{
	CLargeNumber oLNMin(INT_MIN);
	CLargeNumber oLNMax(INT_MAX);
	if(operator<(oLNMin) || operator>(oLNMax))
		//Throw an exception
		throw domain_error("ERROR: Domain in operator int()!");
	unsigned uPow10 = 1;
	int iRes = 0;
	if(-1==m_cSign) {
		for(unsigned i=0; i<m_oNumber.size(); i++) {
			iRes -= m_oNumber[i]*uPow10;
			uPow10 *= 10;
		}
	}
	else {
		for(unsigned i=0; i<m_oNumber.size(); i++) {
			iRes += m_oNumber[i]*uPow10;
			uPow10 *= 10;
		}
	}
	return iRes;
}

//Square Root
CLargeNumber CLargeNumber::SquareRoot() const
{
	if(operator<(CLargeNumber("0")))
		//Throw an exception
		throw invalid_argument("ERROR: Negative Number in function SquareRoot()!");
	//Initialize	
	CLargeNumber oNumber1 = *this;
	CLargeNumber oLN0(0);
	CLargeNumber oLN2(2);
	CLargeNumber oLN10(10);
	CLargeNumber oLN100(100);
	CLargeNumber oSqR(oLN10);
	while((oNumber1 /= oLN100) > oLN0)
	  oSqR *= oLN10;
	//Recursive Algorithm
	CLargeNumber oSqROld;
	while(true) {
		oSqROld = oSqR;
		oSqR = (oSqR + (*this)/oSqR)/oLN2;
		if(oSqR >= oSqROld) {
			oSqR = oSqROld;
			return oSqR;
		}
	}
}

//=====================================================================

