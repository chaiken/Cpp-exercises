#include <iostream>

namespace slist {

struct slistelem {
  char data;
  slistelem *next;
};

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
  slistelem *first() const { return h_; }
  void Print() const;
  void Print(slistelem *cursor) const;
  void Release();
  unsigned Count(char c) const;
  slistelem *Tail() const;
  void Append(SingleLinkList &sll);
  // Stack interface
  void reset() { Release(); }
  void push(char c) { Prepend(c); }
  char Pop();
  char top_of() { return first()->data; }
  bool empty() { return (h_ == 0); }

private:
  slistelem *h_;
};

} // namespace slist
