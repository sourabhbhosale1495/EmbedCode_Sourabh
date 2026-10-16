# EmbedKit – Embedded C Utility Library

## Author
Sourabh Sadashiv Bhosale

## Module: Ring Buffer

A circular FIFO buffer implemented in C99 for storing 8-bit unsigned data. It supports initialization, writing, reading, count queries, and full/empty checks. It also demonstrates overflow handling, underflow handling, and bitwise index wrapping.

## Requirements
- GCC compiler
- C99-compatible compiler

## Build Instructions

Compile the program:

```bash
gcc -Wall -std=c99 ringbuf.c -o ringbuf
```

Run on Linux/macOS:

```bash
./ringbuf
```

Run on Windows:

```powershell
.\ringbuf.exe
```

## Tests Demonstrated
- Write 8 bytes into the buffer
- Reject a write when the buffer is full
- Read 3 bytes in FIFO order
- Reuse freed slots with wrap-around
- Read all remaining bytes
- Reject a read when the buffer is empty

## Optimization
Bitwise AND is used for circular index wrapping. This works when the buffer size is a power of two and can be faster than modulo on microcontrollers without a hardware divider.
