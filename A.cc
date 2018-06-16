namespace {

int foo(int i) { return (i * 3); }
}

int goo(int i) { return (i * foo(i)); }
