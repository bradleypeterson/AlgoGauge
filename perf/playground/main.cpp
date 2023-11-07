//#define _GNU_SOURCE
/*
 * docker instructions:
 * docker build . -t perf:latest
 * docker run --rm -m 150m --name perf perf
 * or
 * create a docker toolchain in clion using https://www.jetbrains.com/help/clion/clion-toolchains-in-docker.html#build-run-debug-docker
 */

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>

using std::cout;
using std::endl;


struct read_format {
  uint64_t nr;
  struct {
    uint64_t value;
    uint64_t id;
  } values[3];
};

void do_something() {
  int i;

  // char* ptr = (char*)malloc(100*1024*1024);
  // for (i = 0; i < 100*1024*1024; i++) {
  //   ptr[i] = (char) (i & 0xff); // pagefault
  // }
  // free(ptr);
  const int N = 10000;
  char* arr = new char[N];
  for (unsigned int round = 0; round < N - 1; round++) {
		for (unsigned int i = 0; i < N - 1 - round; i++) {
			if (arr[i + 1] < arr[i]) {
				auto temp = arr[i];
				arr[i] = arr[i + 1];
			  arr[i + 1] = temp;
			}
		}
	}

  for (int i = 0; i < N; i++) {
    if (arr[i] > arr[i+1]) {
      printf("Problem at %d\n", i);
    }
  }
  delete[] arr;

  // char* ptr = (char*)malloc(100*1024*1024);
  // for (i = 0; i < 100*1024*1024; i++) {
  //   ptr[i] = (char) (i & 0xff); // pagefault
  // }
  // free(ptr);
}

int main(int argc, char* argv[]) {
  struct perf_event_attr pea;
  int fd1{0}, fd2{0}, fd3{0}, fd4{0}, fd5{0}, fd6{0};
  uint64_t id1{0}, id2{0}, id3{0}, id4{0}, id5{0}, id6{0};
  uint64_t val1{0}, val2{0}, val3{0}, val4{0}, val5{0}, val6{0};
  char buf[4096];
  struct read_format* rf = (struct read_format*) buf;
  int i;

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_HARDWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_CPU_CYCLES;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd1 = syscall(__NR_perf_event_open, &pea, 0, -1, -1, 0);
  ioctl(fd1, PERF_EVENT_IOC_ID, &id1);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_SOFTWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_SW_PAGE_FAULTS;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd2 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd2, PERF_EVENT_IOC_ID, &id2);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_HARDWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_INSTRUCTIONS;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd3 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd3, PERF_EVENT_IOC_ID, &id3);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_HW_CACHE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_CACHE_BPU;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd4 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd4, PERF_EVENT_IOC_ID, &id4);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_HARDWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd5 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd5, PERF_EVENT_IOC_ID, &id5);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_HARDWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_BRANCH_MISSES;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd6 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd6, PERF_EVENT_IOC_ID, &id6);

  ioctl(fd1, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
  ioctl(fd1, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
  do_something();
  ioctl(fd1, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

  std::cout << "before read" << endl;
  read(fd1, buf, sizeof(buf));
  cout << "size of rf->nr: " << rf->nr << endl;
  try {
      for (i = 0; i < rf->nr; i++) {
          cout << "current i: " << i << endl;
        if (rf->values[i].id == id1) {
          cout << "Found id1" << endl;
          val1 = rf->values[i].value;
        } else if (rf->values[i].id == id2) {
          cout << "Found id2" << endl;
//          val2 = rf->values[i].value;
        } else if (rf->values[i].id == id3) {
          cout << "Found id3" << endl;
//          val3 = rf->values[i].value;
        } else if (rf->values[i].id == id4) {
          cout << "Found id4" << endl;
//          val4 = rf->values[i].value;
        } else if (rf->values[i].id == id5) {
          cout << "Found id5" << endl;
//          val5 = rf->values[i].value;
        } else if (rf->values[i].id == id6) {
          cout << "Found id6" << endl;
//          val6 = rf->values[i].value;
        }
        else {
            cout <<"Found none" << endl;
        }
      }

      cout << "passed loops" << endl;

      printf("cpu cycles: %" PRIu64 "\n", val1);
      printf("page faults: %" PRIu64 "\n", val2);
      printf("cpu instructions: %" PRIu64 "\n", val3);
      printf("branch prediction something: %" PRIu64 "\n", val4);
      printf("retired branch instructions: %" PRIu64 "\n", val5);
      printf("branch misses: %" PRIu64 "\n", val6);
  } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
  }
  return 0;
}
