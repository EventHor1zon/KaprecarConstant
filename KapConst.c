/**
 * @file    KapConst.c 
 * @author  RJAM
 * @brief   Heard about a cool number called Kaprekar's Constant
 *          6147. If you order the numbers, then subtract the reverse ordered 
 *          number, you get back to 6147. Aparently doing this operation recursively 
 *          on any 4-digit number results in 6147. Let's see...
 * 
 * 
 **/
#define DEBUG_MODE 0 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


void long_to_array(uint8_t *data, uint32_t input) {
    // split number 
    data[0] = (input > 999 ? (uint8_t)(input / 1000) : 0);
    input -= data[0] * 1000;
    data[1] = (input > 99 ? (uint8_t)(input / 100) : 0);
    input -= data[1] * 100;
    data[2] = (input > 9 ? (uint8_t)(input / 10) : 0);
    input -= data[2] * 10;
    data[3] = (uint8_t)input / 1;
}


uint32_t array_to_long(uint8_t *ptr) {
    return ((ptr[0] * 1000) + (ptr[1] * 100) + (ptr[2] * 10) + ptr[3]);
}

uint32_t reorder_digits(uint32_t input) {

    uint32_t output = 0;
    uint8_t tmp[4] = {0};
    uint8_t val = 0;
    uint8_t cnt = 0;
    bool ordered = false;

    long_to_array(tmp, input);

#if DEBUG_MODE
    printf("split %u into %02x %02x %02x %02x\n", input, tmp[0], tmp[1], tmp[2], tmp[3]);
#endif

    // reorder ascending
    do {
        if(
            tmp[0] >= tmp[1] && 
            tmp[1] >= tmp[2] &&
            tmp[2] >= tmp[3]
        ) {
            ordered = true;
        }
        else {
            for(uint8_t i=1; i < 4; i++) {
                if(tmp[i] > tmp[i-1]) {
                    val = tmp[i-1];
                    tmp[i-1] = tmp[i];
                    tmp[i] = val;
                }
            }
            cnt++;

            if(cnt > 100) {
                printf("Error parsing %u\n", input);
                exit(0);
            }
        }
    } while(!ordered);

    return array_to_long(tmp);
}



uint32_t reverse_order(uint32_t input) {

    uint8_t tmp[4] = {0};
    uint8_t data[4] = {0};

    long_to_array(data, input);

    tmp[3] = data[0];
    tmp[2] = data[1];
    tmp[1] = data[2];
    tmp[0] = data[3];

    return array_to_long(tmp);
}

int main(int argc, char *argv[]) {

    uint32_t a = 3251;
    uint32_t b = 8812;
    uint32_t x, y, z;
    uint16_t cnt = 0;
    uint16_t fails = 0;
    uint32_t failures[32];


    for(uint32_t i=1; i<10000; i++) {

        if((i % 1111) == 0) {
            ;
        }
        else {
            a = i;
            cnt = 0;
            printf("Decoding %u ", a);
            do {
                x = reorder_digits(a);
                y = reverse_order(x);
                z = x - y;
                if(z == 6174) {
                    break;
                }
                if(cnt > 999) {
                    failures[fails] = i;
                    fails++;
                    break;
                }
                a = z;
                cnt++;
            } while (1);


            if(cnt < 1000) {
                printf("took %u steps\n", cnt);
            }
            else {
                printf("Failed\n");
            }
        }
    }

    printf("Fails: %u\n", fails);

    for(int i=0; i < fails; i++) {
        printf("%u failed\n", failures[i]);
    }
    return 0;
}