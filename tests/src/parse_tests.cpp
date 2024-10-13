#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "TLP-Processor.h"
}

int main(){
    unsigned int *packet;

    FILE *fileptr;

    packet = (unsigned int*)malloc(sizeof(int)*(4+3));
    fileptr = fopen("./tests/inputs/test1.bin", "r");
    fread(packet, 4, 4+3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    packet = (unsigned int*)malloc(sizeof(int)*3);
    fileptr = fopen("./tests/inputs/test2.bin", "r");
    fread(packet, 4, 3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    packet = (unsigned int*)malloc(sizeof(int)*(6+3));
    fileptr = fopen("./tests/inputs/test4.bin", "r");
    fread(packet, 4, 6+3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    packet = (unsigned int*)malloc(sizeof(int)*3);
    fileptr = fopen("./tests/inputs/test5.bin", "r");
    fread(packet, 4, 3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    packet = (unsigned int*)malloc(sizeof(int)*(3+3));
    fileptr = fopen("./tests/inputs/test6.bin", "r");
    fread(packet, 4, 3+3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    packet = (unsigned int*)malloc(sizeof(int)*(3));
    fileptr = fopen("./tests/inputs/test7.bin", "r");
    fread(packet, 4, 3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet); 

    return 0;
}
