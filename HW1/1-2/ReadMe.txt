1. Server Mode
執行：在Terminal裏面執行 Iperfer -s -p <listen port>
執行範例：./Iperfer -s -p 12345

輸出範例： 
Waiting for connection...
Accepted connection from 127.0.0.1
Receiving data...
Received=2856739 KB | Rate=7617.97 Mbps

2. Client Mode
執行：在Terminal裏面執行 Iperfer -c -h <server hostname> -p <server port> -t <time>
執行範例：./Iperfer -c -h 127.0.0.1 -p 12345 -t 3

輸出範例： 
connecting to 127.0.0.1:12345...
Connected
Sending data...
Sent = 2856739 KB | Rate = 7617.97 Mbps


Iperfer編譯：在Terminal裏面執行 g++ *.cpp -o Iperfer

