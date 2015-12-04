// cpp1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cassert>
#include <forward_list>

#include <set>

#include <iostream>
using namespace std;

enum COMPARE
{
	SMALLER = -1,
	EQUAL = 0,
	GREATER = 1
};

class FareyNumber
{
	//divident/divisor
public:
	FareyNumber(unsigned int divident, unsigned int divisor) : _divident(divident), _divisor(divisor)
	{
		assert(_divisor != 0);
		value = (double)_divident / _divisor;
	}
	//-1 if smaller than num; 0 if equal; 1 if greater
	bool IsSmallerThan(FareyNumber& num)
	{
		return (num.GetValue() > value);
	}

	double GetValue() { return value; };
    unsigned int GetDivident() { return _divident; };
    unsigned int GetDivisor() { return _divisor; };
private:
	unsigned int _divident; //_divident
	unsigned int _divisor; //_divisor
	double value;
};

// add the FareyNumber in the sorted list
// with moving the iterator for the next number
void addFareyNumber(forward_list<FareyNumber>& mylist, FareyNumber fNum, forward_list<FareyNumber>::iterator& it)
{
	forward_list<FareyNumber>::iterator itnext = it;

	for (++itnext; itnext != mylist.end(); ++itnext)
	{
		if (fNum.IsSmallerThan(*itnext))
		{
			it = mylist.insert_after(it, fNum);
			break;
		}
		else
		{
			++it;
		}
	}
}

// return a set of prime numbers that can divide num. excluding 1 and num
// primeNums are all prime numbers < num
set<unsigned int> getPrimeDivisors(unsigned int num, set<unsigned int>& primeNums)
{
    set<unsigned int> primeDivisors;

    for (set<unsigned int>::iterator it = primeNums.begin(); it != primeNums.end(); ++it)
    {
        if( num % *it == 0)
            primeDivisors.insert(*it);
    }

    return primeDivisors;
}

// num can be divided by some prime number in primeDivisors
bool canBeDivided(unsigned int num, set<unsigned int>& primeDivisors)
{
  for (set<unsigned int>::iterator it = primeDivisors.begin(); it != primeDivisors.end(); ++it)
  {
      if( num % *it == 0)
      return true;
  }

  return false;
}

void generateFareySeq(unsigned int N)
{
	assert(N != 0);
	forward_list<FareyNumber> mylist;

	//1. add the first and last - 0/0 and 1/1
	mylist.insert_after(mylist.before_begin(), FareyNumber(1, 1));
	mylist.insert_after(mylist.before_begin(), FareyNumber(0, 1));


    set<unsigned int> primeNums;

    //generate all FareyNumbers and place them in ordered list
	for (unsigned int i = 2; i <= N; i++)
	{
        set<unsigned int> primeDivisors = getPrimeDivisors(i, primeNums);
        if(primeDivisors.size() == 0)
        {
            // i is a prime number
            primeNums.insert(i);
        }

        // iterator from where to try to place the next FareyNumber
        // reset for every new i
		forward_list<FareyNumber>::iterator it = mylist.begin();
		for (unsigned int j = 1; j < i; j++)
		{
			//  j/i
            // if both can be divided by some prime number, it was added already:
            // 2/10 was added as 1/5
            if(canBeDivided(j, primeDivisors))
            {
                continue;
            }

            //add the FareyNumber
			FareyNumber num(j, i);
			addFareyNumber(mylist, num, it);
		}
	}

	cout << endl;

	for (forward_list<FareyNumber>::iterator it = mylist.begin(); it != mylist.end(); ++it)
		cout << (*it).GetDivident() << '/' << (*it).GetDivisor() << ' ';
    cout << endl;

    //for (set<unsigned int>::iterator it = primeNums.begin(); it != primeNums.end(); ++it)
    //    cout << *it << ' ';
    //cout << endl;
};


void fareyWiki(unsigned int n) 
{
	FareyNumber f1(0, 1), f2(1, n);
    cout << f1.GetDivident() << '/' << f1.GetDivisor() << ' ';
    cout << f2.GetDivident() << '/' << f2.GetDivisor() << ' ';

    while (f2.GetDivisor() > 1) 
    {
		unsigned int k = (n + f1.GetDivisor()) / f2.GetDivisor();
		FareyNumber aux = f1;
		f1 = f2;
		f2 = FareyNumber(f2.GetDivident() * k - aux.GetDivident(), f2.GetDivisor() * k - aux.GetDivisor());
        cout << f2.GetDivident() << '/' << f2.GetDivisor() << ' ';
	}

	cout << '\n';
}

int _tmain(int argc, _TCHAR* argv[])
{
    unsigned int N;
    cout << "\nEnter number N for calculating FareyNumber(N): ";
    cin >> N;

    //calculating O(n^2)
	generateFareySeq(N);
    
    //the fast algorithm from wikipedia
    //fareyWiki(N);

	return 0;
}

