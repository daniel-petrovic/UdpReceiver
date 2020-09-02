# UdpReceiver
Simple udp receiver based on boost::asio

Waits for udp packets on provided ports and prints out received packets along with timestamp.

Compilation:

mkdir build && cd build

conan install ..

cmake ..

cmake --build .


Usage:

./UdpReceiver <port 1> <port 2> <port 3> ...
  
 For ex.
 
 ./UdpReceiver 1234 2345 3456
 
 will wait for udp packets on ports 1234 2345 and 3456.

 
