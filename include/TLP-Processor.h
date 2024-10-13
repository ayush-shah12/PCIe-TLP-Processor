#ifndef __TLP-PROCESSOR_H
#define __TLP-PROCESSOR_H

#define INFO(...) do {fprintf(stderr, "[          ] [ INFO ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0)
#define ERROR(...) do {fprintf(stderr, "[          ] [ ERR  ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0) 

void print_packet(unsigned int packet[]);

void store_values(unsigned int packets[], char *memory);

unsigned int* create_completion(unsigned int packets[], const char *memory);

#endif
