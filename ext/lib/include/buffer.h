#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdbool.h>

#define MIN_BUFFER_SIZE (1UL<<15)

typedef struct buffer_t buffer_t;
struct buffer_t {
    unsigned char     *start;
    unsigned char     *end;
    unsigned char     *pos;
    unsigned char     *last;
};

bool    buffer_init(buffer_t *b);
void    buffer_destroy(buffer_t *b);
void    buffer_reset(buffer_t *b);
bool    check_buffer_space(buffer_t *b, size_t len);

/*
 * assuming little-endian for the present
 */
void    buffer_write_char(buffer_t *b, unsigned char n);
void    buffer_write_short(buffer_t *b, unsigned short n);
void    buffer_write_int(buffer_t *b, unsigned int n);
void    buffer_write_long(buffer_t *b, unsigned long n);
void    buffer_write_bytes(buffer_t *b, const char *bytes, size_t len);
void    buffer_write_string(buffer_t *b, const char *str);

char    buffer_read_char(buffer_t *b);
short   buffer_read_short(buffer_t *b);
int     buffer_read_int(buffer_t *b);
long    buffer_read_long(buffer_t *b);
void    buffer_read_bytes(buffer_t *b, char *bytes, size_t len);
void    buffer_read_string(buffer_t *b, char *str, size_t len);

#endif
