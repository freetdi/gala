
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
	trace1("test1", S);
	trace1("test1", T);
	assert(S.compare_int(T) > 0);
	assert(T.compare_int(S) < 0);
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

	T.insert(1);
	trace1("test2", S);
	trace1("test2", T);
	assert(S.compare_int(T) > 0);
	assert(T.compare_int(S) < 0);

	T.insert(9);
	trace1("test2", S);
	trace1("test2", T);
	assert(S.compare_int(T) < 0);

	S.insert(1);
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
	trace1("test4", T);
	assert(S.compare_int(T) < 0);
	assert(T.compare_int(S) > 0);

	S.insert(15);
	trace1("test4", S);
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
}

int main(int , char* [])
{
	test0();
	test1();
	test2();
	test3();
	test4();
	test5();
}

