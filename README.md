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
Unidirection tcp latency test

#Size(B)   #Average Latency(us)
 
1          8.722900   

2          8.846700   

4          10.170050  

8          9.858400   

16         9.252600   

32         9.202900   

64         9.986550   

128        10.087950  

256        9.997100   

512        10.259200  

1024       10.384000  

2048       12.292050  

4096       18.493900  

8192       25.004800  

16384      36.344400  

32768      64.622250  

65536      105.799700 

131072     175.048900 

262144     225.427850 

524288     390.528400 

1048576    710.780400 

2097152    1408.192600 

4194304    3141.392050 

