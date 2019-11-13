
#include <cassert>
#include <cstdlib>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

namespace template_list {
constexpr int ELEMNUM = 100000;
constexpr int SMALLNUM = 100;

template <typename T> struct list_stats {
  T median = 0;
  // Prevent overflow without forcing loss of precision.
  long double mean = 0;
  T max = 0;
  T min = ELEMNUM;
  ::std::pair<T, int> mode;
};

template <typename T> struct object_stats { ::std::pair<T, int> mode; };

template <typename T>
bool ComparePair(const ::std::pair<T, int> &a, const ::std::pair<T, int> &b) {
  return (a.first < b.first);
}

// http://www.cplusplus.com/reference/map/map/map/
// bool fncomp (long lhs, long rhs) {return lhs<rhs;}

template <typename T>
void CalculateListStatisticsObject(::std::list<T> *elemlist,
                                   ::std::map<T, int> &countmap,
                                   struct object_stats<T> &object_stats) {
  ::std::pair<T, int> initial_mode(*elemlist->begin(), 0);
  object_stats.mode = initial_mode;
  // T* is actually an iterator, but the iterator is not a type that the pair
  // declaration will accept as a template parameter.
  //  ::std::pair<T *, bool> result;
  for (T elem : *elemlist) {
    // Initiate a new bucket.
    ::std::pair<T, int> candidate(elem, 1);
    auto result = countmap.insert(candidate);
    // If the item is already present, iterate the existing counter.
    // result.second is the bool.
    if (false == result.second) {
      // result.first is a pointer to a map entry, the 2nd element of which is
      // the count.
      result.first->second++;
    }
    if (result.first->second > object_stats.mode.second) {
      ::std::pair<T, int> new_mode(elem, result.first->second);
      object_stats.mode = new_mode;
    }
  }
}

// Store the occurrence counts in an ordered map that uses the list element
// values as keys.
template <typename T>
void CalculateListStatistics(::std::list<T> *elemlist,
                             ::std::map<T, int> &countmap,
                             struct list_stats<T> &mathstats) {
  ::std::pair<T, int> initial_mode(*elemlist->begin(), 0);
  mathstats.mode = initial_mode;
  // T* is actually an iterator, but the iterator is not a type that the pair
  // declaration will accept as a template parameter.
  //  ::std::pair<T *, bool> result;
  for (T elem : *elemlist) {
    // Initiate a new bucket.
    ::std::pair<T, int> candidate(elem, 1);
    // clang-format off
    // Per  https://cppinsights.io/s/517ae3bb, "auto" is actually:
    // std::pair<std::__map_iterator<std::__tree_iterator<std::__value_type<long, int>, std::__tree_node<std::__value_type<long, int>, void *> *, long> >, bool> result
    // See template_list-cpp_insights_code.cc.
    //
    // template_list_impl.h: In function ‘void template_list::CalculateListStatistics(std::__cxx11::list<T>*, std::map<T, int>&, template_list::list_stats<T>&)’:
    // template_list_impl.h:48:49: error: type/value mismatch at argument 1 in template parameter list for ‘template<class _T1, class _T2> struct std::pair’
    //   ::std::pair<::std::map<T,int>::iterator,bool> result = countmap.insert(candidate);
    // template_list_impl.h:48:49: note:   expected a type, got ‘std::map<T, int>::iterator’
    // clang-format on
    auto result = countmap.insert(candidate);
    // If the item is already present, iterate the existing counter.
    // result.second is the bool.
    if (false == result.second) {
      // result.first is a pointer to a map entry, the 2nd element of which is
      // the count.
      result.first->second++;
    }
    if (result.first->second > mathstats.mode.second) {
      ::std::pair<T, int> new_mode(elem, result.first->second);
      mathstats.mode = new_mode;
    }
    if (::std::is_arithmetic<T>::value) {
      mathstats.mean += elem;
      if (mathstats.min > elem) {
        mathstats.min = elem;
      }
      if (mathstats.max < elem) {
        mathstats.max = elem;
      }
    }
  }
  if (::std::is_arithmetic<T>::value) {
    mathstats.mean /= ELEMNUM;
  }
  /*
   * clang-format off
   * Providing a comparison function is not enough:
   * /usr/include/c++/9/bits/stl_algo.h:1968:22: error: no match for ‘operator-’
   * (operand types are ‘std::_Rb_tree_iterator<std::pair<const long int, int>
   * >’ and ‘std::_Rb_tree_iterator<std::pair<const long int, int> >’
   * ::std::sort(countmap.begin(), countmap.end(), ComparePair<T>);
   * clang-format on
   */
}

template <typename T>
::std::ostream &operator<<(::std::ostream &ost, const ::std::list<T> &alist) {
  /*  for (typename ::std::list<T>::iterator it=alist.begin(); it !=
    alist.end(); it++) { ost << *it << ' ';
    } */
  for (auto x : alist) {
    ost << x << ' ';
  }
  ost << ::std::endl;
  return ost;
}

// Non-class-member functions cannot apparently return templated objects.  Since
// ::std::list is defined by the STL, we need a wrapper class in order to add a
// new templated-object-returning function. Non-member functions can return
// simple types, though, as demonstrated above.
//
//  The keyword "typename" is needed
// before the templated iterator declarations in order to ward off the
// "dependent scope" problem:
// https://stackoverflow.com/questions/610245/where-and-why-do-i-have-to-put-the-template-and-typename-keywords
// Apparently the leading template keyword must appear inside a class
// definition.  The class then provides a scope from which the template type can
// be deduced. Here that's not possible without making a wrapper class for
// ::std::list.
template <typename T> class ListHelper : public ::std::list<T> {
public:
  ListHelper<T>(typename ::std::list<T> *alistp) : listp_(alistp) {}

  typename ::std::list<T>::iterator &
  do_insert(typename ::std::list<T>::iterator *alistit, const T &v) {
    ::std::list<int> templist;
    templist.push_back(v);
    ::std::copy(templist.begin(), templist.end(),
                ::std::inserter(*listp_, *alistit));
    return *alistit;
  }
  void do_erase(typename ::std::list<T>::iterator *alistit) {
    *alistit = listp_->erase(*alistit);
  }

private:
  ::std::list<T> *listp_;
};

} // namespace template_list
