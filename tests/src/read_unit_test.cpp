#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "gtest/gtest.h"

extern "C" {
#include "TLP-Processor.h"
}

class read_tests : public testing::Test { 
    void SetUp() override {
        
    }
};
/*
Read is: 3, Comp is :7
Read is: 6, Comp is :15
Read is: 3, Comp is :10
Read is: 6, Comp is :18
Read is: 6, Comp is :838
Read is: 60, Comp is :8972
*/
//One TLP
TEST_F(read_tests, OneTLP) {
    char *memory = (char*)malloc(1000000);
    FILE *fileptr = fopen("./tests/inputs/RAM.bin", "r");
    fread(memory, 1, 1000000, fileptr);
    fclose(fileptr);
    unsigned int *generated_completions;
    unsigned int *expected_completions=(unsigned int*)malloc(sizeof(int)*89);
    unsigned int *TLPs=(unsigned int*)malloc(sizeof(int)*60);
    fileptr = fopen("./tests/inputs/Read_TLP1.bin", "r");
    fread(TLPs, 4, 3, fileptr);
    fclose(fileptr);
    TLPs[3]=0xFFFFFFFF;
    generated_completions = create_completion(TLPs, memory);
    fileptr = fopen("./tests/inputs/Completion_TLP1.bin", "r");
    fread(expected_completions, 4, 7, fileptr);
    fclose(fileptr);
    for(int i = 0; i<7; i++)
    {
        EXPECT_EQ(generated_completions[i], expected_completions[i]);
    }
    free(memory); free(generated_completions); free(expected_completions); free(TLPs);
}
//Two TLPs
TEST_F(read_tests, TwoTLP) {
    char *memory = (char*)malloc(1000000);
    FILE *fileptr = fopen("./tests/inputs/RAM.bin", "r");
    fread(memory, 1, 1000000, fileptr);
    fclose(fileptr);
    unsigned int *generated_completions;
    unsigned int *expected_completions=(unsigned int*)malloc(sizeof(int)*8972);
    unsigned int *TLPs=(unsigned int*)malloc(sizeof(int)*60);
    fileptr = fopen("./tests/inputs/Read_TLP2.bin", "r");
    fread(TLPs, 4, 6, fileptr);
    fclose(fileptr);
    TLPs[6]=0xFFFFFFFF;
    generated_completions = create_completion(TLPs, memory);
    fileptr = fopen("./tests/inputs/Completion_TLP2.bin", "r");
    fread(expected_completions, 4, 15, fileptr);
    fclose(fileptr);
    for(int i = 0; i<15; i++)
    {
        EXPECT_EQ(generated_completions[i], expected_completions[i]);
    }
    free(memory); free(generated_completions); free(expected_completions); free(TLPs);
}
//One TLP (cross 0x4000)
TEST_F(read_tests, OneTLPover4k) {
    char *memory = (char*)malloc(1000000);
    FILE *fileptr = fopen("./tests/inputs/RAM.bin", "r");
    fread(memory, 1, 1000000, fileptr);
    fclose(fileptr);
    unsigned int *generated_completions;
    unsigned int *expected_completions=(unsigned int*)malloc(sizeof(int)*8972);
    unsigned int *TLPs=(unsigned int*)malloc(sizeof(int)*60);
    fileptr = fopen("./tests/inputs/Read_TLP3.bin", "r");
    fread(TLPs, 4, 3, fileptr);
    fclose(fileptr);
    TLPs[3]=0xFFFFFFFF;
    generated_completions = create_completion(TLPs, memory);
    fileptr = fopen("./tests/inputs/Completion_TLP3.bin", "r");
    fread(expected_completions, 4, 10, fileptr);
    fclose(fileptr);
    for(int i = 0; i<10; i++)
    {
        EXPECT_EQ(generated_completions[i], expected_completions[i]);
    }
    free(memory); free(generated_completions); free(expected_completions); free(TLPs);
}
//Two TLP (cross 0x4000)
TEST_F(read_tests, TwoTLPover4k) {
    char *memory = (char*)malloc(1000000);
    FILE *fileptr = fopen("./tests/inputs/RAM.bin", "r");
    fread(memory, 1, 1000000, fileptr);
    fclose(fileptr);
    unsigned int *generated_completions;
    unsigned int *expected_completions=(unsigned int*)malloc(sizeof(int)*8972);
    unsigned int *TLPs=(unsigned int*)malloc(sizeof(int)*60);
    fileptr = fopen("./tests/inputs/Read_TLP4.bin", "r");
    fread(TLPs, 4, 6, fileptr);
    fclose(fileptr);
    TLPs[6]=0xFFFFFFFF;
    generated_completions = create_completion(TLPs, memory);
    fileptr = fopen("./tests/inputs/Completion_TLP4.bin", "r");
    fread(expected_completions, 4, 18, fileptr);
    fclose(fileptr);
    for(int i = 0; i<18; i++)
    {
        EXPECT_EQ(generated_completions[i], expected_completions[i]);
    }
    free(memory); free(generated_completions); free(expected_completions); free(TLPs);
}

//Random Easy
TEST_F(read_tests, RandShort) {
    char *memory = (char*)malloc(1000000);
    FILE *fileptr = fopen("./tests/inputs/RAM.bin", "r");
    fread(memory, 1, 1000000, fileptr);
    fclose(fileptr);
    unsigned int *generated_completions;
    unsigned int *expected_completions=(unsigned int*)malloc(sizeof(int)*8972);
    unsigned int *TLPs=(unsigned int*)malloc(sizeof(int)*60);
    fileptr = fopen("./tests/inputs/Read_TLP5.bin", "r");
    fread(TLPs, 4, 6, fileptr);
    fclose(fileptr);
    TLPs[6]=0xFFFFFFFF;
    generated_completions = create_completion(TLPs, memory);
    fileptr = fopen("./tests/inputs/Completion_TLP5.bin", "r");
    fread(expected_completions, 4, 838, fileptr);
    fclose(fileptr);
    for(int i = 0; i<838; i++)
    {
        EXPECT_EQ(generated_completions[i], expected_completions[i]);
    }
    free(memory); free(generated_completions); free(expected_completions); free(TLPs);
}

//Random TLP (larger)
TEST_F(read_tests, RandLong) {
    char *memory = (char*)malloc(1000000);
    FILE *fileptr = fopen("./tests/inputs/RAM.bin", "r");
    fread(memory, 1, 1000000, fileptr);
    fclose(fileptr);
    unsigned int *generated_completions;
    unsigned int *expected_completions=(unsigned int*)malloc(sizeof(int)*8972);
    unsigned int *TLPs=(unsigned int*)malloc(sizeof(int)*61);
    fileptr = fopen("./tests/inputs/Read_TLP6.bin", "r");
    fread(TLPs, 4, 60, fileptr);
    fclose(fileptr);
    TLPs[60]=0xFFFFFFFF;
    generated_completions = create_completion(TLPs, memory);
    fileptr = fopen("./tests/inputs/Completion_TLP6.bin", "r");
    fread(expected_completions, 4, 8972, fileptr);
    fclose(fileptr);
    for(int i = 0; i<8972; i++)
    {
        EXPECT_EQ(generated_completions[i], expected_completions[i]);
    }
    free(memory); free(generated_completions); free(expected_completions); free(TLPs);
}
