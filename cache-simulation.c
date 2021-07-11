#include <stdio.h>
#include <stdlib.h>

int memory[16777216]={0x00};
int addressAccessedInBase16;
int sizeAs;
int value;
char adressAccessed[256];
char valueString[256];
char access[24];

int log2(int n) {
    int r=0;
    while (n>>=1) r++;
    return r;
}
int ones(int n){return ((1 << n) - 1);}
int twoThePower(int n ){ return 1 << n;}
int leftShift(int x, int n){return x << n;}
int rightShift(int x, int n){return x >> n;}
int lowerN(int x, int n){return x & ones(n);}

struct Block{
    bool valid = false;
    int order = 0;
    int tag = 0;
};

int main(int argc, char* argv[]){
    int cacheSize;
    int ways;
    int blockSize;



    sscanf(argv[--argc], "%d", &blockSize);
    sscanf(argv[--argc],"%d", &ways);
    sscanf(argv[--argc],"%d",  &cacheSize);

    int set = cacheSize * 1024 /(blockSize*ways);
    int indexBits =log2(set);
    int offsetBits = log2(blockSize);
    int tagBits = 24 - indexBits - offsetBits;

    struct Block cache[set][ways];

    FILE* traceFile;
    traceFile=fopen(argv[1], "r");
    while(fscanf(traceFile, "%s %s %d", access, adressAccessed, &sizeAs)!=EOF){
        sscanf(adressAccessed, "%x", &addressAccessedInBase16);

        int index = lowerN(rightShift(addressAccessedInBase16, offsetBits),indexBits);
        int tag = lowerN(rightShift(rightShift(addressAccessedInBase16, offsetBits), indexBits), tagBits);
        bool flag= false;
        int highestOrder= 0;
        int i = 0;
        while(i < ways){
            if(cache[index][i].order >= highestOrder)  highestOrder = cache[index][i].order;
            i++;
            }
            if(('s' - access[0]) == 0){
                fscanf(traceFile, "%s", valueString);
                sscanf(valueString, "%x", &value);

                for(int i = 0; i < ways; i++){
                    if(cache[index][i].tag == tag && cache[index][i].valid==true){
                        highestOrder++;
                        printf("store 0x%x hit\n", addressAccessedInBase16);
                        flag = true;
                        cache[index][i].order = highestOrder;
                        for (int i = 0;i < sizeAs; i++ ) sscanf(valueString + 2 * i, "%2x", &memory[addressAccessedInBase16 + i]);
                        printf("\n");
                        }
                    }

                if(!flag){
                    printf("store 0x%x miss\n", addressAccessedInBase16);
                    for (int i = 0; i < sizeAs; i++) sscanf(valueString + 2 * i, "%2x", &memory[addressAccessedInBase16 + i]);
                    printf("\n");
                }
            }
            else if(('l' - access[0]) == 0){
                flag = false;
                for(int i=0; i < ways; i++){
                    if(cache[index][i].tag == tag && cache[index][i].valid ==true){
                        highestOrder++;
                        cache[index][i].order = highestOrder;
                        printf("load 0x%x hit ", addressAccessedInBase16);
                        flag = true;
                        for(int i = 0;i < sizeAs; i++) printf("%02x",memory[addressAccessedInBase16 + i]);
                        printf("\n");
                    }
                }
                if(!flag){
                    int loworder = highestOrder;
                    int lowWay = 0;
                    int i = 0;
                    highestOrder++;
                    while(i < ways){
                        if(cache[index][i].order < loworder){
                            loworder = cache[index][i].order;
                            lowWay = i;
                            }
                        i++;
                        }

                    cache[index][lowWay].tag = tag;
                    cache[index][lowWay].valid = true;
                    cache[index][lowWay].order = highestOrder;
                    printf("load 0x%x miss ", addressAccessedInBase16);
                    for(int i = 0;i < sizeAs; i++) printf("%02x", memory[addressAccessedInBase16 + i]);
                    printf("\n");
            }

        }
    }
    fclose(traceFile);
    return EXIT_SUCCESS;
}
