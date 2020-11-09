#include "packet-format.h"
#include <arpa/inet.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: file-receiver <file> <port> <window size>\n");
    exit(EXIT_FAILURE);
  }
  char *file_name = argv[1];
  int port = atoi(argv[2]);
  int window_size = atoi(argv[3]);

  FILE *file = fopen(file_name, "w");
  if (!file) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  if (port < 0 || strlen(argv[2]) != 4) {
    perror("port");
    exit(EXIT_FAILURE);
  }

  if (window_size < 1) {
    perror("window");
    exit(EXIT_FAILURE);
  }

  // Prepare server socket.
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // Allow address reuse so we can rebind to the same port,
  // after restarting the server.
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) <
      0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in srv_addr = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(INADDR_ANY),
      .sin_port = htons(port),
  };
  if (bind(sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr))) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  fprintf(stderr, "Receiving on port: %d\n", port);

  ssize_t len;
  uint32_t seq_expected = 0;
  uint32_t bit_vector = 0;
  ack_pkt_t ack_pkt;
  do { // Iterate over segments, until last the segment is detected.
    // Receive segment.
    struct sockaddr_in src_addr;
    data_pkt_t data_pkt;

    len =
        recvfrom(sockfd, &data_pkt, sizeof(data_pkt), 0,
                 (struct sockaddr *)&src_addr, &(socklen_t){sizeof(src_addr)});

    // Confirmation that received the package     
    printf("Server: Received segment %d, size %ld.\n", ntohl(data_pkt.seq_num), len);
    u_int32_t seq_num_rec = ntohl(data_pkt.seq_num);

    if (seq_num_rec < seq_expected + window_size) {
      fseek(file, seq_num_rec*1000, SEEK_SET);
      fwrite(data_pkt.data, 1, len - offsetof(data_pkt_t, data), file);
      bit_vector = bit_vector | 1 << (seq_num_rec - seq_expected);
    }

    if (seq_expected == seq_num_rec) {
      bit_vector = bit_vector >> 1;
      seq_expected += 1;

      while (bit_vector & 1) {
        bit_vector = bit_vector >> 1;
        seq_expected += 1;
      }
    }

    uint32_t s_acks = 0;
    for (int i = 0; i < window_size; i++) {
      s_acks = s_acks | (bit_vector & 1 << i);
    }
    s_acks = s_acks >> 1;

    ack_pkt.seq_num = htonl(seq_expected);
    ack_pkt.selective_acks = htonl(s_acks);

    int res = sendto(sockfd, &ack_pkt, sizeof(ack_pkt), 0,
            (struct sockaddr *)&src_addr, sizeof(src_addr));

    // If not error sending ack, print message
    if (res != -1) {
      printf("Server: Acknowledge sent: %d\n", ntohl(ack_pkt.seq_num));
    }
    
  } while (len == sizeof(data_pkt_t));

  // Clean up and exit.
  close(sockfd);
  fclose(file);

  exit(EXIT_SUCCESS);
}