#include <arpa/inet.h>   // for inet_pton()
#include <netinet/in.h>  // for sockaddr_in
#include <stdio.h>
#include <stdlib.h>      // for exit(), system()
#include <string.h>      // for memset() if you replace bzero()
#include <strings.h>     // for bzero()
#include <sys/socket.h>  // for socket functions
#include <unistd.h>      // for close()

int main() {
  unsigned short port = 2000;
  // char *server_ip = "10.26.57.8";      // 应该发送到对应IP的网卡地址: 10.26.57.3
  char *server_ip = "localhost";  // ✅ 两个程序在同一个服务器上跑就可
  // char *server_ip = "10.26.57.3";  // ✅ 网卡地址能接收到

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket error");
    exit(-1);
  }

  struct sockaddr_in server_addr;
  // bzero <==> memset(&server_addr, 0, sizeof(server_addr));
  bzero(&server_addr, sizeof(server_addr));  // 初始化服务器地址
  // AF_INET:  IPv4
  // AF_INET6: IPv6
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  // inet_pton() 是一个通用的地址转换函数，不知道你打算转换成 IPv4 还是 IPv6，必须由你告诉它目标地址族
  // AF_INET:  IPv4
  // AF_INET6: IPv6
  // 客户端绑定具体服务端IP必须使用该方法
  inet_pton(AF_INET, server_ip, &server_addr.sin_addr.s_addr);
  system("netstat -ano | grep 2000");

  // int connect(int __fd, const struct sockaddr *__addr, socklen_t __len)
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("connect error");
    close(sockfd);
    exit(-1);
  }

  system("netstat -ano | grep 2000");
  
  while (1) {
    printf("请输入要发送的内容：");
    char buffer[1024];
    fgets(buffer, sizeof(buffer), stdin);

    // ssize_t send(int __fd, const void *__buf, size_t __n, int __flags)
    int count = send(sockfd, buffer, sizeof(buffer), 0);
    if (count < 0) {
      perror("send error");
      close(sockfd);
      exit(-1);
    }
    printf("SEND: %d\n", count);
  }
  
  return 0;
}