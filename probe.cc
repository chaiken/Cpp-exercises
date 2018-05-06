/*
 * 2013 (c) Mentor Graphics Corporation
 *
 */

/* adding test of macros from include/linux/kernel.h: */
#define _RET_IP_                (unsigned long)__builtin_return_address(0)
#define _THIS_IP_  ({ __label__ __here; __here: (unsigned long)&&__here; })

#include <iostream>
#include <string>
#include <cstdlib>
#include <pthread.h>

struct options
{
  options(int argc, char **argv)
  {
    if (argc != 2) //usage(argv[0], "wrong number of arguments");
      testcase = "all";
    else testcase = argv[1];
  }
  static void usage(std::string const &prog_name, std::string const &error = std::string())
  {
    std::ostream &os = error.empty() ? std::cout : std::cerr;
    if (!error.empty()) os << error << std::endl;
    os << "Usage: " << prog_name << " <testcase> " << std::endl;
    if (error.empty()) std::exit(0);
    else std::exit(-1);
  }

  std::string testcase;
};

void memcheck()
{
  std::cout << "In " << __func__ << " at " << (void *)_THIS_IP_  << ", called from " << (void *)_RET_IP_ << std::endl;
  char *buffer = static_cast<char*>(std::malloc(1024));
  buffer = static_cast<char*>(std::realloc(buffer, 512));
  std::free(buffer);
  buffer = static_cast<char*>(std::calloc(1024, sizeof(char)));
  std::free(buffer);
}

void lock()
{
  static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&mutex);
  pthread_mutex_unlock(&mutex);
}

void all()
{
  memcheck();
  lock();
  std::cout << "In " << __func__ << " at " << (void *)_THIS_IP_  << ", called from " << (void *)_RET_IP_ << std::endl;
}

int main(int argc, char **argv)
{
  options opts(argc, argv);
  if (opts.testcase == "memcheck") memcheck();
  else if (opts.testcase == "lock") lock();
  else all();
  std::cout << "In " << __func__ << " at " << (void *)_THIS_IP_  << ", called from " << (void *)_RET_IP_ << std::endl;
  exit(0);
}

//////////////////////////////////////////////////////////////////////////////
//									     //
//	[alison@hildesheim src (master)]$ g++ -O2 -g -ggdb -Wall -Wextra -fsanitize=undefined probe.cc -o probe//
//	[alison@hildesheim src (master)]$ ./probe			     //
//	In memcheck at 0x4012b0, called from 0x401459			     //
//	In all at 0x401450, called from 0x401130			     //
//	In main at 0x40109f, called from 0x7f3382ff9b45			     //
//	[alison@hildesheim src (master)]$ addr2line -e ./probe 0x4012b0	     //
//	/home/alison/src/probe.cc:36					     //
//	[alison@hildesheim src (master)]$ nm -l ./probe | grep 4012b0	     //
//	00000000004012b0 T _Z8memcheckv /home/alison/src/probe.cc:35	     //
//////////////////////////////////////////////////////////////////////////////
