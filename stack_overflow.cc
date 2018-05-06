#include "/usr/include/malloc.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

/* adding test of macros from include/linux/kernel.h: */
#define _RET_IP_ (unsigned long)__builtin_return_address(0)
#define _THIS_IP_                                                              \
  ({                                                                           \
    __label__ __here;                                                          \
  __here:                                                                      \
    (unsigned long)&&__here;                                                   \
  })

using namespace std;

constexpr int ONEMEG = 1024 * 1024;
// The program simply SEGV's if this size is too large.
constexpr int BIGSIZE = 256 * 1024;

inline void print_mem(void) {
  struct mallinfo info = mallinfo();
  cout << "Total allocated space is " << (info.uordblks / 1000) << " kB."
       << endl;
  cout << "Total free space is " << (info.fordblks / 1000) << " kB." << endl;
  cout << "Total mmapped space is " << (info.hblks / 1000) << " kB." << endl;
  cout << "Total non-mmapped allocated space is " << (info.arena / 1000)
       << " kB." << endl
       << endl;
}

inline void print_mem_vec(vector<long> vec) {
  cout << "location of main() " << hex << "0x" << _RET_IP_ << dec << endl;
  long offset = (*(&vec.back()) - _RET_IP_);
  cout << "offset: " << hex << "0x" << offset << dec << endl << endl;
  print_mem();
}

inline void print_ave(vector<long> vec) {
  long ave = 0;
  for (vector<long>::iterator it = vec.begin(); it < vec.end(); it++) {
    ave += *it;
  }
  ave /= vec.size();
  cout << "with " << vec.size() << " items, ave is " << ave << endl;
  print_mem_vec(vec);
}

void print_ave(long arr[], size_t len) {
  long ave = 0;
  for (int i = 0; i < len; i++) {
    ave += arr[i];
  }
  ave /= len;
  cout << "array has average " << ave << " with size " << len << endl;
}

int main(void) {

  //  cout << "Start: " << endl;
  //print_mem();

  long randarr[BIGSIZE*256];
  for (int i = 0; i < BIGSIZE; i++) {
    randarr[i] = rand();
  }
  print_ave(randarr, BIGSIZE);
  cout << "Array: " << endl;
  print_mem();

  for (int j = ONEMEG; j *= 16; j < 256) {

    unique_ptr<vector<long>> vecp(new vector<long>);
    cout << "Unique ptr: " << endl;
    print_mem();

    vector<long> vec;
    for (int i = 0; i < j; i++) {
      vec.push_back(rand());
    }
    print_ave(vec);
    cout << "Vector on stack: " << endl;
    print_mem();

    for (int i = 0; i < j; i++) {
      vecp->push_back(rand());
    }
    print_ave(*vecp);
    cout << "Populate vector: " << endl;
    print_mem();
  }

  exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////
//[alison@bonnet Pohl (master)]$ gdb -q stack_overflow
//Reading symbols from stack_overflow...done.
//(gdb) run
//Starting program: /home/alison/src/exercises/Pohl/stack_overflow 
//[Thread debugging using libthread_db enabled]
//Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

//Program received signal SIGSEGV, Segmentation fault.
//0x0000555555559be4 in main () at stack_overflow.cc:61
//61        cout << "Start: " << endl;
// ^^^ actual stack overflow.
///
//	[alison@bonnet Pohl (master)]$ lpq				     //
//	HP_LaserJet_1022n is not ready					     //
//	no entries							     //
//	[alison@bonnet Pohl (master)]$ ./stack_overflow         	     //
//	Start: 								     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 131 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 204 kB.			     //
//									     //
//	array has average 1073759666 with size 262144			     //
//	Array: 								     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 131 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 204 kB.			     //
//									     //
//	Unique ptr: 							     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 131 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 204 kB.			     //
//									     //
//	with 16777216 items, ave is 1073792602				     //
//	location of main() 0x55833a5f0fc4				     //
//	offset: 0xffffaa7d3e56ed75					     //
//									     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 135 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 208 kB.			     //
//									     //
//	Vector on stack: 						     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 135 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 208 kB.			     //
//									     //
//	with 16777216 items, ave is 1073732900				     //
//	location of main() 0x55833a5f0fc4				     //
//	offset: 0xffffaa7d2d2d90b4					     //
//									     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 135 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 208 kB.			     //
//									     //
//	Populate vector: 						     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 135 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 208 kB.			     //
//									     //
//	Unique ptr: 							     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 135 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 208 kB.			     //
//									     //
//	with 268435456 items, ave is 1073713558				     //
//	location of main() 0x55833a5f0fc4				     //
//	offset: 0xffffaa7d3f194534					     //
//									     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 135 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 208 kB.			     //
//									     //
//	Vector on stack: 						     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 135 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 208 kB.			     //
//									     //
//	with 268435456 items, ave is 1073824664				     //
//	location of main() 0x55833a5f0fc4				     //
//	offset: 0xffffaa7cdabf9e2e					     //
//									     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 135 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 208 kB.			     //
//									     //
//	Populate vector: 						     //
//	Total allocated space is 73 kB.					     //
//	Total free space is 135 kB.					     //
//	Total mmapped space is 0 kB.					     //
//	Total non-mmapped allocated space is 208 kB.			     //
//									     //
//	stack_overflow.cc:72:26: runtime error: signed integer overflow: 268435456 * 16 cannot be represented in type 'int'//
//	[alison@bonnet Pohl (master)]$ 					     //
///////////////////////////////////////////////////////////////////////////////
