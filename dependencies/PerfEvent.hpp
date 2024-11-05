/*

Copyright (c) 2018 Viktor Leis

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */

#pragma once

#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#if defined(__linux__)
#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>


#include <unistd.h>

struct PerfEvent
{

	struct event
	{
		struct read_format
		{
			uint64_t value;
			uint64_t time_enabled;
			uint64_t time_running;
			uint64_t id;
		};

		perf_event_attr pe;
		int fd;
		read_format prev;
		read_format data;

		double readCounter()
		{
			double multiplexingCorrection = static_cast<double>(data.time_enabled - prev.time_enabled) / static_cast<double>(data.time_running - prev.time_running);
			return static_cast<double>(data.value - prev.value) * multiplexingCorrection;
		}
	};

	enum EventDomain : uint8_t
	{
		USER = 0b1,
		KERNEL = 0b10,
		HYPERVISOR = 0b100,
		ALL = 0b111
	};

	std::vector<event> events;
	std::vector<std::string> names;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> stopTime;

	const std::vector<std::pair<std::string, double>> dummyData = {
		{"task-clock", 364444262.00},
		{"context switches", 214.00},
		{"cycles", 595348804.49},
		{"kcycles", 170739620.86},
		{"instructions", 1009631347.96},
		{"L1-misses", 5583046.54},
		{"LLC-misses", 897165.96},
		{"branch predictions", 182104579.70},
		{"branch-misses", 3082415.94},
		{"cache references", 5131660.31},
		{"retired branch instructions", 161551834.08},
		{"total page faults", 10020.00},
		{"minor page faults", 10020.00},
		{"major page faults", 0.00},
		{"context switches", 214.00},
		{"scale", 1.0},
		{"IPC", 1.70},
		{"CPUs", 0.37},
		{"GHz", 1.63}
	};

	/// @brief This is the constutor for PERF it holds all the attributes that should be tracked task-clock cycles etc.
	/// @param pid Optionally pass in the PID of what process track default 0 or the caller function
	PerfEvent(pid_t pid = 0){
		registerCounter("task-clock", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_TASK_CLOCK);
		registerCounter("context switches", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CONTEXT_SWITCHES);

		registerCounter("cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
		registerCounter("kcycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES, KERNEL);

		registerCounter("instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);

		registerCounter("L1-misses", PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_L1D | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16));
		registerCounter("LLC-misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES);
		registerCounter("branch predictions", PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_BPU);
		registerCounter("branch-misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_MISSES);

		registerCounter("cache references", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_REFERENCES);
		registerCounter("retired branch instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_INSTRUCTIONS);

		registerCounter("total page faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS);
		registerCounter("minor page faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS_MIN);
		registerCounter("major page faults", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_PAGE_FAULTS_MAJ);
		registerCounter("context switches", PERF_TYPE_SOFTWARE, PERF_COUNT_SW_CONTEXT_SWITCHES);

		// additional counters can be found in linux/perf_event.h

		// std::vector<std::pair<std::string, std::string>> allEvents = {
		//    {"PERF NOTE", "\"INCLUDED DATA IS DUMMY DATA!\""},
		//    {"cpu cycles", "5432316545"},
		//    {"bus cycles", "1561896"},
		//    {"cpu instructions", "5151651"},
		//    {"cache references", "198456156"},
		//    {"cache misses", "198415652"},
		//    {"branch predictions", "51894156489"},
		//    {"retired branch instructions", "98528445"},
		//    {"branch misses", "7415437"},
		//    {"total page faults", "574"},
		//    {"minor page faults", "242"},
		//    {"major page faults", "473"},
		//    {"context switches", "4"},
		//    {"L1 data cache read accesses", "369545"},
		//    {"L1 instruction cache read accesses", "841616"},
		//    {"L1 data cache prefetch accesses", "261485"},
		//    {"L1 instruction cache prefetch accesses", "2117485"},
		//    {"scale", "1"},

		//    {"IPC", "1.80"}
		// };
		for (unsigned i = 0; i < events.size(); i++){
			auto &event = events[i];
			event.fd = static_cast<int>(syscall(__NR_perf_event_open, &event.pe, pid, -1, -1, 0)); // int syscall(SYS_perf_event_open, struct perf_event_attr *attr, pid_t pid, int cpu, int group_fd, unsigned long flags);
			if (event.fd < 0){
				std::cerr << "Error opening counter " << names[i] << std::endl;
				events.resize(0);
				names.resize(0);
				return;
			}
		}
	}
	/// @brief Used to register your own events that aren't found in the constructor. Find more in linux/perf_event.h
	/// @param name The name of the event
	/// @param type The PERF_TYPE: Hardware, Software...
	/// @param eventID What is getting tracked ex: PERF_COUNT_HW_INSTRUCTIONS
	/// @param domain Where the event is found and tracked
	void registerCounter(const std::string &name, uint64_t type, uint64_t eventID, EventDomain domain = ALL){
		names.push_back(name);
		events.push_back(event());
		auto &event = events.back();
		auto &pe = event.pe;
		memset(&pe, 0, sizeof(struct perf_event_attr));
		pe.type = static_cast<uint32_t>(type);
		pe.size = sizeof(struct perf_event_attr);
		pe.config = eventID;
		pe.disabled = true;
		pe.inherit = 1;
		pe.inherit_stat = 0;
		pe.exclude_user = !(domain & USER);
		pe.exclude_kernel = !(domain & KERNEL);
		pe.exclude_hv = !(domain & HYPERVISOR);
		pe.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING;
	}

	/// @brief Start recording the registered counters
	void startCounters(){
		for (unsigned i = 0; i < events.size(); i++)
		{
			auto &event = events[i];
			ioctl(event.fd, PERF_EVENT_IOC_RESET, 0);
			ioctl(event.fd, PERF_EVENT_IOC_ENABLE, 0);
			if (read(event.fd, &event.prev, sizeof(uint64_t) * 3) != sizeof(uint64_t) * 3)
				std::cerr << "Error reading counter " << names[i] << std::endl;
		}
		startTime = std::chrono::steady_clock::now();
	
	}
	/// @brief Deconstrutor
	~PerfEvent(){
		for (auto &event : events)
		{
			close(event.fd);
		}
	}
	/// @brief Stop recording the registered counters
	void stopCounters(){
		stopTime = std::chrono::steady_clock::now();
		for (unsigned i = 0; i < events.size(); i++)
		{
			auto &event = events[i];
			if (read(event.fd, &event.data, sizeof(uint64_t) * 3) != sizeof(uint64_t) * 3)
				std::cerr << "Error reading counter " << names[i] << std::endl;
			ioctl(event.fd, PERF_EVENT_IOC_DISABLE, 0);
		}
	}

	/// @brief Get how long the Counters where recording
	/// @return The time
	double getDuration(){
		return std::chrono::duration<double>(stopTime - startTime).count();
	}
	/// @brief Get how many instructions per cylce where called
	/// @return A number
	double getIPC(){
		return getCounter("instructions") / getCounter("cycles");
	}

	/// @brief Get how many CPUs where used and how much
	/// @return A number
	double getCPUs(){
		return getCounter("task-clock") / (getDuration() * 1e9);
	}

	/// @brief Number of ticks per ms
	/// @return the number of ticks
	double getClockTicksPerMS(){
		return getCounter("task-clock");
	}

	/// @brief Computes the CPU frequency in GHz based on cycle and task clock counters.
	/// @return The computed frequency in GHz.
	double getGHz(){
		return getCounter("cycles") / getCounter("task-clock");
	}

	/// @brief Used to get the details of a specific counter
	/// @param name The name of the counter you want back
	/// @return What the counter recorded
	double getCounter(const std::string &name){
		for (unsigned i = 0; i < events.size(); i++)
			if (names[i] == name)
				return events[i].readCounter();
		return -1;
	}

	/// @brief A default method as part of the library
	/// @param headerOut
	/// @param dataOut
	/// @param name
	/// @param counterValue
	/// @param addComma
	static void printCounter(std::ostream &headerOut, std::ostream &dataOut, std::string name, std::string counterValue, bool addComma = true){
		auto width = std::max(name.length(), counterValue.length());
		headerOut << std::setw(static_cast<int>(width)) << name << (addComma ? "," : "") << " ";
		dataOut << std::setw(static_cast<int>(width)) << counterValue << (addComma ? "," : "") << " ";
	}

	/// @brief A default method as part of the library
	/// @tparam T
	/// @param headerOut
	/// @param dataOut
	/// @param name
	/// @param counterValue
	/// @param addComma
	template <typename T>
	static void printCounter(std::ostream &headerOut, std::ostream &dataOut, std::string name, T counterValue, bool addComma = true){
		std::stringstream stream;
		stream << std::fixed << std::setprecision(8) << counterValue;
		PerfEvent::printCounter(headerOut, dataOut, name, stream.str(), addComma);
	}

	/// @brief A default method as part of the library
	/// @param out
	/// @param normalizationConstant
	void printReport(std::ostream &out, uint64_t normalizationConstant){
		std::stringstream header;
		std::stringstream data;
		printReport(header, data, normalizationConstant);
		out << header.str() << std::endl;
		out << data.str() << std::endl;
	}

	/// @brief A default method as part of the library
	/// @param headerOut
	/// @param dataOut
	/// @param normalizationConstant
	void printReport(std::ostream &headerOut, std::ostream &dataOut, uint64_t normalizationConstant){
		if (!events.size())
			return;

		// print all metrics
		for (unsigned i = 0; i < events.size(); i++)
		{
			printCounter(headerOut, dataOut, names[i], events[i].readCounter() / static_cast<double>(normalizationConstant));
		}

		printCounter(headerOut, dataOut, "scale", normalizationConstant);

		// derived metrics
		printCounter(headerOut, dataOut, "IPC", getIPC());
		printCounter(headerOut, dataOut, "CPUs", getCPUs());
		printCounter(headerOut, dataOut, "GHz", getGHz(), false);
	}

	void printReportVertical(std::ostream &out, uint64_t normalizationConstant)
	{
		std::stringstream info;
		printReportVerticalUtil(info, normalizationConstant);
		out << info.str() << std::endl;
	}

private:
	template <typename T>
	static void printCounterVertical(std::ostream &infoOut, std::string name, T counterValue, int eNameWidth){
		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << counterValue;
		infoOut << std::setw(eNameWidth) << std::left << name << " : " << stream.str() << std::endl;
	}

	void printReportVerticalUtil(std::ostream &infoOut, uint64_t normalizationConstant){
		if (!events.size())
			return;

		// get width of the widest event name. Minimum width is the one of 'scale'
		int eNameWidth = 5;
		for (unsigned i = 0; i < events.size(); i++)
		{
			eNameWidth = std::max(static_cast<int>(names[i].length()), eNameWidth);
		}

		// print all metrics
		for (unsigned i = 0; i < events.size(); i++)
		{
			printCounterVertical(infoOut, names[i], events[i].readCounter() / static_cast<double>(normalizationConstant), eNameWidth);
		}

		printCounterVertical(infoOut, "scale", normalizationConstant, eNameWidth);

		// derived metrics
		printCounterVertical(infoOut, "IPC", getIPC(), eNameWidth);
		printCounterVertical(infoOut, "CPUs", getCPUs(), eNameWidth);
		printCounterVertical(infoOut, "GHz", getGHz(), eNameWidth);
	}

public:
	/// @brief Gets a vertical report with all the counters that were recorded. If not on linux returns the Dummy Data report.
	/// @param normalizationConstant Used to scale the results may be required depending on CPU
	/// @param precision How many decimal places should be kept
	/// @return A string
	std::string getPrintReport(uint64_t normalizationConstant = 1, uint8_t precision = 6){
		std::stringstream info;
		printReportVerticalUtil(info, normalizationConstant);
		return info.str() + "\n";

	}

	/// @brief The dummy data report is a run that was copied. And is printed for computers that aren't linux.
	/// @param normalizationConstant Nothing
	/// @param precision Nothing
	/// @return a vertical report of dummy perf data
	std::string getPerfRepotDummy(uint64_t normalizationConstant = 1, uint8_t precision = 6)
	{
		std::string report;

		int eNameWidth = 5;
		// Calculate the maximum width for the event names
		for (const auto &event : dummyData)
		{
			eNameWidth = std::max(static_cast<int>(event.first.length()), eNameWidth);
		}
		// Build the report string with formatted output
		for (const auto &event : dummyData)
		{
			std::ostringstream stream;
			stream << std::left << std::setw(eNameWidth) << event.first << " : ";

			// Normalize event.second and set precision
			double normalizedValue = event.second / normalizationConstant;
			stream << std::fixed << std::setprecision(precision) << normalizedValue << '\n';

			// Append the formatted string to report
			report += stream.str();
		}

		return report;
	}

	/// @brief Gets the perf results as a string JSON. If not on linux it will return the Dummy Data JSON string
	/// @param normalizationConstant A constant used to normalize the data values may be required depending on CPU (default is 1).
	/// @param precision The number of decimal places for floating-point values (default is 6).
	/// @return A JSON-formatted string representing the performance data.
	std::string getPerfJSONString(uint64_t normalizationConstant = 1, uint8_t precision = 6)
	{
		std::string jsonString = "{";
		for (unsigned i = 0; i < events.size(); i++)
		{
			std::stringstream stream;
			stream << std::setprecision(precision) << events[i].readCounter() / static_cast<double>(normalizationConstant);
			jsonString += "\"" + names[i] + "\":" + stream.str() + ",";
		}
		std::ostringstream stream;
		stream << "\"scale\":" << std::setprecision(precision) << normalizationConstant << ",";
		stream << "\"GHz\":" << std::fixed << std::setprecision(precision) << getGHz() << ",";
		stream << "\"IPC\":" << std::fixed << std::setprecision(precision) << getIPC() << ",";
		stream << "\"CPUs\":" << std::fixed << std::setprecision(precision) << getCPUs();

		jsonString += stream.str() + "}";

		return jsonString;
	}

	/// @brief Generates a JSON string from dummy performance data with optional normalization and precision.
	/// @param normalizationConstant Nothing (default is 1).
	/// @param precision Nothing (default is 6).
	/// @return A JSON-formatted string representing the performance data.
	std::string getPerfJSONStringDummy(uint64_t normalizationConstant = 1, uint8_t precision = 6)
	{
		std::string returnString = "{";
		size_t i = 0;

		for (const auto &event : dummyData)
		{
			std::ostringstream stream;
			stream << std::fixed << std::setprecision(precision) << event.second / normalizationConstant;
			returnString += "\"" + event.first + "\": " + stream.str();
			if (++i != dummyData.size())
				returnString += ", ";
		}
		returnString += "}";
		return returnString;
	}
};

#else

struct PerfEvent {

	PerfEvent(pid_t pid = 0){

	}
	const std::vector<std::pair<std::string, double>> dummyData = {
	{"task-clock", 364444262.00},
	{"context switches", 214.00},
	{"cycles", 595348804.49},
	{"kcycles", 170739620.86},
	{"instructions", 1009631347.96},
	{"L1-misses", 5583046.54},
	{"LLC-misses", 897165.96},
	{"branch predictions", 182104579.70},
	{"branch-misses", 3082415.94},
	{"cache references", 5131660.31},
	{"retired branch instructions", 161551834.08},
	{"total page faults", 10020.00},
	{"minor page faults", 10020.00},
	{"major page faults", 0.00},
	{"context switches", 214.00},
	{"scale", 1.0},
	{"IPC", 1.70},
	{"CPUs", 0.37},
	{"GHz", 1.63}};


   void startCounters() {}
   void stopCounters() {}
   void printReport(std::ostream&, uint64_t) {}
   template <class T> void setParam(const std::string&, const T&) {};
	double getDuration(){
		return 0;
	}
   std::string getPerfJSONString(uint64_t normalizationConstant = 1, uint8_t precision = 6)
	{
		return getPerfJSONStringDummy(normalizationConstant, precision);
	}

	std::string getPerfJSONStringDummy(uint64_t normalizationConstant = 1, uint8_t precision = 6)
	{
		std::string returnString = "{";
		size_t i = 0;

		for (const auto &event : dummyData)
		{
			std::ostringstream stream;
			stream << std::fixed << std::setprecision(precision) << event.second / normalizationConstant;
			returnString += "\"" + event.first + "\": " + stream.str();
			if (++i != dummyData.size())
				returnString += ", ";
		}
		returnString += "}";
		return returnString;
	}
};

#endif

/// Unused
#if defined(__linux__)

struct BenchmarkParameters
{

	void setParam(const std::string &name, const std::string &value)
	{
		params[name] = value;
	}

	void setParam(const std::string &name, const char *value)
	{
		params[name] = value;
	}

	template <typename T>
	void setParam(const std::string &name, T value)
	{
		setParam(name, std::to_string(value));
	}

	void printParams(std::ostream &header, std::ostream &data)
	{
		for (auto &p : params)
		{
			PerfEvent::printCounter(header, data, p.first, p.second);
		}
	}

	BenchmarkParameters(std::string name = "")
	{
		if (name.length())
			setParam("name", name);
	}

private:
	std::map<std::string, std::string> params;
};

struct PerfRef
{
	union
	{
		PerfEvent instance;
		PerfEvent *pointer;
	};
	bool has_instance;

	PerfRef() : instance(), has_instance(true) {}
	PerfRef(PerfEvent *ptr) : pointer(ptr), has_instance(false) {}
	PerfRef(const PerfRef &) = delete;

	~PerfRef()
	{
		if (has_instance)
			instance.~PerfEvent();
	}

	PerfEvent *operator->()
	{
		return has_instance ? &instance : pointer;
	}
};

struct PerfEventBlock
{
	PerfRef e;
	uint64_t scale;
	BenchmarkParameters parameters;
	bool printHeader;

	PerfEventBlock(uint64_t scale = 1, BenchmarkParameters params = {}, bool printHeader = true)
		: scale(scale),
		  parameters(params),
		  printHeader(printHeader)
	{
		e->startCounters();
	}

	PerfEventBlock(PerfEvent &perf, uint64_t scale = 1, BenchmarkParameters params = {}, bool printHeader = true)
		: e(&perf),
		  scale(scale),
		  parameters(params),
		  printHeader(printHeader)
	{
		e->startCounters();
	}

	~PerfEventBlock()
	{
		e->stopCounters();
		std::stringstream header;
		std::stringstream data;
		parameters.printParams(header, data);
		PerfEvent::printCounter(header, data, "time sec", e->getDuration());
		e->printReport(header, data, scale);
		if (printHeader)
			std::cout << header.str() << std::endl;
		std::cout << data.str() << std::endl;
	}
};

#else
struct BenchmarkParameters
{
};

struct PerfEventBlock
{
	PerfEventBlock(uint64_t = 1, BenchmarkParameters = {}, bool = true) {};
	PerfEventBlock(PerfEvent e, uint64_t = 1, BenchmarkParameters = {}, bool = true) {};
};
#endif