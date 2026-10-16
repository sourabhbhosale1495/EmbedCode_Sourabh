#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 8

typedef struct {
    uint8_t data[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} RingBuffer;


void ringbuf_init(RingBuffer *rb);
int ringbuf_write(RingBuffer *rb, uint8_t value);
int ringbuf_read(RingBuffer *rb, uint8_t *value);
uint8_t ringbuf_count(const RingBuffer *rb);

uint8_t ringbuf_count(const RingBuffer *rb)
{
    return rb->count;
}

int ringbuf_is_empty(const RingBuffer *rb)
{
    return (rb->count == 0);
}

int ringbuf_is_full(const RingBuffer *rb)
{
    return (rb->count == BUFFER_SIZE);
}


int ringbuf_is_full(const RingBuffer *rb);

int ringbuf_write(RingBuffer *rb, uint8_t value)
{
    if (ringbuf_is_full(rb))
    {
        return 0;
    }
    rb->data[rb->head] = value;
     /*
     * Bitwise AND is often faster than modulo (%) on
     * MCUs without a hardware divider.
     * This works correctly only when BUFFER_SIZE
     * is a power of 2, such as 8.
     */
    rb->head = (rb->head + 1) & (BUFFER_SIZE - 1);
    rb->count++;
    return 1;
}


int ringbuf_read(RingBuffer *rb, uint8_t *value)
{
    if (ringbuf_is_empty(rb))
    {
        return 0;
    }
    *value = rb->data[rb->tail];
    /*
     * Bitwise AND is often faster than modulo (%) on
     * MCUs without a hardware divider.
     * This works correctly only when BUFFER_SIZE
     * is a power of 2, such as 8.
     */
    rb->tail = (rb->tail + 1) & (BUFFER_SIZE - 1);
    rb->count--;
    return 1;
}


int ringbuf_is_empty(const RingBuffer *rb);


void ringbuf_init(RingBuffer *rb)
{
    memset(rb->data, 0, BUFFER_SIZE);
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}


int main(void)
{
    RingBuffer rb;
    uint8_t received;

    ringbuf_init(&rb);

    printf("Buffer initialized successfully.\n");

    for (uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
        uint8_t value = (uint8_t)(0x41 + i);
        if (ringbuf_write(&rb, value))
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)\n",
                   (unsigned int)value,
                   (unsigned int)ringbuf_count(&rb));
        }
        else
        {
            printf("[WRITE] 0x%02X -> FAIL\n",
                   (unsigned int)value);
        }
    }

    printf("Buffer full: %s\n",
           ringbuf_is_full(&rb) ? "YES" : "NO");

    printf("Final count: %u\n",
           (unsigned int)ringbuf_count(&rb));
    
    
    if (ringbuf_write(&rb, 0x99))
    {
        printf("[WRITE] 0x99 -> OK (unexpected)\n");
    }
    else
    {
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n");
    }
    printf("Count after overflow attempt: %u\n",
           (unsigned int)ringbuf_count(&rb));
    
    
    for (uint8_t i = 0; i < 3; i++)
    {
        if (ringbuf_read(&rb, &received))
        {
            printf("[READ] -> 0x%02X (count=%u)\n",
                   (unsigned int)received,
                   (unsigned int)ringbuf_count(&rb));
        }
        else
        {
            printf("[READ] -> FAIL (buffer empty)\n");
        }
    }

    printf("Count after 3 reads: %u\n",
           (unsigned int)ringbuf_count(&rb));
    
    printf("\n--- Wrap-Around Test ---\n");

    for (uint8_t i = 0; i < 3; i++)
    {
        uint8_t value = (uint8_t)(0x49 + i);

        if (ringbuf_write(&rb, value))
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)\n",
                   (unsigned int)value,
                   (unsigned int)ringbuf_count(&rb));
        }
        else
        {
            printf("[WRITE] 0x%02X -> FAIL\n",
                   (unsigned int)value);
        }
    }

    printf("Buffer full: %s\n",
           ringbuf_is_full(&rb) ? "YES" : "NO");

    printf("Final count: %u\n",
           (unsigned int)ringbuf_count(&rb));
    
    
    printf("\n--- Read Remaining Bytes ---\n");

    while (!ringbuf_is_empty(&rb))
    {
        if (ringbuf_read(&rb, &received))
        {
            printf("[READ] -> 0x%02X (count=%u)\n",
                   (unsigned int)received,
                   (unsigned int)ringbuf_count(&rb));
        }
    }

    printf("Buffer empty: %s\n",
           ringbuf_is_empty(&rb) ? "YES" : "NO");

    printf("Final count: %u\n",
           (unsigned int)ringbuf_count(&rb));
    
    printf("\n--- Empty Buffer Test ---\n");

    if (ringbuf_read(&rb, &received))
    {
        printf("[READ] -> 0x%02X (unexpected success)\n",
               (unsigned int)received);
    }
    else
    {
        printf("[READ] (empty) -> FAIL (buffer empty)\n");
    }

    printf("Final count: %u\n",
           (unsigned int)ringbuf_count(&rb));

    return 0;
}
