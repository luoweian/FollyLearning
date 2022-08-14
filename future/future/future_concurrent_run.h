
#include <folly/futures/Future.h>
#include <vector>
#include <functional>

struct Req {
	Req(int _key) {
		key = _key;
	}
	int key;
};

struct Res {
	Res(int _val) {
		val = _val;
	}
	int val;
};

void foo(Req& req, folly::Promise<int> p) {
	std::cout << req.key << std::endl;
	//res->get().val = req.key + 1;
	p.setValue(1);
}


int foo1(Req& req) {
	return 0;
}

//folly::Future<std::vector<int>>
void concurrent_run(std::vector<Req>& reqs) {
	std::vector<folly::Future<int>> futures;
	futures.reserve(reqs.size());
	folly::ThreadedExecutor executor;
	for (auto& req : reqs) {
		folly::Promise<int> p;
		folly::Future<int> f1 = p.getSemiFuture().via(&executor);
		//auto f2 = std::move(f1).thenValue(foo1);
		//int b = 3;
		//futures.push_back(f2);
	}



}





