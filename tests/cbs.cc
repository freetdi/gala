
#include <assert.h>
#include "../boost.h"
#include "../cbset.h"

template<class T, class...>
// class myset : public cbset::BSET_DYNAMIC<4>{};
using myset=cbset::BSET_DYNAMIC<4>;

using cbset::BSET_DYNAMIC;

int main(int , char* [])
{
	BSET_DYNAMIC<2> S;
	assert(S.size()==0);

	S.insert(8);
	assert(S.size()==1);
	trace1("+8", S);

	S.insert(0);
	trace1("+0", S);
	assert(S.size()==2);

	S.insert(4);
	trace1("+4", S);
	S.insert(4);
	trace1("+4", S);
	assert(S.size()==3);

	S.erase(4);
	trace1("-4", S);
	assert(S.size()==2);
	trace1("", S);

}
