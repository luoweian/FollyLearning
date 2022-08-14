# CMakeList.txt: future 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 寻找依赖项
find_package(folly CONFIG REQUIRED)
find_package(Boost REQUIRED)
# 添加头文件路径
include_directories(${Boost_INCLUDE_DIRS})
include_directories(include ${FOLLY_INCLUDE_DIRS})


#set_and_check(FOLLY_INCLUDE_DIR /usr/local/include/folly)
#set_and_check(FOLLY_CMAKE_DIR /usr/local/lib/cmake/folly)
set(FOLLY_LIBRARIES Folly::folly)
if (NOT folly_FIND_QUIETLY)
  message(STATUS "Found folly: ${PACKAGE_PREFIX_DIR}")
endif()


# 将源代码添加到此项目的可执行文件。
add_executable (future "future.cpp" "future.h" "future_demo.h" "memcache_client.h" "memcache_client.cpp" "future_concurrent_run.h")

# 添加链接库
target_link_libraries(future ${Boost_LIBRARIES} ${FOLLY_LIBRARIES})


# TODO: 如有需要，请添加测试并安装目标。
