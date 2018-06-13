
iperf -c 192.168.2.1 -t 30 &
./main $! -cpuset 1 -bandwidth 100Mbit

