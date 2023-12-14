#pragma once

#include <iostream>
#include <stdexcept>
#include <forward_list>
#include <functional>
#include <regex>

class SimpleClass
{
public:
  int value = 3;

  void print(int t) const { std::cout << t << "\n"; }
  void add(int t) const { std::cout << value + t << "\n"; }
};

void simpleClassPrint(const SimpleClass &obj)
{
  std::cout << "SimpleClass: value: " << obj.value << " \n";
}

namespace methodsKeywords 
{
  /* default - force to generate default method
delete - forbid to use method
final - forbid override method or inherit class
override - put it after function to make sure if mrthod is overrided from base class
*/
  class Trivial
  {
    Trivial() = delete;
    Trivial(int n, char ch) {}
    Trivial(char ch) : Trivial(0, ch) {} // Delegate constructor

  public:
    virtual void anyFun() {}
  };

  class NotTrivial final : Trivial
  {
    void anyFun() final override {}
    ~NotTrivial() = default;
  };
}

namespace ConstExpr
{

  // C++11 constexpr functions use recursion rather than iteration
  constexpr int factorial(int n)
  {
    return n <= 1 ? 1 : (n * factorial(n - 1));
  }

  // C++14 constexpr functions may use local variables and loops
#if __cplusplus >= 201402L
  constexpr int factorial_cxx14(int n)
  {
    int res = 1;
    while (n > 1)
      res *= n--;
    return res;
  }
#endif // C++14

  // literal class
  class conststr
  {
    const char* p;
    std::size_t sz;
  public:
    template<std::size_t N>
    constexpr conststr(const char(&a)[N]) : p(a), sz(N - 1) {}

    // constexpr functions signal errors by throwing exceptions
    // in C++11, they must do so from the conditional operator ?:
    constexpr char operator[](std::size_t n) const
    {
      return n < sz ? p[n] : throw std::out_of_range("");
    }

    constexpr std::size_t size() const { return sz; }
  };

  // C++11 constexpr functions had to put everything in a single return statement
  // (C++14 doesn't have that requirement)
  constexpr std::size_t countlower(conststr s, std::size_t n = 0,
    std::size_t c = 0)
  {
    return n == s.size() ? c :
      'a' <= s[n] && s[n] <= 'z' ? countlower(s, n + 1, c + 1) :
      countlower(s, n + 1, c);
  }

  // output function that requires a compile-time constant, for testing
  template<int n>
  struct constN
  {
    constN() { std::cout << n << '\n'; }
  };

  int constExpress()
  {
    std::cout << "4! = ";
    constN<factorial(4)> out1; // computed at compile time

    volatile int k = 8; // disallow optimization using volatile
    std::cout << k << "! = " << factorial(k) << '\n'; // computed at run time

    std::cout << "the number of lowercase letters in \"Hello, world!\" is ";
    constN<countlower("Hello, world!")> out2; // implicitly converted to conststr

    constexpr int a[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    constexpr int length_a = sizeof(a) / sizeof(int); // std::size(a) in C++17,
                                                    // std::ssize(a) in C++20
    std::cout << "array of length " << length_a << " has elements: ";
    for (int i = 0; i < length_a; ++i)
      std::cout << a[i] << " ";

    return 0;
  }
}


// every object of type struct_float will be aligned to alignof(float) boundary
// (usually 4):
struct alignas(float) struct_float
{
  // your definition here
};

// every object of type sse_t will be aligned to 32-byte boundary:
struct alignas(32) sse_t
{
  float sse_data[4];
};

// the array "cacheline" will be aligned to 64-byte boundary:
alignas(64) char cacheline[64];

[[noreturn]]
void nothingToReturn() {}

namespace lambdas
{

  void callLambdas()
  {
    auto glambda = [](auto a, auto&& b) { return a < b; };
    bool b = glambda(3, 3.14);

    int s1 = 1;
    //take all args in the scope by reference
    //mutalbe allow changes value and call const function of args
    auto l1 = [&](int a) mutable { s1 += a; };
    //take all args in the scope as const references
    auto l2 = [=](int a) { return s1 + a; };
  }
}

void newForLoop()
{
  std::vector<int> numbers = {1, 2, 3};
  for (int num : numbers)
  {
    std::cout << num << " ";
  }
  std::cout << "\n";
}

void forwardList()
{
  //однонаправленный связный список
  std::forward_list<int> list = { 1, 2 };
  std::vector<int> vList = { 1, 2 };
}

void initializerList()
{
  for (int x : {-1, -2, -3})
    std::cout << x << ' ';

  auto al = { 10, 11, 12 };
  std::vector<int> v = { 1, 2, 3 };
}

void functions()
{
  std::function<void(const SimpleClass&, int)> fAdd = &SimpleClass::add;
  SimpleClass obj;
  obj.value = 2;
  fAdd(obj, 5);

  std::function<void(const SimpleClass&)> printObj = simpleClassPrint;
  printObj(obj);
}

void regularExpressions()
{
  /*
  [a-z] - range for 1 lowercase letter
  [a-z]+ - range for 1 or more lowercase letter
  [a-z]* - range for 0 or more lowercase letter
  [a-zA-Z]+ - 1 or more letter
  [a-z]{3} - specify exact number of characters should occur in the string
  [a-z]{0, 3} - from 0 up to 3 character
  (Xyz)+ - 1 or more group of characters, examples: Xyz, XyzXyzXyz
  \\+ - to add + to regexp
  \\sb - to add space to regexp

  Examples:
    text filename: "([a-z]+)\\.txt"
    phone: \\+7[0-9\\sb]+
  */


  std::regex querry("\\+7[0-9\\sb]+");
  std::string phoneNumber = "+7 912 123 4343";
  bool match = std::regex_match(phoneNumber, querry);
  assert(match);

  std::smatch m;
  std::regex_search(phoneNumber, m, std::regex("9[0-9]{2}"));
  
  //convert smatch to string
  std::string sFound;
  for (auto x : m)
    sFound += x;

  assert(sFound == "912");

  bool wCompare = std::regex_search(L"asd", std::wregex(L"asd"));
  assert(wCompare);
}

std::tuple<int, int, std::string> simpleTuples()
{
  return std::make_tuple(0, 1, "Ararat");
}

using UserInfo = std::tuple<int, int, std::string>;

void tuplesExample()
{
  UserInfo userInfo = simpleTuples();
  std::cout << "id: " << std::get<0>(userInfo) 
    << " number: " << std::get<1>(userInfo)
    << " user name: " << std::get<2>(userInfo) 
    << "\n";

  size_t nFields = std::tuple_size_v<UserInfo>;
}

template< typename T >
CString intToHex(T i)
{
  std::stringstream stream;
  stream << "0x"
    << std::setfill('0') << std::setw(sizeof(T) * 2)
    << std::hex << i;
  return stream.str().c_str();
}

void parseAndScanExample()
{
  INT64 i = 0x00000196537f6710;
  CString str;
  str.Format(L"{%x}", i);

  std::cout << str << "\n"; //{0x00000196537f6710}

  INT64 ptr = 0;
  swscanf_s(str.GetString(), L"{%I64x}", &ptr);

  std::cout << ptr << "\n"; // 0x00000196537f6710
}