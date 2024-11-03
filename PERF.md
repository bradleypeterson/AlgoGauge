# PERF

PERF is a linux tool to be able to understand the preformance of a given process. 

https://man7.org/linux/man-pages/man2/perf_event_open.2.html#EXAMPLES


https://www.kernel.org/doc/html/latest/admin-guide/perf-security.html

perf_event_paranoid After discussing perf_event_paranoid should be set to -1. This allows use to use mutliple CPUs to analazye the preformace. It also allows not sudo process to use call perf events. This is really important as it remove the requirement to run Algogauge as sudo. 
`cat /proc/sys/kernel/perf_event_paranoid`

`echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid`

`echo "kernel.perf_event_paranoid = 0" | sudo tee -a /etc/sysctl.conf`
`sudo sysctl -p`
