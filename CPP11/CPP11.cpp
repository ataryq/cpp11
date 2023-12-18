#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>
#include <atlstr.h>
#include <iomanip>
#include <ios>
#include <sstream>
#include <map>

#include "OtherExamples.h"
#include "Templates.h"
#include "Multithreading.h"
#include "MoveSemantic.h"

using namespace std;

void autoExample();
void moveSemantic();

int main()
{
  //checkClass();
  //templateBaseClass();
  //autoExample();
  moveSemantic();
  //lambdas::callLambdas();
  //newForLoop();
  //multAsync();
  //functions();
  //regularExpressions();
  
  //tuplesExample();


  //str = intToHex(i);

  //OMPExamples();

  //system("pause");
  return 0;
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

static int* tNull = nullptr;

void rangeBasedLoop()
{
  std::map<int, std::string> numberMap;
  for (auto val: numberMap) {}

  for (auto i : {1, 2, 3}) {}
}