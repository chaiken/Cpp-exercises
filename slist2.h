#include <iostream>
#include <memory>
#include <sstream>

namespace slist {

class SingleLinkListElem;

class SingleLinkList {
public:
  SingleLinkList() {}
  SingleLinkList(const char *s);
  SingleLinkList(const SingleLinkList &) = delete;
  SingleLinkList(SingleLinkList &&sl) = default;
  size_t Length() const;
  void Prepend(char c);
  void Delete();
  SingleLinkListElem *First() const { return h_.get(); }
  SingleLinkListElem *Last() const;
  unsigned Count(char c) const;
  void Append(SingleLinkList &sll);
  // Stack interface
  void push(char c) { Prepend(c); }
  char Pop();
  bool empty() { return (h_ == 0); }
  friend std::ostringstream &operator<<(std::ostringstream &out,
                                        const SingleLinkList &sll);

private:
  std::unique_ptr<SingleLinkListElem> h_;
};

std::ostringstream &operator<<(std::ostringstream &out,
                               const SingleLinkList &sll);

struct SingleLinkListElem {
  SingleLinkListElem(char c) : data(c), next(nullptr) {}
  SingleLinkListElem(char c, SingleLinkList *sll)
      : data(c), next(sll->First()) {}
  SingleLinkListElem(const SingleLinkListElem &e) = delete;
  SingleLinkListElem(SingleLinkListElem &&e) = default;
  char data;
  std::unique_ptr<SingleLinkListElem> next;
};

} // namespace slist
