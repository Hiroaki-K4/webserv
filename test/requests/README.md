# HTTP request
<br></br>

## Request line
<br></br>

## Request header
<br></br>

## Request body
<br></br>

## Examples
<br></br>

## How to parse HTTP request
### 1. Read the start-line into a structure.
### 2. Read each header field line into a hash table by field name until the empty line.
### 3. Use the parsed data to determine if a message body is expected.
### 4. If a message body has been indicated, then it is read as a stream until an amount of octets equal to the message body length is read or the connection is closed.
