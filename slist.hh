#include <iostream>
#include <memory>
#include <sstream>

namespace slist {

struct slistelem {
  slistelem(const char ch) : data(ch) {}

  char data;
  std::unique_ptr<slistelem> next;
};

class SingleLinkList {
public:
  SingleLinkList(const char *s);
  SingleLinkList(const SingleLinkList &s) = delete;
  SingleLinkList(SingleLinkList &&s) = default;
  size_t Length() const;
  void Prepend(char c);
  void Delete();
  slistelem *first() const { return h_.get(); }
  friend std::ostream &operator<<(std::ostream &out, const SingleLinkList &sll);
  void Print(slistelem *cursor) const;
  size_t Count(char c) const;
  slistelem *Tail() const;
  void Append(SingleLinkList &&sll);
  // Stack interface
  void push(char c) { Prepend(c); }
  char Pop();
  char top_of() { return first()->data; }
  bool empty() { return (!h_); }

private:
  std::unique_ptr<slistelem> h_;
};

std::ostream &operator<<(std::ostream &out, const SingleLinkList &sll);

} // namespace slist
