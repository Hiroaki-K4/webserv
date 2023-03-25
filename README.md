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
If you ccess http://localhost:8080/, you will get static page like the image below.

<img src='images/satsuma_hog.png' width='500'>

<br></br>
If you access http://localhost:8080/cgi/cgi?Gryffindor, you will get dynamic page.

<img src='images/satsuma_img.png' width='500'>

<br></br>
## How to build client
```
make client
```

## How to run client
./client [host] [port] [use_stdin] [repeat_times]
```
./client ::1 8080 1 0
```
You can request static contents from the server. In the example below, the server will return the contents of home.html
```
GET / HTTP/1.1
```
You can request dynamic contents from the server. In the example below, the server returns the result of an cgi program that decide on your dormitory at Hogwarts.
```
GET /cgi/cgi?Gryffindor HTTP/1.1
```
<br></br>

## Reference
- RFC9110([en](https://httpwg.org/specs/rfc9110.html) / [ja](https://triple-underscore.github.io/http-semantics-ja.html#protocol.version))
- RFC9112([en](https://httpwg.org/specs/rfc9112.html) / [ja](https://triple-underscore.github.io/http1-ja.html))
- [Computer Systems: A Programmer's Perspective chapter11, 12](https://csapp.cs.cmu.edu/)
- [C++でHTTPサーバを作った話](https://nafuka.hatenablog.com/entry/2022/04/14/194200)
- [C++でHTTPサーバーを作った](https://jun-networks.hatenablog.com/entry/2022/12/05/234522)
