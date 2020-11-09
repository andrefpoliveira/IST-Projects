#include "packet-format.h"
#include <limits.h>
#include <netdb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("Usage: file-sender <file> <host> <port> <window size>\n");
    exit(EXIT_FAILURE);
  }
  char *file_name = argv[1];
  char *host = argv[2];
  int port = atoi(argv[3]);
  int window_size = atoi(argv[4]);

  FILE *file = fopen(file_name, "r");
  if (!file) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  if (port < 0 || strlen(argv[3]) != 4) {
    perror("port");
    exit(EXIT_FAILURE);
  }

  if (window_size < 1) {
    perror("window");
    exit(EXIT_FAILURE);
  }

  // Prepare server host address.
  struct hostent *he;
  if (!(he = gethostbyname(host))) {
    perror("gethostbyname");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in srv_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr = *((struct in_addr *)he->h_addr),
  };

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  uint32_t window_base = 0;
  uint32_t bit_vector = 0;
  uint32_t pcks_available = window_size;
  uint32_t last_chunk = -1;

  data_pkt_t data_pkt;
  size_t data_len;

  // Initialize timer
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  int timeouts = 0;
  ssize_t res;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  do { // Generate segments from file, until the the end of the file.
    ack_pkt_t ack_pkt;

    while (pcks_available > 0 && !(feof(file) && data_len < sizeof(data_pkt.data))) {
      if (!(bit_vector & 1 << (window_size - pcks_available))) {
        uint32_t pck = window_base + window_size - pcks_available;

        // Prepare data segment.
        data_pkt.seq_num = htonl(pck);

        // Load data from file.
        fseek(file, pck * 1000, SEEK_SET);
        data_len = fread(data_pkt.data, 1, sizeof(data_pkt.data), file);

        // Message sent after the chunk is sent
        printf("Client: Sending segment %d, size %ld.\n", ntohl(data_pkt.seq_num),
                offsetof(data_pkt_t, data) + data_len);

        // Send segment.
        ssize_t sent_len =
            sendto(sockfd, &data_pkt, offsetof(data_pkt_t, data) + data_len, 0,
                  (struct sockaddr *)&srv_addr, sizeof(srv_addr));

        if (sent_len < 1000) {
          last_chunk = pck;
        }

        if (sent_len != offsetof(data_pkt_t, data) + data_len) {
          fprintf(stderr, "Truncated packet.\n");
          exit(EXIT_FAILURE);
        }
      }
      pcks_available--;
    }
      
    // Receive acknowledge
    res = recvfrom(sockfd, &ack_pkt, sizeof(ack_pkt), 0,
        (struct sockaddr *)&srv_addr, &(socklen_t){sizeof(srv_addr)});

    // If ack is not error, confirm receive
    if (res >= 0) {
      printf("Client: Acknowledge received: %d\n", ntohl(ack_pkt.seq_num));
      timeouts = 0;

      uint32_t s_num = ntohl(ack_pkt.seq_num);
      uint32_t s_bit = ntohl(ack_pkt.selective_acks);

      pcks_available = s_num - window_base;
      bit_vector = bit_vector >> (s_num - window_base);
      bit_vector = bit_vector | (s_bit << 1);
      window_base = s_num;

    // Error
    } else {
      timeouts++;
      pcks_available = window_size;
      fseek(file, window_base * 1000, SEEK_SET);
      if (timeouts == 3){
        printf("Error: Too many timeouts!\n");
        exit(EXIT_FAILURE);
      }
      printf("Error: Timout message\n");
    }    
  } while (last_chunk == -1 || window_base <= last_chunk);


  // Clean up and exit.
  close(sockfd);
  fclose(file);

  exit(EXIT_SUCCESS);
}