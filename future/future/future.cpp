// future.cpp: 定义应用程序的入口点。
//

#include "future.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	m1();

	Req req = Req(1);
	Req req1 = Req(2);
	std::vector<Req> reqs;
	reqs.push_back(req);
	reqs.push_back(req1);
	concurrent_run(reqs);
	return 0;
}
