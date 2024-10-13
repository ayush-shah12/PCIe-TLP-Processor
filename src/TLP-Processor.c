#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "TLP-Processor.h"

// macros for packet masks to avoid repetition
#define WRITE_REQ_MASK 0x40000000
#define READ_REQ_MASK 0x00000000
#define READ_RES_MASK 0x4A000000
#define LENGTH_MASK 0x000003FF
#define REQ_ID_MASK 0xFFFF0000
#define TAG_MASK 0x0000FF00
#define LAST_BE_MASK 0x000000F0
#define FIRST_BE_MASK 0x0000000F


// Function to print packet details
void print_packet(unsigned int packet[]) {
    unsigned int packet_type = packet[0] & 0xFFFFFC00;
    unsigned int length = packet[0] & LENGTH_MASK;
    unsigned int reqID = (packet[1] & REQ_ID_MASK) >> 16;
    unsigned int tag = (packet[1] & TAG_MASK) >> 8;
    unsigned int address = packet[2];

    switch (packet_type) {
        case WRITE_REQ_MASK:
            printf("Packet Type: Write\n");
            printf("Address: %d\n", address);
            printf("Length: %d\n", length);
            printf("Requester ID: %d\n", reqID);
            printf("Tag: %d\n", tag);

            if (length == 1) {
                printf("Last BE: None (Length is 1)\n");
                printf("1st BE: %d\n", packet[1] & FIRST_BE_MASK);
            } else {
                printf("Last BE: %d\n", (packet[1] & LAST_BE_MASK) >> 4);
                printf("1st BE: %d\n", packet[1] & FIRST_BE_MASK);
            }

            printf("Data: ");
            for (unsigned int i = 0; i < length; i++) {
                printf("%d ", packet[3 + i]);
            }
            printf("\n");
            break;

        case READ_REQ_MASK:
            printf("Packet Type: Read\n");
            printf("Address: %d\n", address);
            printf("Length: %d\n", length);
            printf("Requester ID: %d\n", reqID);
            printf("Tag: %d\n", tag);

            if (length == 1) {
                printf("Last BE: None (Length is 1)\n");
                printf("1st BE: %d\n", packet[1] & FIRST_BE_MASK);
            } else {
                printf("Last BE: %d\n", (packet[1] & LAST_BE_MASK) >> 4);
                printf("1st BE: %d\n", packet[1] & FIRST_BE_MASK);
            }
            break;

        case READ_RES_MASK:
            printf("Packet Type: Read Response\n");
            // Further details can be added as needed
            break;

        default:
            printf("No Output (invalid packet)\n");
            return;
    }
}

// Function to store packet values into memory
void store_values(unsigned int packets[], char *memory) {
    int pos = 0;

    while (true) {
        unsigned int packet_type = packets[pos] & 0xFFFFFC00;
        if (packet_type != WRITE_REQ_MASK) break;

        unsigned int length = packets[pos] & LENGTH_MASK;
        unsigned int firstBE = packets[++pos] & FIRST_BE_MASK;
        unsigned int lastBE = (packets[pos] & LAST_BE_MASK) >> 4;
        unsigned int address = packets[++pos];

        // Process first BE
        int firstBE_indices[4], lastBE_indices[4];
        int firstBE_count = 0, lastBE_count = 0;
        for (int i = 0; i < 4; i++) {
            if (firstBE & (1 << i)) firstBE_indices[firstBE_count++] = i;
            if (lastBE & (1 << i)) lastBE_indices[lastBE_count++] = i;
        }

        if (address > 1000000) break;

        unsigned int data = packets[++pos];
        // Write data based on first BE
        for (int i = 0; i < firstBE_count; i++) {
            memory[address + firstBE_indices[i]] = (data >> (8 * firstBE_indices[i])) & 0xFF;
        }
        address += 4;

        // Write data for middle packets (if length > 1)
        for (unsigned int i = 1; i < length - 1; i++) {
            data = packets[++pos];
            for (int j = 0; j < 4; j++) {
                memory[address + j] = (data >> (8 * j)) & 0xFF;
            }
            address += 4;
        }

        // Process last BE
        if (length > 1) {
            data = packets[++pos];
            for (int i = 0; i < lastBE_count; i++) {
                memory[address + lastBE_indices[i]] = (data >> (8 * lastBE_indices[i])) & 0xFF;
            }
        }
    }
}

// Function to create completion packets from read requests
unsigned int* create_completion(unsigned int packets[], const char *memory) {
    int packetPos = 0;              // Position of packets
    int returnPos = 0;              // Position for the return array
    int totalLength = 0;            // Total length of the return array
    unsigned int *returnArray = NULL; // Pointer to the return array

    while (1) {
        unsigned int currentPacket = packets[packetPos];
        unsigned int headerMask = 0xFFFFFC00;
        unsigned int result = currentPacket & headerMask; 
        unsigned int lengthMask = 0x000003FF;
        unsigned int length = currentPacket & lengthMask;

        unsigned int currentLengthOfCompletion = 0; // Track the current length of the completion packet

        // Check if we have a read request
        if (result != 0x00000000) {
            break;
        }

        int newLength = totalLength + (length + 3);  // Current length + new TLP + 3 headers
        returnArray = realloc(returnArray, sizeof(unsigned int) * newLength);
        if (returnArray == NULL) {
            return NULL; // Memory allocation failed
        }

        // Set the first header
        returnArray[returnPos++] = (0x4A000000 | length);
        unsigned int firstHeaderPos = returnPos - 1; // Track first header position for updating later if required

        unsigned int byteCount = (4 * length) & 0x00000FFF; 
        returnArray[returnPos++] = (0xDC << 16) | byteCount; 

        packetPos++;
        unsigned int nextPacket = packets[packetPos];
        unsigned int reqID = (nextPacket >> 16) & 0xFFFF; 
        unsigned int tag = (nextPacket >> 8) & 0xFF;     
        unsigned int firstBE = nextPacket & 0x0000000F;
        unsigned int lastBE = (nextPacket & 0x000000F0) >> 4;

        // Debugging output
        printf("requesterID: %d\n", reqID);
        printf("tag: %d\n", tag);
        printf("firstBE: %x\n", firstBE);
        printf("lastBE: %x\n", lastBE);

        packetPos++;
        unsigned int address = packets[packetPos];
        printf("address: %d\n", address);

        // Set the request ID, tag, and address
        returnArray[returnPos++] = (reqID << 16) | (tag << 8) | (address & 0x7F);

        // Determine lengths for completion packets based on a 0x4000 boundary
        unsigned int firstLength = ((0x4000 - (address % 0x4000)) / 4 > length) ? length : (0x4000 - (address % 0x4000)) / 4;
        unsigned int secondLength = length - firstLength; 

        int requiresTwoPackets = (secondLength != 0 && firstLength != 0); 
        if (requiresTwoPackets) {
            newLength = totalLength + ((firstLength + 3 + secondLength + 3) * sizeof(unsigned int));
            returnArray = realloc(returnArray, sizeof(unsigned int) * newLength);   
            if (returnArray == NULL) {
                return NULL; // Memory allocation failed
            }
        }

        // Process the first byte enable
        unsigned int data = 0x00000000;
        for (int byte = 0; byte < 4; byte++) {
            if (firstBE & (1 << byte)) {
                data |= ((unsigned char)memory[address + byte] << (byte * 8));
            }
        }
        currentLengthOfCompletion++;
        returnArray[returnPos++] = data;  
        address += 4; 

        // Process subsequent data
        for (unsigned int c = 1; c < length - 1; c++) {
            unsigned int data1 = 0x00000000;
            if (currentLengthOfCompletion == firstLength && requiresTwoPackets) {
                // Update first header and add second header for the next completion packet
                returnArray[firstHeaderPos] = (0x4A000000 | firstLength);
                returnArray[returnPos++] = (0x4A000000 | secondLength);
                returnArray[returnPos++] = ((0xDC << 16) | ((4 * secondLength) & 0x00000FFF));
                returnArray[returnPos++] = (reqID << 16) | (tag << 8) | (address & 0x7F);
                currentLengthOfCompletion++
            }
            
            for (int x = 0; x < 4; x++) {
                data1 |= ((unsigned char)memory[address + x] << (x * 8));  
            }
            currentLengthOfCompletion++;
            returnArray[returnPos++] = data1; 
            address += 4; 
        }

        // Process the last byte enable
        data = 0x00000000;
        for (int byte = 0; byte < 4; byte++) {
            if (lastBE & (1 << byte)) {  
                if (currentLengthOfCompletion == firstLength && requiresTwoPackets) {
                    // Update first header again if required
                    returnArray[firstHeaderPos] = (0x4A000000 | firstLength);
                    returnArray[returnPos++] = (0x4A000000 | secondLength);
                    returnArray[returnPos++] = ((0xDC << 16) | ((4 * secondLength) & 0x00000FFF));
                    returnArray[returnPos++] = (reqID << 16) | (tag << 8) | (address & 0x7F);
                    currentLengthOfCompletion++;
                }
                data |= ((unsigned char)memory[address + byte] << (byte * 8)); 
            }
        }
        returnArray[returnPos++] = data;
        totalLength = newLength;
        packetPos++;
    }

    return returnArray; 
}
