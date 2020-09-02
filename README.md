# GetFile-Proxy-Server
# Aim and design 
To implement handle_with_curl mechanism for the proxy server to connect to an external server through the url and serve the clients.

<image src="https://raw.githubusercontent.com/sreeganeshji/GetFile-Proxy-Server/master/illustration/p1.png" width=350>

The client interfaces with the proxy server which in turn interfaces with the main server which holds the data through the internet. The client requests data using the get file (gf) protocol. the proxy server uses the easy_curl api to connect with the server using its URL and acquire the data. The design choices involved making the handler multi-threaded to support multiple requests simultaneously.
# Flow of control 
The client uses the getFile protocol to request the proxy server for the file. It contains the name and location of the file. The proxy server translates this request into a web request by appending it with the URL of the main server. It transfers back the received files from the main server using the gf protocol to the client.
The implementation of the interaction between the proxy server and the web server is done using the easy_curl interface. A handler is created and initialized with the server URL, the write back function and the datatype for transfer.
Upon a data transmission from the web server to the proxy server, the curl interface calls the write back function with the received data. This function transfers the data into the local memory storing the whole file. This is then transferred to the client using the gf protocol.
# Implementation and testing
The implementation mainly consisted of interfacing the gf protocol for file transfer with the easy_curl interface for communication with the web server. A number of handler threads are initialized using the gf interface. The proxy listens for any requests from the clients on its address. The handlers receive the parsed information about the path and the URL information from the server.
The testing was primarily done by starting the server process and requesting multiple files as a client. It was tested for different types of possible failures like the 404 error from the web server, invalid url, and so on. The functionality was tested at different number of threads, sizes of files, etc.
# References
The details of the curl api were referred to from the curl.haxx.se website.
