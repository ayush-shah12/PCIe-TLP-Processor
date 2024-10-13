#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "TLP-Processor.h"

void print_packet(unsigned int packet[])
{
    unsigned int _1row = packet[0];
    unsigned int _1rowMask = 0xFFFFFC00;
    unsigned int result = _1row & _1rowMask; // type of request

    // types of request
    unsigned int writeReq = 0x40000000;
    unsigned int readReq = 0x00000000;
    unsigned int readRes = 0x4A000000; 

    unsigned int _2row = packet[1];

    if(result == writeReq){

        unsigned int lengthMask = 0x000003FF; // length
        unsigned int length = _1row & lengthMask;

        unsigned int _2rowMask1 = 0xFFFF0000; // req. id (only write req / read req)
        unsigned int reqID =(_2row & _2rowMask1) >> 16;

        unsigned int _2rowMask2 = 0x0000FF00;
        unsigned int tag = (_2row & _2rowMask2) >> 8;

        unsigned int _3row = packet[2];
        unsigned int address = (_3row);
        
        printf("%s\n", "Packet Type: Write");
        printf("%s%d\n","Address: ", address);
        printf("%s%d\n","Length: ", length);
        printf("%s%d\n","Requester ID: ", reqID);
        printf("%s%d\n","Tag: ", tag);

        if(length == 1){    // only check 1 BE
            unsigned int _2rowMask3 = 0x0000000F;
            unsigned int firstBE = _2row & _2rowMask3;
            printf("%s\n", "Last BE: None (Length is 1)");
            printf("%s%d\n","1st BE: ", firstBE);
        }
        else{
            unsigned int _2rowMask4 = 0x000000F0;
            unsigned int lstBE = (_2row & _2rowMask4) >> 4;
            printf("%s%d\n","Last BE: ", lstBE);

            unsigned int _2rowMask3 = 0x0000000F;
            unsigned int firstBE = _2row & _2rowMask3;
            printf("%s%d\n","1st BE: ", firstBE);
        }

        unsigned int *pointer = &packet[3];
        printf("Data: ");
        for(unsigned int x = 0; x < length; x++){
            printf("%d ", *pointer++);
        }
        printf("\n");

    }
    else if(result == readReq){

        unsigned int lengthMask = 0x000003FF; // length
        unsigned int length = _1row & lengthMask;

        unsigned int _2rowMask1 = 0xFFFF0000; // req. id (only write req / read req)
        unsigned int reqID = (_2row & _2rowMask1) >> 16;

        unsigned int _2rowMask2 = 0x0000FF00;
        unsigned int tag = (_2row & _2rowMask2) >> 8;

        unsigned int _3row = packet[2];
        unsigned int address = (_3row);

        printf("%s\n", "Packet Type: Read");
        printf("%s%d\n","Address: ", address);
        printf("%s%d\n","Length: ", length);
        printf("%s%d\n","Requester ID: ", reqID);
        printf("%s%d\n","Tag: ", tag);


        if(length == 1){    // only check 1 BE
            unsigned int _2rowMask3 = 0x0000000F;
            unsigned int firstBE = _2row & _2rowMask3;
            printf("%s\n", "Last BE: None (Length is 1)");
            printf("%s%d\n","1st BE: ", firstBE);
        }
        else{
            unsigned int _2rowMask4 = 0x000000F0;
            unsigned int lstBE = (_2row & _2rowMask4) >> 4;
            printf("%s%d\n","Last BE: ", lstBE);

            unsigned int _2rowMask3 = 0x0000000F;
            unsigned int firstBE = _2row & _2rowMask3;
            printf("%s%d\n","1st BE: ", firstBE);
        }

        printf("Data: \n");

    }
    else if(result == readRes){
        //to do  
        printf("%s\n", "Packet Type: Read Response");
        
    }
    else{
        printf("%s", "No Output (invalid packet)\n");
        return;
    }
}

void store_values(unsigned int packets[], char *memory)
{
    int pos = 0;
    while(1){
            
            unsigned int _1rowMask = 0xFFFFFC00;
            unsigned int result = packets[pos] & _1rowMask; // type of request
            unsigned int writeReq = 0x40000000;


            if(result != writeReq){
                break;
            }
            
            unsigned int lengthMask = 0x000003FF; // length
            unsigned int length = packets[pos] & lengthMask;


            pos++;
            unsigned int _2rowMask4 = 0x000000F0;
            unsigned int lastBE = (packets[pos] & _2rowMask4) >> 4;
            unsigned int _2rowMask3 = 0x0000000F;
            unsigned int firstBE = packets[pos] & _2rowMask3;


            pos++;
            unsigned int _3row = packets[pos];
            unsigned int address = (_3row);


            int firstBE_indices[4] = {0}; 
            int lastBE_indices[4] = {0};
            int firstBE_count = 0;         
            int lastBE_count = 0; 

            // deals with which bytes i need for first/last BE
            if(length == 1){    //first BE Only
                for (int i = 0; i < 4; i++) {
                    if (firstBE & (1 << i)) {
                        firstBE_indices[firstBE_count++] = i;
                    }
                }
            }
            else{
                for (int i = 0; i < 4; i++) {
                    if (firstBE & (1 << i)) {
                        firstBE_indices[firstBE_count++] = i;
                    }
                }

                for (int i = 0; i < 4; i++) {
                    if (lastBE & (1 << i)) {
                        lastBE_indices[lastBE_count++] = i;
                    }
                }
            }
            

            if(address > 1000000){  //add logic for if address + byte is > 1,000,000?
                break;
            }

            pos++;
            unsigned int data = packets[pos];

            // deals with first BE for first data point
            for(int x = 0; x < firstBE_count; x++){
                int add = firstBE_indices[x];
                memory[add + address] = (data >> (8 * firstBE_indices[x])) & 0xFF;
            }

            address += 4;

            // no need for last BE
            if(length == 1){
                unsigned int c = 1;
                while(c < length){
                    pos++;
                    data = packets[pos]; 
                    for (int j = 0; j < 4; j++) {
                        memory[address + j] = (data >> (8 * j)) & 0xFF;
                    }
                    address += 4;
                    c++;
                }
            }

            // go right before last data to account for last BE
            else{
                unsigned int c = 1;
                while(c < length - 1){
                    pos++;
                    data = packets[pos]; 
                    for (int j = 0; j < 4; j++) {
                        memory[address + j] = (data >> (8 * j)) & 0xFF;
                    }
                    address += 4;
                    c++;
                }
            }
            
            //Last BE
            if(length != 1){
                pos++;
                data = packets[pos];
                for(int x = 0; x < lastBE_count; x++){
                    int add = lastBE_indices[x];
                    memory[address + add] = (data >> (8 * lastBE_indices[x])) & 0xFF;
                }
                address+=4;
            }

            pos++;   
    }
}

unsigned int* create_completion(unsigned int packets[], const char *memory) {
    int pos = 0;    //pos of packets
    int returnPos = 0;  //pos of my return
    int totalLength = 0;    //total length of the return array
    unsigned int* returnArray = NULL;

    while (1) {
        unsigned int _1row = packets[pos];
        unsigned int _1rowMask = 0xFFFFFC00;
        unsigned int result = _1row & _1rowMask; 
        unsigned int lengthMask = 0x000003FF;
        unsigned int length = _1row & lengthMask;

        unsigned int readReq = 0x00000000;
        if (result != readReq) {
            break;
        }

        int newLength = totalLength + (length + 3);  // Current length + new TLP + 3 headers
        returnArray = realloc(returnArray, sizeof(unsigned int) * newLength);

        if (returnArray == NULL) {
            return NULL;
        }

        // printf("%s%d\n", "length ", length);

        returnArray[returnPos++] = (0x4A000000 | length);

        unsigned int TrackFirstPos = returnPos - 1; //tracking the first header to update length later if needed for 2 completion requests.
        
        unsigned int byteCount = (4 * length) & 0x00000FFF; 
        returnArray[returnPos++] = (0xDC << 16) | byteCount; 

        pos++;
        unsigned int _2row = packets[pos];
        unsigned int reqID = (_2row >> 16) & 0xFFFF; 
        unsigned int tag = (_2row >> 8) & 0xFF;     
        unsigned int _2rowMask4 = 0x000000F0;
        unsigned int lastBE = (_2row & _2rowMask4) >> 4;
        unsigned int _2rowMask3 = 0x0000000F;
        unsigned int firstBE = _2row & _2rowMask3;

        printf("%s%d\n", "requesterID ", reqID);
        printf("%s%d\n", "tag ", tag);
        printf("%s%x\n", "firstBE ", firstBE);
        printf("%s%x\n", "lasttBE ", lastBE);

        pos++;
        unsigned int _3row = packets[pos];
        unsigned int address = (_3row);
        printf("%s%d\n", "address ", address);

        returnArray[returnPos++] = (reqID << 16) | (tag << 8) | (address & 0x7F);

        //splitting lengths; max 2 completion packets per TLP is possible
        unsigned int first = ((0x4000 - (address % 0x4000))/4 > length) ? length : (0x4000 - (address % 0x4000))/4;
        unsigned int second = length - first; 

        unsigned int currentLengthOfCompletion = 0; //count to keep track of if i need to move to the 2nd completion(if applic.) or not
        int isTwo = 0;  //bool to check if this TLP even requires more than 1 completion packets

        if(second != 0 && first !=0){
            newLength = totalLength + ((first + 3 + second + 3) * sizeof(unsigned int));
            returnArray = realloc(returnArray, sizeof(unsigned int) * newLength);   
            isTwo = 1;
        }

        // printf("%d %s\n", numBeforeBound, "numBeforeBound");
        // printf("%d %s\n", first, "first length");
        // printf("%d %s\n", second, "second length");


        // deal with first byte enable
        unsigned int data = 0x00000000;
        for (int byte = 0; byte < 4; byte++) {
            if (firstBE & (1 << byte)) {
                // printf("%x%s%d\n", (unsigned char) memory[address + byte], " memory at address: ", address + byte);
                data |= ((unsigned char) memory[address + byte] << (byte * 8));
            }
        }
        currentLengthOfCompletion++;
        returnArray[returnPos++] = data;  
        address += 4;
        // printf("%s%x\n","DATA ", data);

        for (unsigned int c = 1; c < length - 1; c++) {
            unsigned int data1 = 0x00000000;
            if(currentLengthOfCompletion == first && isTwo==1){
                returnArray[TrackFirstPos] = (0x4A000000 | first);
                returnArray[returnPos++] = (0x4A000000 | second);
                returnArray[returnPos++] = ((0xDC << 16) | ((4 * second) & 0x00000FFF));
                returnArray[returnPos++] = (reqID << 16) | (tag << 8) | (address & 0x7F);
                currentLengthOfCompletion++; //so this won't trigger again
            }
            
            for (int x = 0; x < 4; x++) {
                // printf("%x%s%d\n", (unsigned char) memory[address + x], " memory at address: ", address + x);
                data1 |= ((unsigned char)memory[address + x] << ((x) * 8));  
            }
            currentLengthOfCompletion++;
            returnArray[returnPos++] = data1; 
            address += 4; 
            // printf("%s%x\n","DATA ", data1);            
        }

        data = 0x00000000;
        for (int byte = 0; byte < 4; byte++) {
            if (lastBE & (1 << byte)) {  
                if(currentLengthOfCompletion == first && isTwo==1){
                    returnArray[TrackFirstPos] = (0x4A000000 | first);
                    returnArray[returnPos++] = (0x4A000000 | second);
                    returnArray[returnPos++] = ((0xDC << 16) | ((4 * second) & 0x00000FFF));
                    returnArray[returnPos++] = (reqID << 16) | (tag << 8) | (address & 0x7F);
                    currentLengthOfCompletion++; //so this won't trigger again
            }
                // printf("%x%s%d\n", (unsigned char)memory[address + byte], " memory at address: ", address + byte);
                data |= ((unsigned char)memory[address + byte] << ((byte) * 8)); 
            }
        }
        returnArray[returnPos++] = data;
        totalLength = newLength;
        pos++;

        // for (int i = 0; i < returnPos; i++) {
        //     printf("returnArray[%d] = %x\n", i, returnArray[i]);
        // }
    }
    return returnArray; 
}