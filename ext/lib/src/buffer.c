#include "lib/include/buffer.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

bool
buffer_init(buffer_t *b) {
    /*if (b->start != NULL) {*/
    /*return true;*/
    /*}*/
    b->start = (unsigned char*) malloc(MIN_BUFFER_SIZE);
    if (b->start == NULL) {
        return false;
    }
    b->pos = b->last = b->start;
    b->end = b->start + MIN_BUFFER_SIZE;
    return true;
}

void
buffer_destroy(buffer_t *b) {
    free(b->start);
}

void
buffer_reset(buffer_t *b) {
    b->pos = b->last = b->start;
}

/*
 * ensure available space, expand if necessary
 */
bool
check_buffer_space(buffer_t *b, size_t len) {
    size_t available  = b->end - b->last;
    size_t reusable   = b->pos - b->start;
    size_t total      = b->end - b->start;
    size_t used       = b->last - b->pos;

    if (len < available) {
        return true;
    }

    if (len < reusable && reusable > total / 4) {
        memcpy(b->start, b->pos, used);
        b->pos -= reusable;
        b->last -= reusable;
        return true;
    }

    size_t new_total = (total<<1);
    while (len > new_total - used) new_total <<= 1;
    unsigned char *new_start = (unsigned char*) malloc(new_total);
    if (new_start == NULL) {
        return false;
    }
    memcpy(new_start, b->pos, used);
    free(b->start);
    b->start = new_start;
    b->pos = b->start;
    b->last = b->pos + used;
    b->end = b->start + new_total;

    return true;
}

void
buffer_write_char(buffer_t *b, unsigned char n) {
    check_buffer_space(b, sizeof(n));
    *b->last++ = n;
}

void
buffer_write_short(buffer_t *b, unsigned short n) {
    check_buffer_space(b, sizeof(n));
    unsigned short tmp = 0;
    while (n != 0) {
        tmp |= (n & 0xFF);
        tmp <<= 8;
        n >>= 8;
    }
    *(unsigned short*)b->last = tmp;
    b->last += 2;
}

void
buffer_write_int(buffer_t *b, unsigned int n) {
    check_buffer_space(b, sizeof(n));
    unsigned int tmp = 0;
    while (n != 0) {
        tmp |= (n & 0xFF);
        tmp <<= 8;
        n >>= 8;
    }
    *(unsigned int*)b->last = tmp;
    b->last += sizeof(n);
}

void
buffer_write_long(buffer_t *b, unsigned long n) {
    check_buffer_space(b, sizeof(n));
    unsigned long tmp = 0;
    while (n != 0) {
        tmp |= (n & 0xFF);
        tmp <<= 8;
        n >>= 8;
    }
    *(unsigned long*)b->last = tmp;
    b->last += sizeof(n);
}

void
buffer_write_bytes(buffer_t *b, const char *bytes, size_t len) {
    check_buffer_space(b, len);
    memcpy(b->last, bytes, len);
    b->last += len;
}

void
buffer_write_string(buffer_t *b, const char *str) {
    size_t len = strlen(str);
    buffer_write_bytes(b, str, len);
}

char
buffer_read_char(buffer_t *b) {
    assert(b->last - b->pos >= sizeof(char));
    return *b->pos++;
}

short
buffer_read_short(buffer_t *b) {
    assert(b->last - b->pos >= sizeof(short));
    unsigned short tmp = 0;
    size_t i = 0;
    while (i < sizeof(short)) {
        tmp |= (unsigned short) *b->pos++;
        tmp <<= 8;
    }
    return tmp;
}

int
buffer_read_int(buffer_t *b) {
    assert(b->last - b->pos >= sizeof(int));
    unsigned int tmp = 0;
    size_t i = 0;
    while (i < sizeof(int)) {
        tmp |= (unsigned int) *b->pos++;
        tmp <<= 8;
    }
    return tmp;
}

long
buffer_read_long(buffer_t *b) {
    assert(b->last - b->pos >= sizeof(long));
    unsigned long tmp = 0;
    size_t i = 0;
    while (i < sizeof(long)) {
        tmp |= (unsigned long) *b->pos++;
        tmp <<= 8;
    }
    return tmp;
}

void
buffer_read_bytes(buffer_t *b, char *bytes, size_t len) {
    assert(b->last - b->pos >= len);
    if (bytes == NULL) {
        bytes = (char*) malloc(len);
        assert(bytes != NULL);
    }
    memcpy(bytes, b->pos, len);
    b->pos += len;
}

/*
 * you should care about the trailing-null by yourself
 */
void
buffer_read_string(buffer_t *b, char *str, size_t len) {
    buffer_read_bytes(b, str, len);
}
