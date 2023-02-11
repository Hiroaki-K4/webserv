# webserv
This project is about writing HTTP server. You will be able to test it with an actual browser.

## How to build server
```
make
```

## How to run server
./webserv <port>
```
./webserv 8080
```

## How to build client
```
make test
```

## How to run client
./client <host> <port>
```
./client ::1 8080
```
You can request static contents from the server. In the example below, the server will return the contents of home.html
```
GET / HTTP/1.1
```
You can request dynamic contents from the server. In the example below, the server returns the result of an adder program (cgi) that adds two values.
```
GET /cgi-bin/adder?15000&213 HTTP/1.1
```
