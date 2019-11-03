// https://cppinsights.io/s/517ae3bb
constexpr int ELEMNUM = 10000;

template <typename T>
void CalculateListStatistics(::std::list<T> *elemlist,
                             ::std::map<T, int> &countmap) {
  for (T elem : *elemlist) {
    ::std::pair<T, int> candidate(elem, 1);
    auto result = countmap.insert(candidate);
    if (false == result.second) {
      result.first->second++;
    }
  }
}

long int GetRand() { return (random() % (ELEMNUM - 1)); }

void FillList(::std::list<long int> *elemlist) {
  for (int i = 0; i < ELEMNUM; i++) {
    elemlist->emplace_back(GetRand());
  }
}

int main(int argc, char** argv) {
  ::std::list<long int> *newlist = new list<long int>();
  FillList(newlist);
  ::std::map<long int, int> randcounter;
  CalculateListStatistics(newlist, randcounter);
}

Becomes:

#include <cstdlib>
#include <list>
#include <map>

constexpr const int ELEMNUM = 10000;


template <typename T>
void CalculateListStatistics(::std::list<T> *elemlist,
                             ::std::map<T, int> &countmap) {
  for (T elem : *elemlist) {
    ::std::pair<T, int> candidate(elem, 1);
    auto result = countmap.insert(candidate);
    if (false == result.second) {
      result.first->second++;
    }
  }
}

/* First instantiated from: insights.cpp:31 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
void CalculateListStatistics<long>(std::list<long, std::allocator<long> > * elemlist, std::map<long, int, std::less<long>, std::allocator<std::pair<const long, int> > > & countmap)
{
  {
    std::list<long, std::allocator<long> > & __range1 = *elemlist;
    for(std::__list_iterator<long, void *> __begin0 = std::__list_iterator<long, void *>(__range1.begin()), __end0 = std::__list_iterator<long, void *>(__range1.end()); operator!=(__begin0, __end0); __begin0.operator++()) 
    {
      long elem = __begin0.operator*();
      ::std::pair<long, int> candidate = std::pair<long, int>(elem, 1);
      std::pair<std::__map_iterator<std::__tree_iterator<std::__value_type<long, int>, std::__tree_node<std::__value_type<long, int>, void *> *, long> >, bool> result = std::pair<std::__map_iterator<std::__tree_iterator<std::__value_type<long, int>, std::__tree_node<std::__value_type<long, int>, void *> *, long> >, bool>(countmap.insert(candidate));
      if(static_cast<int>(false) == static_cast<int>(result.second)) {
        result.first.operator->()->second++;
      }
    }
  }
}
#endif


long GetRand()
{
  return (random() % static_cast<long>((ELEMNUM - 1)));
}


void FillList(std::list<long, std::allocator<long> > * elemlist)
{
  for(int i = 0; i < ELEMNUM; i++) 
  {
    elemlist->emplace_back(GetRand());
  }
  
}


int main(int argc, char ** argv)
{
  std::list<long, std::allocator<long> > * newlist = new std::list<long, std::allocator<long> >();
  FillList(newlist);
  ::std::map<long, int> randcounter = std::map<long, int, std::less<long>, std::allocator<std::pair<const long, int> > >();
  CalculateListStatistics(newlist, randcounter);
}


&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

Error before replacing

  ::std::pair<T *, bool> result;
  for (T elem : *elemlist) {
    // Initiate a new bucket.
    ::std::pair<T, int> candidate(elem, 1);
    result = countmap.insert(candidate);
    // If the item is already present, iterate the existing counter.
    // result.second is the bool.
    if (false = result.second) {
      // result.first is a pointer to a map entry, the 2nd element of which is
      // the count.
      result.first->second++;
    }
  }

with

  ::std::pair<T *, bool> result;
  for (T elem : *elemlist) {
    // Initiate a new bucket.
    ::std::pair<T, int> candidate(elem, 1);
    result = countmap.insert(candidate);
    // If the item is already present, iterate the existing counter.
    // result.second is the bool.
    if (false = result.second) {
      // result.first is a pointer to a map entry, the 2nd element of which is
      // the count.
      result.first->second++;
    }
  }

c++ insights shows that rather than

  ::std::pair<T *, bool> result

I end up with

  std::pair<std::__map_iterator<std::__tree_iterator<std::__value_type<long, int>, std::__tree_node<std::__value_type<long, int>, void *> *, long> >, bool> result

There's still a bool as second argument, but first is now quadruply nested templates.

$ make template_list_lib_test
/usr/bin/g++ -std=c++11 -pthread -ggdb -Wall -Wextra -g -O0 -fno-inline -fsanitize=address,undefined -I/home/alison/gitsrc/googletest/googletest/include -ggdb -g -fsanitize=address -L/home/alison/gitsrc/googletest/googletest/make -lpthread /home/alison/gitsrc/googletest/googletest/make/libgtest.a /home/alison/gitsrc/googletest/googletest/make/libgtest_main.a template_list_lib.cc template_list_lib_test.cc -o template_list_lib_test
In file included from template_list.h:11,
                 from template_list_lib_test.cc:1:
template_list_impl.h: In instantiation of ‘void template_list::CalculateListStatistics(std::__cxx11::list<T>*, std::map<T, int>&) [with T = long int]’:
template_list_lib_test.cc:32:47:   required from here
template_list_impl.h:21:12: error: no match for ‘operator=’ (operand types are ‘std::pair<long int*, bool>’ and ‘std::__enable_if_t<true, std::pair<std::_Rb_tree_iterator<std::pair<const long int, int> >, bool> >’ {aka ‘std::pair<std::_Rb_tree_iterator<std::pair<const long int, int> >, bool>’})
   21 |     result = countmap.insert(candidate);
In file included from /usr/include/c++/9/bits/stl_algobase.h:64,
                 from /usr/include/c++/9/list:60,
                 from template_list.h:4,
                 from template_list_lib_test.cc:1:
/usr/include/c++/9/bits/stl_pair.h:378:7: note: candidate: ‘std::pair<_T1, _T2>& std::pair<_T1, _T2>::operator=(typename std::conditional<std::__and_<std::is_copy_assignable<_T1>, std::is_copy_assignable<_T2> >::value, const std::pair<_T1, _T2>&, const std::__nonesuch_no_braces&>::type) [with _T1 = long int*; _T2 = bool; typename std::conditional<std::__and_<std::is_copy_assignable<_T1>, std::is_copy_assignable<_T2> >::value, const std::pair<_T1, _T2>&, const std::__nonesuch_no_braces&>::type = const std::pair<long int*, bool>&]’
  378 |       operator=(typename conditional<
      |       ^~~~~~~~
/usr/include/c++/9/bits/stl_pair.h:381:51: note:   no known conversion for argument 1 from ‘std::__enable_if_t<true, std::pair<std::_Rb_tree_iterator<std::pair<const long int, int> >, bool> >’ {aka ‘std::pair<std::_Rb_tree_iterator<std::pair<const long int, int> >, bool>’} to ‘std::conditional<true, const std::pair<long int*, bool>&, const std::__nonesuch_no_braces&>::type’ {aka ‘const std::pair<long int*, bool>&’}
  378 |       operator=(typename conditional<
      |                 ~~~~~~~~~~~~~~~~~~~~~              
  379 |   __and_<is_copy_assignable<_T1>,
      |   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                  
  380 |          is_copy_assignable<_T2>>::value,
      |          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~          
  381 |   const pair&, const __nonesuch_no_braces&>::type __p)
      |   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~
/usr/include/c++/9/bits/stl_pair.h:389:7: note: candidate: ‘std::pair<_T1, _T2>& std::pair<_T1, _T2>::operator=(typename std::conditional<std::__and_<std::is_move_assignable<_Tp>, std::is_move_assignable<_T2> >::value, std::pair<_T1, _T2>&&, std::__nonesuch_no_braces&&>::type) [with _T1 = long int*; _T2 = bool; typename std::conditional<std::__and_<std::is_move_assignable<_Tp>, std::is_move_assignable<_T2> >::value, std::pair<_T1, _T2>&&, std::__nonesuch_no_braces&&>::type = std::pair<long int*, bool>&&]’
  389 |       operator=(typename conditional<
      |       ^~~~~~~~
/usr/include/c++/9/bits/stl_pair.h:392:41: note:   no known conversion for argument 1 from ‘std::__enable_if_t<true, std::pair<std::_Rb_tree_iterator<std::pair<const long int, int> >, bool> >’ {aka ‘std::pair<std::_Rb_tree_iterator<std::pair<const long int, int> >, bool>’} to ‘std::conditional<true, std::pair<long int*, bool>&&, std::__nonesuch_no_braces&&>::type’ {aka ‘std::pair<long int*, bool>&&’}
  389 |       operator=(typename conditional<
      |                 ~~~~~~~~~~~~~~~~~~~~~    
  390 |   __and_<is_move_assignable<_T1>,
      |   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~        
  391 |          is_move_assignable<_T2>>::value,
      |          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  392 |   pair&&, __nonesuch_no_braces&&>::type __p)
      |   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~
/usr/include/c++/9/bits/stl_pair.h:405:2: note: candidate: ‘template<class _U1, class _U2> typename std::enable_if<std::__and_<std::is_assignable<_T1&, const _U1&>, std::is_assignable<_T2&, const _U2&> >::value, std::pair<_T1, _T2>&>::type std::pair<_T1, _T2>::operator=(const std::pair<_U1, _U2>&) [with _U1 = _U1; _U2 = _U2; _T1 = long int*; _T2 = bool]’
  405 |  operator=(const pair<_U1, _U2>& __p)
      |  ^~~~~~~~
/usr/include/c++/9/bits/stl_pair.h:405:2: note:   template argument deduction/substitution failed:
/usr/include/c++/9/bits/stl_pair.h: In substitution of ‘template<class _U1, class _U2> typename std::enable_if<std::__and_<std::is_assignable<long int*&, const _U1&>, std::is_assignable<bool&, const _U2&> >::value, std::pair<long int*, bool>&>::type std::pair<long int*, bool>::operator=<_U1, _U2>(const std::pair<_T1, _T2>&) [with _U1 = std::_Rb_tree_iterator<std::pair<const long int, int> >; _U2 = bool]’:
template_list_impl.h:21:12:   required from ‘void template_list::CalculateListStatistics(std::__cxx11::list<T>*, std::map<T, int>&) [with T = long int]’
template_list_lib_test.cc:32:47:   required from here
/usr/include/c++/9/bits/stl_pair.h:405:2: error: no type named ‘type’ in ‘struct std::enable_if<false, std::pair<long int*, bool>&>’
template_list_impl.h: In instantiation of ‘void template_list::CalculateListStatistics(std::__cxx11::list<T>*, std::map<T, int>&) [with T = long int]’:
template_list_lib_test.cc:32:47:   required from here
/usr/include/c++/9/bits/stl_pair.h:416:2: note: candidate: ‘template<class _U1, class _U2> typename std::enable_if<std::__and_<std::is_assignable<_T1&, _U1&&>, std::is_assignable<_T2&, _U2&&> >::value, std::pair<_T1, _T2>&>::type std::pair<_T1, _T2>::operator=(std::pair<_U1, _U2>&&) [with _U1 = _U1; _U2 = _U2; _T1 = long int*; _T2 = bool]’
  416 |  operator=(pair<_U1, _U2>&& __p)
      |  ^~~~~~~~
/usr/include/c++/9/bits/stl_pair.h:416:2: note:   template argument deduction/substitution failed:
/usr/include/c++/9/bits/stl_pair.h: In substitution of ‘template<class _U1, class _U2> typename std::enable_if<std::__and_<std::is_assignable<long int*&, _U1&&>, std::is_assignable<bool&, _U2&&> >::value, std::pair<long int*, bool>&>::type std::pair<long int*, bool>::operator=<_U1, _U2>(std::pair<_T1, _T2>&&) [with _U1 = std::_Rb_tree_iterator<std::pair<const long int, int> >; _U2 = bool]’:
template_list_impl.h:21:12:   required from ‘void template_list::CalculateListStatistics(std::__cxx11::list<T>*, std::map<T, int>&) [with T = long int]’
template_list_lib_test.cc:32:47:   required from here
/usr/include/c++/9/bits/stl_pair.h:416:2: error: no type named ‘type’ in ‘struct std::enable_if<false, std::pair<long int*, bool>&>’
In file included from template_list.h:11,
                 from template_list_lib_test.cc:1:
template_list_impl.h: In instantiation of ‘void template_list::CalculateListStatistics(std::__cxx11::list<T>*, std::map<T, int>&) [with T = long int]’:
template_list_lib_test.cc:32:47:   required from here
template_list_impl.h:24:15: error: lvalue required as left operand of assignment
   24 |     if (false = result.second) {
      |         ~~~~~~^~~~~~~~~~~~~~~
template_list_impl.h:27:21: error: request for member ‘second’ in ‘* result.std::pair<long int*, bool>::first’, which is of non-class type ‘long int’
   27 |       result.first->second++;
      |       ~~~~~~~~~~~~~~^~~~~~
make: *** [Makefile:166: template_list_lib_test] Error 1

&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

compiler-explorer Slack channel:


Alison Chaiken 9:41 PM
Thanks for the tip above, logan.   Rather than trying "std::pair<std::map<long,int>::iterator, bool>", I was actually trying "template <typename T> std::pair<std::map<T, int>::iterator, bool>", which is perhaps expecting too much of template inference.  Perhaps what I was trying just won't work outside an instantiation.
I see that in the templated function, "auto" remains in the c++-insight output.   Perhaps "auto" here really means "indeterminate", as opposed to "I'm too lazy to figure it out"?
logan 9:53 PM
I see that in the templated function, "auto" remains in the c++-insight output.   Perhaps "auto" here really means "indeterminate", as opposed to "I'm too lazy to figure it out"?
logan 9:53 PM
I’m not positive what is happening in your example or with cppinsights, but auto does always represent a real, knowable type
it’s not always something you can write out by hand, though, especially when it comes to templates. consider:

int foo(int);
double foo(double);
template <typename T> void callFoo(T t) { 
    auto x = foo(t); // what's the type of auto? depends on T
}

beached 10:08 PM

auto foo = [] { return 5; }

I don’t think the type of foo is knowable, it is real though.
m.zdun 10:13 PM
Yeah, "knowable" might refer to either programmer or compiler. In the first case, it's definitely not knowable, it surely is in the second... :grin:
arthur-odwyer 8:30 AM
Funnily enough, I would say that in the first case the type is obviously T (T x = foo(t);) [EDIT: retracted], whereas in the second case it's... well, I'd say it's not spellable. (Except as decltype(foo) after the fact.) (edited) 
OTOH, I admit that in the first case T is a simplification that is possible only through human knowledge. Consider

int foo(int);
float foo(double);
template <typename T> void callFoo(T t) { 
    auto x = foo(t); // what's the type of auto? depends on T
}

where the type of x is basically std::conditional_t<std::is_same_v<T, double>, float, int> :wink: ...and even then, if T is some user-defined type, then maybe the foo that gets called is some ADL function. Good times.
...and even without ADL, T x = foo(t); would not be equivalent, if T is let's say float. Okay, auto in the first case is "unknowable" :slightly_smiling_face:
logan 8:41 AM
it’s “unknowable” because it depends on T, which we don’t know. once we do, we can definitely figure out what auto is
that was my original point. for every T, there is a type that auto definitely is as a result, but we can’t in general spell out what it is in the template     

Alison Chaiken Yesterday at 10:18 PM
@logan @arthur-odwyer I noticed that the result of expanding "auto" with cpp-insights includes some functions whose names starts with an underscore.    I assume that these functions are private methods of some STL object.  Then "auto" might not be expandable in the template because the template could not directly invoke those methods?
7 replies

arthur-odwyer  22 hours ago
I'd have to see an example. My first guess is that these "functions" are really member typedefs, like std::vector<T, A>::__internal_type, and that they're non-private... but who knows.

arthur-odwyer  22 hours ago
FYI, lambda types are often spelled e.g. $_0, $_1, etc. So if you see dollar signs, that may be why.

Alison Chaiken  9 hours ago
I don't mean to take up a bunch of your time, but I'm just curious about https://cppinsights.io/s/517ae3bb .  Line 31 where "auto" is expanded has, for example, std::__value_type.  Perhaps that's also a typedef that I haven't managed to find.

logan  5 hours ago
you guess correctly that std::__value_type, and all those crazy looking double-underscore types, are private details of the standard library implementation (libc++ in this case). your user C++ code is actually not allowed to talk about them directly. there is a standard, portable way to spell them in this case, which is std::map<T, int>::iterator. that’s a typedef provided by std::map that expands out to whatever gobbledygook your specific implementation uses to implement iterating over std::maps

logan  5 hours ago
it seems in some of these examples cppinsights is being “too helpful” for your goal of practicing spelling out types instead of using auto. while what it’s showing you is correct, it’s skipping past what you can and should write in your code, and straight to the gory details.

logan  5 hours ago
it may be more helpful to study the documentation of the functions you’re capturing the return of. https://en.cppreference.com/w/cpp/container/map/insert you’re calling overload #1 here. as it says, it returns a std::pair<iterator,bool>. the return type is assumed to be within the scope of the map class, so iterator is used unqualified, but in your code, you’d write std::pair<std::map<K,V>::iterator, bool>

Alison Chaiken  < 1 minute ago
Thanks, logan.   I tried using the approach you suggest and got stuck, so then I thought I'd trying going backward.  It makes sense that cpp-insights produces an endpoint of the preprocessing that does in fact involve private members.

arthur-odwyer  16 hours ago
To be clear, those uglified names are not private names. They're public. You can take the output of CppInsights and paste it right back into the compiler, and it'll compile fine. There is no access-control violation implied here. https://godbolt.org/z/X744_0 [EDIT: oops, better link https://godbolt.org/z/Ot1jVv ] (edited) 
arthur-odwyer  16 hours ago
But those uglified names represent "implementation details" of libc++. They're just helpers. They're not mandated by the standard to exist on every implementation; other implementations might have different details. (edited) 
arthur-odwyer  16 hours ago
You can observe in https://godbolt.org/z/Ot1jVv that the output code compiles with libc++, but not with libstdc++ (nor with MSVC's STL).
arthur-odwyer  16 hours ago
...And std::__value_type is defined at https://github.com/llvm-mirror/libcxx/blob/master/include/map#L656-L657 in libc++.
include/map:656-657

template <class _Key, class _Tp>
struct __value_type

<https://github.com/llvm-mirror/libcxx|llvm-mirror/libcxx>llvm-mirror/libcxx | Added by GitHub
Alison Chaiken  < 1 minute ago
Thanks for the further details, arthur-odwyer.  It makes sense that the functions are not private, but just not portable or officially supported
