#include <iostream>

namespace slist {

class SingleLinkListElem;

class SingleLinkList {
public:
  SingleLinkList() : h_(0) {}
  SingleLinkList(const char *s);
  ~SingleLinkList() {
    ::std::cout << "Destructor." << ::std::endl;
    Release();
  }
  size_t Length() const;
  void Prepend(char c);
  void Delete();
  SingleLinkListElem *first() const { return h_; }
  void Print() const;
  void Release();
  unsigned Count(char c) const;
  void Append(SingleLinkList &sll);
  // Stack interface
  void reset() { Release(); }
  void push(char c) { Prepend(c); }
  char Pop();
  //  char top_of() { return first()->data_; }
  bool empty() { return (h_ == 0); }

private:
  SingleLinkListElem *Tail() const;

  SingleLinkListElem *h_;
};

class SingleLinkListElem {
public:
  SingleLinkListElem(char c) : data_(c), next_(nullptr) {}
  SingleLinkListElem(char c, SingleLinkList *slle)
      : data_(c), next_(slle->first()) {}

  // Why doesn't this work?
  // friend size_t SingleLinkList::Length() const;
  friend class SingleLinkList;

private:
  char data_;
  SingleLinkListElem *next_;
};

} // namespace slist
