#define _GNU_SOURCE
#include <stdint.h>

typedef struct index_s
{
    double timeMark;
    uint64_t recordNumber;
} record;

typedef struct index_hdr_s
{
    uint64_t recordsCount;
    struct index_s *index;
} indexHeader;