#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "gtest/gtest.h"

extern "C" {
#include "TLP-Processor.h"
}

class write_tests : public testing::Test { 
    void SetUp() override {
        
    }
};

//Single Write Size 1
TEST_F(write_tests, TrivialWrite) {
    char *memory = (char*)malloc(1000000);
    FILE *fileptr = fopen("./tests/inputs/RAM.bin", "r");
    fread(memory, 1, 1000000, fileptr);
    fclose(fileptr);
    char *our_memory = (char*)malloc(1000000);
    fileptr = fopen("./tests/inputs/RAM1.bin", "r");
    fread(our_memory, 1, 1000000, fileptr);
    fclose(fileptr);
    unsigned int *TLPs = (unsigned int*)malloc(sizeof(int)*4);
    fileptr = fopen("./tests/inputs/Write_TLP1.bin", "r");
    fread(TLPs, 4, 4, fileptr);
    fclose(fileptr);

    store_values(TLPs, memory);

    for(int mem_index = 0; mem_index< 1000000; mem_index++)
    {
        EXPECT_EQ(memory[mem_index], our_memory[mem_index]);
    }
    free(memory); free(TLPs); free(our_memory);
}
//Single Write of size 3
TEST_F(write_tests, SimpleWrite) {
    char *memory = (char*)malloc(1000000);
    FILE *fileptr = fopen("./tests/inputs/RAM.bin", "r");
    fread(memory, 1, 1000000, fileptr);
    fclose(fileptr);
    char *our_memory = (char*)malloc(1000000);
    fileptr = fopen("./tests/inputs/RAM2.bin", "r");
    fread(our_memory, 1, 1000000, fileptr);
    fclose(fileptr);
    unsigned int *TLPs= (unsigned int*)malloc(sizeof(int)*6);
    fileptr = fopen("./tests/inputs/Write_TLP2.bin", "r");
    fread(TLPs, 4, 6, fileptr);
    fclose(fileptr);

    store_values(TLPs, memory);

    for(int mem_index = 0; mem_index< 1000000; mem_index++)
    {
        EXPECT_EQ(memory[mem_index], our_memory[mem_index]);
    }
    free(memory); free(TLPs); free(our_memory);
}
//Singe Write Size 5, BEs added
TEST_F(write_tests, TestBEs) {
    char *memory = (char*)malloc(1000000);
    FILE *fileptr = fopen("./tests/inputs/RAM.bin", "r");
    fread(memory, 1, 1000000, fileptr);
    fclose(fileptr);
    char *our_memory = (char*)malloc(1000000);
    fileptr = fopen("./tests/inputs/RAM3.bin", "r");
    fread(our_memory, 1, 1000000, fileptr);
    fclose(fileptr);
    unsigned int *TLPs= (unsigned int*)malloc(sizeof(int)*8);
    fileptr = fopen("./tests/inputs/Write_TLP3.bin", "r");
    fread(TLPs, 4, 8, fileptr);
    fclose(fileptr);

    store_values(TLPs, memory);

    for(int mem_index = 0; mem_index< 1000000; mem_index++)
    {
        EXPECT_EQ(memory[mem_index], our_memory[mem_index]);
    }
    free(memory); free(TLPs); free(our_memory);
}
//Two Write requests w/ BE 
TEST_F(write_tests, MultipleTLPs) {
    char *memory = (char*)malloc(1000000);
    FILE *fileptr = fopen("./tests/inputs/RAM.bin", "r");
    fread(memory, 1, 1000000, fileptr);
    fclose(fileptr);
    char *our_memory = (char*)malloc(1000000);
    fileptr = fopen("./tests/inputs/RAM4.bin", "r");
    fread(our_memory, 1, 1000000, fileptr);
    fclose(fileptr);
    unsigned int *TLPs= (unsigned int*)malloc(sizeof(int)*11);
    fileptr = fopen("./tests/inputs/Write_TLP4.bin", "r");
    fread(TLPs, 4, 11, fileptr);
    fclose(fileptr);

    store_values(TLPs, memory);

    for(int mem_index = 0; mem_index< 1000000; mem_index++)
    {
        EXPECT_EQ(memory[mem_index], our_memory[mem_index]);
    }
    free(memory); free(TLPs); free(our_memory);
}
