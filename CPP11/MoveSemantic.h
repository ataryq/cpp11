#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <atlstr.h>


/* rvalue, lvalue references
*/

template <class T>
int passByRef(T&& t)
{
  t = 6;
  return t;
}

class ClassB
{
public:
  ClassB() : v{ 1, 2, 3 }
  {
  }

  ClassB(const ClassB& obj)
  {
    v = obj.v;
    std::cout << "ClassB: copy constructor \n";
  }

  ClassB(ClassB&& obj) noexcept :
    v(std::move(obj.v))
  {
    std::cout << "ClassB: move constructor \n";
  }

  ClassB& operator=(const ClassB& obj) = default;

  std::vector<int> v;
};

class ClassA
{
public:
  ClassA()
  {
    std::cout << "ClassA: constructor \n";
  }

  ClassA(const std::vector<int>& v)
  {
    objB.v = v;
    std::cout << "ClassA: constructor 2 \n";
  }

  ClassA(const ClassA& obj) :
    objB(obj.objB)
  {
    std::cout << "ClassB: copy constructor \n";
  }

  ClassA(ClassA&& obj) noexcept :
    objB(std::move(obj.objB))
  {
    std::cout << "ClassA: move constructor \n";
  }

  ClassA& operator=(const ClassA& obj)
  {
    objB = ClassB(obj.objB);
    std::cout << "ClassA: = constructor \n";
    return *this;
  }

  ClassA& operator=(ClassA&& obj) noexcept
  {
    objB = std::move(obj.objB);
    std::cout << "ClassA: &&= constructor \n";
    return *this;
  }

  ~ClassA() {}

  void print(std::string nameObj)
  {
    std::cout << nameObj << " output: ";
    for (size_t i = 0; i < objB.v.size(); ++i)
    {
      std::cout << objB.v[i] << " ";
    }
    std::cout << "\n";
  }

  ClassB objB;
};

ClassA createClassA()
{
  return ClassA({9, 8, 7});
}

void moveSemantic()
{
  std::vector<int> v{ 1, 2, 3, 4, 5 };
  std::vector<int> v2(std::move(v));
  assert(v.empty());

  assert(passByRef(5) == 6);
  int t = 4;
  passByRef(t);
  assert(t == 6);

  ClassA a;
  a.objB.v = { 3, 4, 5 };
  ClassA a1 = std::move(a);
  a.print("a");
  a1.print("a1");

  std::cout << "\n";

  ClassA a2;
  a2 = ClassA({ 0, 1, 0 });
  a2.print("a2");

  std::cout << "\n";

  ClassA &&a3 = createClassA();
  a3.print("a3");

}
