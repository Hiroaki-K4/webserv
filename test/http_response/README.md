## Status codes
The status code of a response is a three-digit integer code that describes the result of the request and the semantics of the response, including whether the request was successful and what content is enclosed (if any). All valid status codes are within the range of 100 to 599, inclusive.

The first digit of the status code defines the class of response. The last two digits do not have any categorization role. There are five values for the first digit:

- [1xx](https://httpwg.org/specs/rfc9110.html#status.1xx) (Informational): The request was received, continuing process
- [2xx](https://httpwg.org/specs/rfc9110.html#status.2xx) (Successful): The request was successfully received, understood, and accepted
- [3xx](https://httpwg.org/specs/rfc9110.html#status.3xx) (Redirection): Further action needs to be taken in order to complete the request
- [4xx](https://httpwg.org/specs/rfc9110.html#status.4xx) (Client Error): The request contains bad syntax or cannot be fulfilled
- [5xx](https://httpwg.org/specs/rfc9110.html#status.5xx) (Server Error): The server failed to fulfill an apparently valid request

<br></br>
## The status codes used by this server
- **200 OK**
  - The 200 (OK) status code indicates that the request has succeeded.
- **400 Bad Request**
  - The 400 (Bad Request) status code indicates that the server cannot or will not process the request due to something that is perceived to be a client error.
- **403 Forbidden**
  - The 403 (Forbidden) status code indicates that the server understood the request but refuses to fulfill it. A server that wishes to make public why the request has been forbidden can describe that reason in the response content (if any).
- **404 Not Found**
  - The 404 (Not Found) status code indicates that the origin server did not find a current representation for the target resource or is not willing to disclose that one exists.
- **500 Internal Server Error**
  - The 500 (Internal Server Error) status code indicates that the server encountered an unexpected condition that prevented it from fulfilling the request.
- **501 Not Implemented**
  - The 501 (Not Implemented) status code indicates that the server does not support the functionality required to fulfill the request. This is the appropriate response when the server does not recognize the request method and is not capable of supporting it for any resource.

<br></br>
## Reference
- [RFC9110 Status codes](https://httpwg.org/specs/rfc9110.html#status.codes)