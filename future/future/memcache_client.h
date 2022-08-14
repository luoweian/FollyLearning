#include <string>
#include <functional>
#include <folly/futures/Future.h>
#include <folly/executors/ThreadedExecutor.h>
#include <iostream>

class MemcacheClient {
public:
    struct GetReply {
        enum class Result {
            FOUND,
            NOT_FOUND,
            SERVER_ERROR,
        };

        Result result;
        // The value when result is FOUND,
        // The error message when result is SERVER_ERROR or CLIENT_ERROR
        // undefined otherwise
        std::string value;
    };

    GetReply get(std::string key);

    int async_get(std::string key, GetReply res);

    folly::SemiFuture<GetReply> future_get(std::string key);
    
    //  fetching a batch of requests and waiting for all of them
    void wait_all_async(std::vector<std::string> keys) {
	    MemcacheClient mc;
	
	    std::vector<folly::SemiFuture<GetReply>> futs;
	    for (auto& key : keys) {
		    folly::SemiFuture<GetReply> fut = mc.future_get(key);
		    futs.push_back(std::move(fut));
	    }
	    auto all = collectAll(futs.begin(), futs.end());  
    }

    //  fetching a group of requests and waiting for only one of them
    void wait_a_async(std::vector<std::string> keys) {
        MemcacheClient mc;

        std::vector<folly::SemiFuture<GetReply>> futs;
        for (auto& key : keys) {
            folly::SemiFuture<GetReply> fut = mc.future_get(key);
            futs.push_back(std::move(fut));
        }
        auto any = collectAny(futs.begin(), futs.end());
    }

    /**
    * we can associate a Future with an executor. 
    An executor specifies where work will run, and we detail this more later. 
    In summary, given an executor we can convert a SemiFuture to a Future with an executor, 
    or a Future on one executor to a Future on another executor.
    一旦附加了执行者，Future 允许将延续附加并以单子方式链接在一起
    */
    void bind_executor() {
        MemcacheClient mc;
        folly::ThreadedExecutor executor;
        folly::SemiFuture<GetReply> semiFut = mc.future_get("foo");
        folly::Future<GetReply> fut1 = std::move(semiFut).via(&executor);

        folly::Future<std::string> fut2 = std::move(fut1).thenValue(
            [](GetReply reply) {
                if (reply.result == MemcacheClient::GetReply::Result::FOUND)
                    return reply.value;
                std::cerr << "throw std::SomeException(No value)" << std::endl; 
                return reply.value;
            }
        );

        /*folly::Future<folly::Unit> fut3 = std::move(fut2)
            .thenValue([](std::string str) {
            std::cout << str << std::endl;
             })
            .thenTry([](folly::Try<std::string> strTry) {
                    std::cout << strTry.value() << std::endl;
                })
            .thenError(folly::tag_t<std::exception>{}, [](std::exception const& e) {
                    std::cerr << e.what() << std::endl;
                        });*/
    }
};