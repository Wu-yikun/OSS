#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void* client_thread(void* arg) {
  int client_fd = *(int*)arg;
  free(arg);

  while (1) {
    char buffer[1024] = {0};
    int count = recv(client_fd, buffer, sizeof(buffer), 0);

    if (count < 0) {
      break;  // 客户端关闭或出错
    }

    printf("RECV: %s\n", buffer);
    count = send(client_fd, buffer, count, 0);
    printf("SEND: %d\n", count);
  }

  close(client_fd);
  return NULL;
}

int main() {
  // int socket(int __domain, int __type, int __protocol)
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 0.0.0.0
  server_addr.sin_port = htons(2000);               // system used: 0 ~ 1023

  if (bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
    printf("bind failed: %s\n", strerror(errno));
  }

  // int listen(int __fd, int __n): 后者为等待队列的长度
  listen(sock_fd, 10);
  printf("listen finished: %d\n", sock_fd);  // 3
  system("netstat -ano | grep 2000");
  // getchar();  // 方便查看 netstat -ano | grep 2000: tcp  0    0 0.0.0.0:2000   0.0.0.0:*   LISTEN   off (0.00/0/0)

  struct sockaddr_in client_addr;
  // int accept(int __fd, struct sockaddr *__restrict__ __addr, socklen_t *__restrict__ __addr_len)
  socklen_t len = sizeof(client_addr);
#if 0  // version 1.0: 单次收发
//     int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &len);
//     printf("accept finished\n");
//     system("netstat -ano | grep 2000");

//     char buffer[1024] = {0};
//     // ssize_t recv(int __fd, void *__buf, size_t __n, int __flags)
//     int count = recv(client_fd, buffer, sizeof(buffer), 0);
//     printf("RECV: %s\n", buffer);

//     // ssize_t send(int __fd, const void *__buf, size_t __n, int __flags)
//     count = send(client_fd, buffer, count, 0);
//     printf("SEND: %d\n", count);

#elif 0  // version 2.0: 循环收发（但是如果 1,2,3 顺序连接，且 3,2,1 客户端顺序发数据，则存在问题）

  //   while (1) {
  //     int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &len);
  //     system("netstat -ano | grep 2000");

  //     char buffer[1024] = {0};
  //     int count = recv(client_fd, buffer, sizeof(buffer), 0);
  //     printf("RECV: %s\n", buffer);

  //     count = send(client_fd, buffer, count, 0);
  //     printf("SEND: %d\n", count);
  //   }

#else  // version 3.0: 开线程 pthread

  while (1) {
    int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &len);

    // 为每个县城申请分配独立的 client_fd
    int* p_client = malloc(sizeof(int));
    *p_client = client_fd;

    system("netstat -ano | grep 2000");

    pthread_t pid;
    pthread_create(&pid, NULL, client_thread, p_client);
  }

#endif

  return 0;
}