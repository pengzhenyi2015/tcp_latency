# Function
test tcp_latency from end to end

# Principles
A simple tcp socket program, Read/Write socket 10000 iterations, then get the average latency.
The block size can range from 1 to 4M

#How to use
Start server progarm on one host, assign a port, for example 9999

./tcp_lat_server port

Start client program on the other host, assign server host's ip and the same port

./tcp_lat_client serverip port

#Result Example

