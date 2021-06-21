/* Not an exercise from the Pohl book, but fun with Scott Meyers' assertion that
   "rhs is an lvalue even though it has an rvalue reference type" on p.3 of
   __Effective Modern C++_.  "By similar reasoning, all [function] parameters
   are lvalues" (p.3) where (p. 4) "In a function call, the expressions passed
   at the call site are the function's arguments.  The arguments are used to
   initialize the function's parameters."
 */

#include <array>
#include <cassert>

#include <utility>

using namespace std;

class Widget {
public:
  Widget() : me(this) {}
  // p. 2: "A useful heuristic to determine whether an expression is an lvalue
  // is to ask if you can take its address."
  Widget(Widget &&rhs) : me(&rhs) {}

  const Widget *me;
};

const Widget *wheres_this(const Widget& w) {
  return w.me;
}

int main(void) {
  Widget awidget;
  Widget another(std::move(awidget));
  assert(awidget.me == another.me);

  std::array<Widget, 3> yetmore{
      {std::move(awidget), std::move(awidget), std::move(awidget)}};
  for (auto &&w : yetmore) {
    assert(wheres_this(awidget) == wheres_this(w));
  }
}
