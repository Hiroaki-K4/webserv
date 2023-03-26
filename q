[1mdiff --git a/srcs/HttpResponse.cpp b/srcs/HttpResponse.cpp[m
[1mindex a390147..b6925d2 100644[m
[1m--- a/srcs/HttpResponse.cpp[m
[1m+++ b/srcs/HttpResponse.cpp[m
[36m@@ -6,7 +6,7 @@[m
 /*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */[m
 /*                                                +#+#+#+#+#+   +#+           */[m
 /*   Created: 2023/03/26 14:03:34 by hkubo             #+#    #+#             */[m
[31m-/*   Updated: 2023/03/26 14:17:50 by hkubo            ###   ########.fr       */[m
[32m+[m[32m/*   Updated: 2023/03/26 14:31:06 by hkubo            ###   ########.fr       */[m
 /*                                                                            */[m
 /* ************************************************************************** */[m
 [m
[36m@@ -14,8 +14,14 @@[m
 [m
 HttpResponse::HttpResponse() {}[m
 [m
[32m+[m[32mHttpResponse::HttpResponse(int conn_fd) { set_conn_fd(conn_fd); }[m
[32m+[m
 HttpResponse::~HttpResponse() {}[m
 [m
[32m+[m[32mvoid HttpResponse::set_conn_fd(int conn_fd) { this->conn_fd = conn_fd; }[m
[32m+[m
[32m+[m[32mint HttpResponse::get_conn_fd() { return this->conn_fd; }[m
[32m+[m
 int HttpResponse::parse_uri(char *uri, char *filename, char *cgiargs) {[m
     char *ptr;[m
 [m
[36m@@ -52,7 +58,7 @@[m [mvoid HttpResponse::get_filetype(char *filename, char *filetype) {[m
         strcpy(filetype, "text/plain");[m
 }[m
 [m
[31m-void HttpResponse::serve_static(int fd, char *filename, int filesize) {[m
[32m+[m[32mvoid HttpResponse::serve_static(char *filename, int filesize) {[m
     int src_fd;[m
     char *srcp, filetype[MAXLINE];[m
 [m
[36m@@ -65,7 +71,7 @@[m [mvoid HttpResponse::serve_static(int fd, char *filename, int filesize) {[m
     out = ss.str();[m
     char resp_head[out.length() + 1];[m
     strcpy(resp_head, out.c_str());[m
[31m-    if (rio_writen(fd, resp_head, strlen(resp_head)) == -1) {[m
[32m+[m[32m    if (rio_writen(get_conn_fd(), resp_head, strlen(resp_head)) == -1) {[m
         std::cout << "rio_writen error!" << std::endl;[m
         return;[m
     }[m
[36m@@ -79,33 +85,33 @@[m [mvoid HttpResponse::serve_static(int fd, char *filename, int filesize) {[m
     }[m
     srcp = static_cast<char *>(mmap(0, filesize, PROT_READ, MAP_PRIVATE, src_fd, 0));[m
     close(src_fd);[m
[31m-    rio_writen(fd, srcp, filesize);[m
[32m+[m[32m    rio_writen(get_conn_fd(), srcp, filesize);[m
     munmap(srcp, filesize);[m
 }[m
 [m
[31m-void HttpResponse::serve_dynamic(int fd, char *filename, char *cgiargs) {[m
[32m+[m[32mvoid HttpResponse::serve_dynamic(char *filename, char *cgiargs) {[m
     char buf[MAXLINE], *emptylist[] = {NULL};[m
 [m
     // Return first part of HTTP response[m
     sprintf(buf, "HTTP/1.0 200 OK\r\n");[m
[31m-    rio_writen(fd, buf, strlen(buf));[m
[32m+[m[32m    rio_writen(get_conn_fd(), buf, strlen(buf));[m
     sprintf(buf, "Server: Tiny Web Server\r\n");[m
[31m-    rio_writen(fd, buf, strlen(buf));[m
[32m+[m[32m    rio_writen(get_conn_fd(), buf, strlen(buf));[m
 [m
     if (fork() == 0) {[m
         setenv("QUERY_STRING", cgiargs, 1);[m
[31m-        dup2(fd, STDOUT_FILENO);[m
[32m+[m[32m        dup2(get_conn_fd(), STDOUT_FILENO);[m
         execve(filename, emptylist, environ);[m
     }[m
     wait(NULL);[m
 }[m
 [m
[31m-void HttpResponse::serve_contents(int fd) {[m
[32m+[m[32mvoid HttpResponse::serve_contents() {[m
     char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];[m
     char filename[MAXLINE], cgiargs[MAXLINE];[m
 [m
     rio_t rio;[m
[31m-    rio_readinitb(&rio, fd);[m
[32m+[m[32m    rio_readinitb(&rio, get_conn_fd());[m
     rio_readlineb(&rio, buf, MAXLINE, true);[m
     std::cout << "Request headers:" << std::endl;[m
     std::cout << buf;[m
[36m@@ -137,12 +143,12 @@[m [mvoid HttpResponse::serve_contents(int fd) {[m
             std::cout << "403 Forbidden: Tiny couldn't read the file" << std::endl;[m
             return;[m
         }[m
[31m-        serve_static(fd, filename, sbuf.st_size);[m
[32m+[m[32m        serve_static(filename, sbuf.st_size);[m
     } else {[m
         if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {[m
             std::cout << "403 Forbidden: Tiny couldn't run the CGI program" << std::endl;[m
             return;[m
         }[m
[31m-        serve_dynamic(fd, filename, cgiargs);[m
[32m+[m[32m        serve_dynamic(filename, cgiargs);[m
     }[m
 }[m
[1mdiff --git a/srcs/HttpResponse.hpp b/srcs/HttpResponse.hpp[m
[1mindex 83e0592..1038a78 100644[m
[1m--- a/srcs/HttpResponse.hpp[m
[1m+++ b/srcs/HttpResponse.hpp[m
[36m@@ -6,7 +6,7 @@[m
 /*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */[m
 /*                                                +#+#+#+#+#+   +#+           */[m
 /*   Created: 2023/03/26 14:03:45 by hkubo             #+#    #+#             */[m
[31m-/*   Updated: 2023/03/26 14:15:29 by hkubo            ###   ########.fr       */[m
[32m+[m[32m/*   Updated: 2023/03/26 14:30:21 by hkubo            ###   ########.fr       */[m
 /*                                                                            */[m
 /* ************************************************************************** */[m
 [m
[36m@@ -19,16 +19,21 @@[m
 class HttpResponse {[m
    public:[m
     HttpResponse();[m
[32m+[m[32m    HttpResponse(int conn_fd);[m
     ~HttpResponse();[m
 [m
[32m+[m[32m    void set_conn_fd(int conn_fd);[m
[32m+[m[32m    int get_conn_fd();[m
[32m+[m
     int parse_uri(char *uri, char *filename, char *cgiargs);[m
     void get_filetype(char *filename, char *filetype);[m
[31m-    void serve_static(int fd, char *filename, int filesize);[m
[31m-    void serve_dynamic(int fd, char *filename, char *cgiargs);[m
[31m-    void serve_contents(int fd);[m
[32m+[m[32m    void serve_static(char *filename, int filesize);[m
[32m+[m[32m    void serve_dynamic(char *filename, char *cgiargs);[m
[32m+[m[32m    void serve_contents();[m
 [m
 [m
    private:[m
[32m+[m[32m    int conn_fd;[m
 };[m
 [m
 #endif[m
[1mdiff --git a/srcs/HttpServer.cpp b/srcs/HttpServer.cpp[m
[1mindex 6635e23..5f27aad 100644[m
[1m--- a/srcs/HttpServer.cpp[m
[1m+++ b/srcs/HttpServer.cpp[m
[36m@@ -6,7 +6,7 @@[m
 /*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */[m
 /*                                                +#+#+#+#+#+   +#+           */[m
 /*   Created: 2023/03/19 14:17:30 by hkubo             #+#    #+#             */[m
[31m-/*   Updated: 2023/03/26 14:17:01 by hkubo            ###   ########.fr       */[m
[32m+[m[32m/*   Updated: 2023/03/26 14:28:43 by hkubo            ###   ########.fr       */[m
 /*                                                                            */[m
 /* ************************************************************************** */[m
 [m
[36m@@ -33,8 +33,8 @@[m [mvoid HttpServer::simple_server_run() {[m
         char host_name[MAXLINE], port[MAXLINE];[m
         getnameinfo((sockaddr *)&clientaddr, client_len, host_name, MAXLINE, port, MAXLINE, 0);[m
         std::cout << "Accepted connection from " << host_name << ":" << port << std::endl;[m
[31m-        HttpResponse resp;[m
[31m-        resp.serve_contents(conn_fd);[m
[32m+[m[32m        HttpResponse resp(conn_fd);[m
[32m+[m[32m        resp.serve_contents();[m
         close(conn_fd);[m
     }[m
 }[m
[36m@@ -56,8 +56,8 @@[m [mvoid HttpServer::multiple_io_server_run() {[m
             char host_name[MAXLINE], port[MAXLINE];[m
             getnameinfo((sockaddr *)&clientaddr, client_len, host_name, MAXLINE, port, MAXLINE, 0);[m
             std::cout << "Accepted connection from " << host_name << ":" << port << std::endl;[m
[31m-            HttpResponse resp;[m
[31m-            resp.serve_contents(conn_fd);[m
[32m+[m[32m            HttpResponse resp(conn_fd);[m
[32m+[m[32m            resp.serve_contents();[m
             close(conn_fd);[m
         }[m
     }[m
