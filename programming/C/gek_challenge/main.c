const char* ip = "89.168.66.251";
const int port = 1337;

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
  struct sockaddr_in addr;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0); // ipv4 over tcp basically
  // make sure stuff worked
  if (sockfd == -1) {
    fprintf(stderr, "Socket failed to open for %s:%d\n", ip, port);
    return 1;
  } else {
    printf("Socket opened for %s:%d\n", ip, port);
  }

  // configure sockaddr struct trash
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  // convert the string IP to binary
  inet_pton(AF_INET, ip, &addr.sin_addr);

  int socket_status = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
  if (socket_status == -1) {
    fprintf(stderr, "Failed to connect to server at %s:%d\n", ip, port);
    return 1;
  } else {
    printf("Connected to server at %s:%d!\n", ip, port);
  }

  // 24 bytes + null terminator
  char buf[25];
  // read 24 bytes from server
  recv(sockfd, buf, 24, 0);
  printf("Data obtained: %s\n", buf);

  return 0;
}
