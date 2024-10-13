#include <stdio.h>
#include <stdlib.h>

#include "TLP-Processor.h"

int main(){
    unsigned int *packet;

    FILE *fileptr;

    printf("Expected Values:\n");
    printf("Packet Type: Write\nAddress: 45\nLength: 4\nRequester ID: 14\nTag: 5\nLast BE: 15\n1st BE: 12\nData: -5000 -4993 -4986 -4979\n");
    printf("Your Function: \n");
    packet = malloc(sizeof(int)*(4+3));
    fileptr = fopen("./tests/inputs/test1.bin", "r");
    fread(packet, 4, 4+3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    printf("\nExpected Values:\n");
    printf("Packet Type: Read\nAddress: 4500\nLength: 17\nRequester ID: 24\nTag: 15\nLast BE: 3\n1st BE: 10\nData: \n");
    printf("Your Function: \n");
    packet = malloc(sizeof(int)*3);
    fileptr = fopen("./tests/inputs/test2.bin", "r");
    fread(packet, 4, 3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    printf("\nExpected Values:\n");
    printf("No Output (invalid packet)\n");
    printf("Your Function: \n");
    packet = malloc(sizeof(int)*3);
    fileptr = fopen("./tests/inputs/test3.bin", "r");
    fread(packet, 4, 3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    printf("\nExpected Values:\n");
    printf("Packet Type: Write\nAddress: 4660\nLength: 6\nRequester ID: 20\nTag: 8\nLast BE: 15\n1st BE: 1\nData: 305419896 305419897 305419898 305419899 305419900 305419901 \n");
    printf("Your Function: \n");
    packet = malloc(sizeof(int)*(6+3));
    fileptr = fopen("./tests/inputs/test4.bin", "r");
    fread(packet, 4, 6+3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    printf("\nExpected Values:\n");
    printf("Packet Type: Read\nAddress: 43981\nLength: 6\nRequester ID: 20\nTag: 8\nLast BE: 3\n1st BE: 5\nData: \n");
    printf("Your Function: \n");
    packet = malloc(sizeof(int)*3);
    fileptr = fopen("./tests/inputs/test5.bin", "r");
    fread(packet, 4, 3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    printf("\nExpected Values:\n");
    printf("Packet Type: Write\nAddress: 43690\nLength: 3\nRequester ID: 65535\nTag: 255\nLast BE: 15\n1st BE: 14\nData: 0 500 1000 \n");
    printf("Your Function: \n");
    packet = malloc(sizeof(int)*(3+3));
    fileptr = fopen("./tests/inputs/test6.bin", "r");
    fread(packet, 4, 3+3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet);

    printf("\nExpected Values:\n");
    printf("Packet Type: Read\nAddress: 48059\nLength: 3\nRequester ID: 65535\nTag: 255\nLast BE: 15\n1st BE: 14\nData:\n");
    printf("Your Function: \n");
    packet = malloc(sizeof(int)*(3));
    fileptr = fopen("./tests/inputs/test7.bin", "r");
    fread(packet, 4, 3, fileptr);
    fclose(fileptr); 
    print_packet(packet);
    free(packet); 

    return 0;
}
