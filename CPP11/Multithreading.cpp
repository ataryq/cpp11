#include "Multithreading.h"
#include <thread>
#include <chrono>
#include <future>
#include <iostream>

int performLongComputation()
{
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 1;
}

void multAsync()
{
  simpleThreadExmaple();
  packagedTaskExample();
  asyncExample();
  safeVectorTest();
}

void simpleThreadExmaple()
{
  int threadRes = 0;
  std::thread t([&threadRes]() { threadRes = performLongComputation(); });
  std::cout << "thread 1: " << threadRes << "\n";
  t.join();
}

/*
  you can use "future" only once
*/
void asyncExample()
{
  int threadRes = 0;

  auto ret1 = std::async([]() { return performLongComputation(); });
  std::cout << "thread 2: " << ret1.get() << "\n";

  std::future<int> ret2 = std::async([]() { return performLongComputation(); });
  std::cout << "thread 3: " << ret2.get() << "\n";
}

void packagedTaskExample()
{
  std::packaged_task<int(int, int)> task([](int a, int b)
    {
      return static_cast<int>(std::pow(a, b));
    });
  std::future<int> result = task.get_future();

  task(2, 9);
  std::cout << "task_lambda:\t" << result.get() << '\n';
}

template <class T>
class SafeVector : public std::vector<T>
{
public:
  void pushBack(T val)
  {
    std::lock_guard<std::mutex> lock(m);
    this->push_back(val);
  }

  T& operator [] (int i) 
  { 
    std::lock_guard<std::mutex> lock(m);
    return ((std::vector<T> *)this)->operator[](i); 
  }

private:
  mutable std::mutex m;
};

void safeVectorTest()
{
  SafeVector<char> v;
  v.pushBack('a');
  char ch = v[0];
  std::cout << ch << "\n";
}