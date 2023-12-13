#pragma once
#include <iostream>

template<typename T>
class IsClass
{
  typedef char yes[1];
  typedef char no[2];

  template<typename C>
  static yes& test(int C::*); // selected if C is a class type

  template<typename C>
  static no& test(...);       // selected otherwise
public:
  static bool const value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

class TestClass
{
public:
  int t = 0;
};

void checkClass()
{
  std::cout << "Is TestClass class: " << IsClass<TestClass>::value << std::endl;
  std::cout << "Is int class: " << IsClass<int>::value << std::endl;
}

//inherit base class by template

class TestClass2 {};

template<class... Mixins>
class X : public Mixins...
{
public:
  X(const Mixins&... mixins) : Mixins(mixins)... {}
};

void templateBaseClass()
{
  X<TestClass, TestClass2> obj{ TestClass(), TestClass2() };
  int t = obj.t;
}