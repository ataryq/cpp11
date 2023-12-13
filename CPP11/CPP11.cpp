#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>
#include <atlstr.h>
#include <iomanip>
#include <ios>
#include <sstream>

#include "OtherExamples.h"
#include "Templates.h"
#include "Multithreading.h"

using namespace std;

void autoExample();
void r_l_values();

template< typename T >
CString intToHex(T i)
{
  std::stringstream stream;
  stream << "0x"
    << std::setfill('0') << std::setw(sizeof(T) * 2)
    << std::hex << i;
  return stream.str().c_str();
}

int main()
{
  /*
  checkClass();
  templateBaseClass();
  autoExample();
  r_l_values();
  lambdas::callLambdas();
  newForLoop();
  multAsync();
  //functions();
  regularExpressions();
  */
  //tuplesExample();

  INT64 i = 0x00000196537f6710;
  CString str;
  str.Format(L"{%x}", i);

  INT64 ptr = 0;
  swscanf_s(str.GetString(), L"{%I64x}", &ptr);

  //str = intToHex(i);

  int t = 0;
}

/* auto
*/
void autoExample()
{
  auto i = 1;
  auto d = { 1, 2 }; // type of d is std::initializer_list<int>
  auto lambda = [](int x) { return x + 3; };
  assert(lambda(1) == 4);

}

/* rvalue, lvalue references
*/

template <class T>
int passByRef(T&& t)
{
  t = 6;
  return t;
}

class A
{
public:
  A() {}
  ~A() {}
};

void r_l_values()
{
  std::vector<int> v{ 1, 2, 3, 4, 5 };
  std::vector<int> v2(std::move(v));
  assert(v.empty());

  assert(passByRef(5) == 6);
  int t = 4;
  passByRef(t);
  assert(t == 6);

  A a;
  A a1 = std::move(a);
}

static int* tNull = nullptr;
