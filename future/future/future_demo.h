#pragma once

#include <folly/futures/Future.h>
#include <folly/executors/ThreadedExecutor.h>
#include <windows.h>


void foo_(int x) {
	// do something with x
	std::cout << "foo(" << x << ") begin sleep" << std::endl;
	Sleep(5000);
	std::cout << "foo(" << x << ") end sleep" << std::endl;
}


void m2(folly::ThreadedExecutor& executor, int x) {
	std::cout << "making Promise" << std::endl;
	folly::Promise<int> p;
	folly::Future<int> f = p.getSemiFuture().via(&executor);
	/**
	* The primary difference from std::future is that you can attach callbacks to Futures (with thenValue or thenTry)
	* 使用
	*/
	auto f2 = std::move(f).thenValue(foo_);
	std::cout << "Future chain made" << std::endl;

	// ... now perhaps in another event callback
	std::cout << "fulfilling Promise" << std::endl;
	p.setValue(x);
	std::move(f2).get();
	std::cout << "Promise fulfilled" << std::endl;

	/**
	* 总结：
	* 首先定义一个Promise，通过Promise获取与它相关联的Future
	* 通过thenValue or thenTry设置一个回调函数foo.
	* 最后当Promise赋值填充时候（setValue），相关的Future会变为ready状态（或者是completed状态）, foo会被执行
	* std::move(f2).get(); 会等待foo执行完成
	*/
}


void m1() {
	// ...
	folly::ThreadedExecutor executor;
	m2(executor, 1);
	m2(executor, 2);
}

