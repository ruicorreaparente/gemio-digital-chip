#include <stdio.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
    uint32_t a;
    uint32_t b;
    uint8_t  c;
    uint8_t  d;
    uint16_t e;
    uint64_t f;
    uint64_t g;
} test_pack_t;
#pragma pack(pop)

typedef struct __attribute__((packed)) {
    uint32_t a;
    uint32_t b;
    uint8_t  c;
    uint8_t  d;
    uint16_t e;
    uint64_t f;
    uint64_t g;
} test_attr_t;

int main(void) {
    printf("test_pack_t (pragma)  = %zu bytes\n", sizeof(test_pack_t));
    printf("test_attr_t (attr)    = %zu bytes\n", sizeof(test_attr_t));
    return 0;
}