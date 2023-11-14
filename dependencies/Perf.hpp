#ifdef linux //only compile if on linux
#ifndef ALGOGAUGE_PERF_HPP
#define ALGOGAUGE_PERF_HPP

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
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::to_string;

namespace Perf {
    struct read_format {
        uint64_t nr; //the number of events
        struct {
            uint64_t value; //the value of the event
            uint64_t id; //if PERF_FORMAT_ID
        } values[3];
    };

    struct PerfEvent {
        string perfEventName;
        int fileDescriptor{0};
        uint64_t id{0};
        uint64_t value{0};
    };

    class Perf {
    private:
        char buffer[4096];
        struct read_format* rf;
        vector<struct PerfEvent> perfEvents;

    public:
        struct perf_event_attr pea;
        // constructors/destructors:
        Perf() {
            rf = (struct read_format*) buffer;
            memset(&pea, 0, sizeof(struct perf_event_attr));
        }

        ~Perf() = default;

        struct PerfEvent addNewPerfEvent(const string &perfEventName) {
            /*__NR_perf_event_open params:
             * 1: the perf_event_attr (pea) reference
             * 2, 3: pid/cpu:
             *     pid == 0, cpu == -1: This measures the calling process/thread on any CPU.
             *     pid == 0, cpu >= 0: This measures the calling process/thread only when running
             *       on the specified CPU.
             *      pid > 0, cpu == -1: This measures the specified process/thread on any CPU.
             *      pid > 0, cpu >= 0: This measures the specified process/thread only when
             *        running on the specified CPU.
             *      pid == -1, cpu >= 0: This measures all processes/threads on the specified CPU.
             *        This requires CAP_PERFMON (since Linux 5.8) or
             *        CAP_SYS_ADMIN capability or a
             *        /proc/sys/kernel/perf_event_paranoid value of less than 1.
             *      pid == -1, cpu == -1: This setting is invalid and will return an error.
             * 4: group_fd: allows event groups to be created. An event group has one event which is the gorup leader.
             *      The leader is created first, with group_fd = -1. The rest of the group members are created with
             *      subsequent perf_event_open() (in our case syscall(__NR_perf_event_open, &pea, ...)) calls with group_fd
             *      being set to the file descriptor of the group leader. (A single event on its own is created with
             *      group_fd = -1 and is considered to be a group with only 1 member).
             * 5: flags: is formed by OR-ing together zero or more of the following values:
             *      PERF_FLAG_FD_CLOEXEC, PERF_FLAG_FD_NO_GROUP, PERF_FLAG_FD_OUTPUT, PERF_FLAG_PID_CGROUP
             */
            int fileDescriptor = syscall(
                    __NR_perf_event_open, //system call
                    &pea, //perf event attr object
                    0, // pid
                    -1, // cpu
                    getFirstFileDescriptor(),// group_fd
                    0 // flags
            );
            struct PerfEvent newPerfEvent {perfEventName, fileDescriptor};
            ioctl(newPerfEvent.fileDescriptor, PERF_EVENT_IOC_ID, &newPerfEvent.id);
            perfEvents.push_back(newPerfEvent);

            memset(&pea, 0, sizeof(struct perf_event_attr)); //reset pea

            return newPerfEvent;
        }

        struct PerfEvent addNewPerfEvent(const string &perfEventName, uint64_t peaType, uint64_t pea_hw_id) {
            memset(&pea, 0, sizeof(struct perf_event_attr)); //reset pea
            pea.type = peaType;
            pea.size = sizeof(struct perf_event_attr);
            pea.config = pea_hw_id;
            pea.disabled = 1;
            pea.exclude_kernel = 1;
            pea.exclude_hv = 1;
            pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
            return addNewPerfEvent(perfEventName);
        }

        int getFirstFileDescriptor() {
            return perfEvents.empty() ? -1 : perfEvents[0].fileDescriptor;
        }

        void readBuffer() {
            read(getFirstFileDescriptor(), buffer, sizeof(buffer));
            for (int i = 0; i < rf->nr; i++) {
                for (PerfEvent &perfEvent : perfEvents) {
                    if (rf->values[i].id == perfEvent.id) perfEvent.value = rf->values[i].value;
                }
            }
        }

        void printBuffer() {
            printf("%s", getBufferString().c_str());
        }

        string getBufferString() {
            string finalString;
            for (PerfEvent &perfEvent : perfEvents) {
                finalString += string(perfEvent.perfEventName) + ": " + to_string(perfEvent.value) + " ";
            }
            return finalString;
        }

        string getBufferJSON() {
            string finalJSON = "{";
            for (PerfEvent &perfEvent : perfEvents) {
                finalJSON += "\"" + string(perfEvent.perfEventName) + "\": " + to_string(perfEvent.value) + ",";
            }
            finalJSON.pop_back();
            finalJSON += "}";

            return finalJSON;
        }
    };
}

#endif //ALGOGAUGE_PERF_HPP
#endif //linux