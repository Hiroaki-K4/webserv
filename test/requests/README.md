# HTTP1.1 request
See [RFC9112](https://httpwg.org/specs/rfc9112.html) for details.
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
The only origin-form request target is supported on my server.
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

## Request header
<br></br>

## Request body
<br></br>

## Examples
<br></br>

## How to parse HTTP request
1. Read the start-line into a structure.
2. Read each header field line into a hash table by field name until the empty line.
3. Use the parsed data to determine if a message body is expected.
4. If a message body has been indicated, then it is read as a stream until an amount of octets equal to the message body length is read or the connection is closed.
