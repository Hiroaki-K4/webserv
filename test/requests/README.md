# HTTP1.1 request
An HTTP/1.1 message consists of a start-line followed by a CRLF and a sequence of octets in a format similar to the Internet Message Format: zero or more header field lines (collectively referred to as the "headers" or the "header section"), an empty line indicating the end of the header section, and an optional message body. See [RFC9112](https://httpwg.org/specs/rfc9112.html) and [RFC9110](https://httpwg.org/specs/rfc9110.html) for details.
```
HTTP-message = start-line(request line) CRLF
               *(field-line CRLF)
               CRLF
               [message-body]
```
<br></br>


## Request line
A request-line begins with a method token, followed by a single space (SP), the request-target, and another single space (SP), and ends with the protocol version.
```
request-line = [method] [SP] [request-target] [SP] [HTTP-version]
```
For example,
```
GET / HTTP/1.1
```

### **Method**
The following methods are supported on my server.
- GET
- POST
- DELETE

### **Request target**
The only origin-form request target is supported on my server. Recipients of an invalid request-line SHOULD respond with either a 400 (Bad Request) error or a 301 (Moved Permanently) redirect with the request-target properly encoded. A server MUST respond with a 400 (Bad Request) status code to any HTTP/1.1 request message that lacks a Host header field and to any request message that contains more than one Host header field line or a Host header field with an invalid field value.
```
origin-form = absolute-path [ "?" query ]
```
For example,
```
GET /where?q=now HTTP/1.1
Host: www.example.org
```

### **HTTP version**
My server only supports HTPP/1.1.
<br></br>


## Request header field
Each field line consists of a case-insensitive field name followed by a colon (":"), optional leading whitespace, the field line value, and optional trailing whitespace(OWS). A server MUST reject, with a response status code of 400 (Bad Request), any received request message that contains whitespace between a header field name and colon.
```
field-line = field-name ":" OWS field-value OWS
```
For example,
```
Accept: image/gif, image/jpeg, */*
Accept-Language: ja
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (Compatible; MSIE 6.0; Windows NT 5.1;)
Host: www.example.org
Connection: Keep-Alive
```
<br></br>


## Request message body
The presence of a message body in a request is signaled by a Content-Length or Transfer-Encoding header field. Request message framing is independent of method semantics.

### **Transfer-Encoding**
The Transfer-Encoding header field lists the transfer coding names corresponding to the sequence of transfer codings that have been (or will be) applied to the content in order to form the message body. A server MAY reject a request that contains both Content-Length and Transfer-Encoding or process such a request in accordance with the Transfer-Encoding alone. Regardless, the server MUST close the connection after responding to such a request to avoid the potential attacks.
```
Transfer-Encoding = #transfer-coding
```
For example,
```
Transfer-Encoding: gzip, chunked
```

### **Content-Length**
When a message does not have a Transfer-Encoding header field, a Content-Length header field can provide the anticipated size, as a decimal number of octets, for potential content.
A sender MUST NOT send a Content-Length header field in any message that contains a Transfer-Encoding header field.
<br></br>


## Request example
```
POST / HTTP/1.1
User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3
Host: www.example.com
Accept-Language: en, mi
Date: Mon, 27 Jul 2009 12:28:53 GMT
Server: Apache
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
ETag: "34aa387-d-1568eb00"
Accept-Ranges: bytes
Content-Length: 51
Vary: Accept-Encoding
Content-Type: text/plain

<html>
    <body>
        <h1>Hello, World!</h1>
    </body>
</html>
```
<br></br>


## How to parse HTTP request
1. Read the start-line into a structure.
2. Read each header field line into a hash table by field name until the empty line.
3. Use the parsed data to determine if a message body is expected.
4. If a message body has been indicated, then it is read as a stream until an amount of octets equal to the message body length is read or the connection is closed.
