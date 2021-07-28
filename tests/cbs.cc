
#include <assert.h>
#include "../boost.h"
#include "../cbset.h"

template<class T, class...>
// class myset : public cbset::BSET_DYNAMIC<4>{};
using myset=cbset::BSET_DYNAMIC<4>;

using cbset::BSET_DYNAMIC;

void test0()
{
	BSET_DYNAMIC<2> S, T;
	assert(S.size()==0);

	S.insert(8);
	assert(S.size()==1);
	trace1("+8", S);

	S.insert(0);
	trace1("+0", S);
	assert(S.size()==2);

	T.insert(4);
	assert(!T.is_subset_of(S));

	S.insert(4);

	trace1("test0", S);
	trace1("test0", T);

	assert(T.is_subset_of(S));
	S.insert(4);
	trace1("+4", S);
	assert(S.size()==3);

	S.erase(4);
	trace1("test0", S);
	trace1("test0", T);
	assert(S.size()==2);
}

void test1()
{
	BSET_DYNAMIC<2> S, T;

	assert(S.compare_int(T) == 0);
	S.insert(4);
	trace1("test1 4", S);
	trace1("test1", T);
	assert(T.compare_int(S) < 0);
	assert(S.compare_int(T) > 0);
	T.insert(5);
	trace1("test1", S);
	trace1("test1", T);
	assert(S.compare_int(T) < 0);
	assert(T.compare_int(S) > 0);
	S.insert(9);
	trace1("test1", S);
	trace1("test1", T);
	assert(S.compare_int(T) > 0);
	assert(T.compare_int(S) < 0);

}

void test2()
{
	BSET_DYNAMIC<4> S, T;

	S.insert(9);
	trace1("test2", S);
	trace1("test2", T);
	assert(S.compare_int(T) > 0);
	assert(T.compare_int(S) < 0);

	T.insert(2);
	trace1("test2 9", S);
	trace1("test2 2", T);
	assert(T.compare_int(S) < 0);
	assert(S.compare_int(T) > 0);

	T.insert(9);
	trace1("test2 9  ", S);
	trace1("test2 2 9", T);
	assert(S.compare_int(T) < 0);

	S.insert(2);
	trace1("test2", S);
	trace1("test2", T);
	assert(S.compare_int(T) == 0);

}

void test3()
{
	BSET_DYNAMIC<2> S, T;

	S.insert(1);
	T.insert(9);
	trace1("test3", S);
	trace1("test3", T);
	assert(S.compare_int(T) < 0);

	T.insert(1);
	trace1("test3", S);
	trace1("test3", T);
	assert(S.compare_int(T) < 0);

	S.insert(9);
	trace1("test3", S);
	trace1("test3", T);
	assert(S.compare_int(T) == 0);
}

void test4()
{
	BSET_DYNAMIC<3> S, T;

	T.insert(17);
	trace1("test4", S);
	trace1("test4 17", T);
	assert(T.compare_int(S) > 0);
	assert(S.compare_int(T) < 0);

	S.insert(15);
	trace1("test4 15", S);
	trace1("test4", T);
	assert(S.compare_int(T) < 0);
	assert(T.compare_int(S) > 0);

	T.insert(15);
	trace1("test4", S);
	trace1("test4", T);
	assert(S.compare_int(T) < 0);
	assert(T.compare_int(S) > 0);
}

void test5()
{
	BSET_DYNAMIC<3> S, T;

	T.insert(17);
	S.insert(3);
	trace1("test5", S);
	trace1("test5", T);
	assert(S.compare_int(T) < 0);
	assert(T.compare_int(S) > 0);


	S.insert(17);
	trace1("test5", S);
	trace1("test5", T);
	assert(T.compare_int(S) < 0);
	assert(S.compare_int(T) > 0);

	T.insert(3);
	assert(S.compare_int(T) == 0);
}

void test6()
{
	BSET_DYNAMIC<3> S;
  
	S.insert(17);
	S.insert(3);

	BSET_DYNAMIC<3> T(S);

	assert(S == T);
	BSET_DYNAMIC<3> T2(std::move(S));

	assert(S == T2);
}

void test7()
{
	std::vector<BSET_DYNAMIC<5>> a(5);
	a[3].insert(2);

	auto b = std::move(a);
	trace1("test", b[3]);
	assert(b[3].contains(2));
}

void test8()
{
	BSET_DYNAMIC<5> x0;
	assert(x0.begin()==x0.end());

	BSET_DYNAMIC<3> x1;
	assert(x1.begin()==x1.end());

	x1.insert(1);
	x1.insert(2);
	x1.insert(16);
	auto b = x1.begin();
	assert(*b==1);
	++b;
	assert(*b==2);
	++b;
	assert(*b==16);

	b = x1.begin();
	assert(*b==1);
	x1.erase(1);
	++b;
	assert(*b==2);

	x1.insert(1);
	b = x1.begin();
	assert(*b==1);
	x1.erase(2);
	++b;
//	assert(*b==16); // not yet

}

void test9()
{
	typedef cbset::BSET_DYNAMIC<2, uint64_t, cbset::nohowmany_t, cbset::nooffset_t, cbset::nosize_t> myset;
	myset x0;
	x0.insert(0);
	x0.insert(1);
	x0.insert(2);
	x0.insert(16);
	assert(*x0.rbegin() == 16);
	x0.erase(16);
	x0.erase(1);
	assert(*x0.rbegin() == 2);
	x0.erase(2);
	assert(*x0.rbegin() == 0);

	BSET_DYNAMIC<3> x1;
	assert(x1.begin()==x1.end());

	x1.insert(1);
	x1.insert(2);
	x1.insert(16);

//	assert(*x1.rbegin() == 16); // incomplete();


}

int main(int , char* [])
{
	test0();
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();
}

