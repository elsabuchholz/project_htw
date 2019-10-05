#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

//  __asm__ __volatile__ ("invd":::"memory");
//

#define DEF_FILE "/lfs/p1"
#define DEF_SIZE 20*1024*1024  // 20 MB


void cflush(uint32_t *startAddr, int len)
{

    void *addr = startAddr;
    while (addr < (void *)((uint8_t *)(startAddr) + len))
    {
        uint32_t *endPtr;
        __asm__ __volatile__ (
        "clflush (%0)"
        :
        : "r" (addr)
        : "memory"
        );

    addr += 64;
    }
}

int main (int argc, char *argv[])
{

    void *startPtr;
    uint32_t *endPtr;
    uint32_t *dataPtr;
    uint32_t offset = 0;
    int fd;
    int firstRepeat;
    int c;
    int index;

    char *fname   = DEF_FILE;
    int size      = DEF_SIZE;
    uint32_t data = 0x00;
    int noflush   = 0;
    int preflush  = 0;
    int postflush = 0;
    int rd        = 1;
    int wt        = 0;
    int wait      = 0;


    static char *optstring = "hcnf:s:w";
    static struct option longopts[] = {
                        {"help",        no_argument,     0,   'h'},
                        {"flush",       no_argument,     0,   'c'},
                        {"noflush",     no_argument,     0,   'n'},
                        {"file",  required_argument,     0,   'f'},
                        {"size",  required_argument,     0,   's'},
                        {"wait",        no_argument,     0,   'w'},
                        };
    // Process command line

    while (1)
    {
        c = getopt_long(argc, argv, optstring, longopts, &index);

        if (c == -1)
        {
            break;
        }

        switch (c)
        {
            case 'h':
                printf("%s [-c][-n][-w] [--file <filename>] [--size <size>] [data value]\n", argv[0]);
                return (0);
                break;
            case 'c':
                preflush = 1;
                break;
            case 'n':
                noflush = 1;
                break;
            case 'f':
                fname = optarg;
                break;
            case 's':
                size = strtoul(optarg, NULL, 0);
                break;
            case 'w':
                wait = 1;
                break;
        }
    }

    if (optind < argc)
    {
        data = strtoul(argv[optind], NULL, 0);
    wt = 1;
    rd = 0;
    }

    if (preflush)
    {
        rd = 0;
        wt = 0;
        postflush = 0;
    }

    if (rd && !noflush)
    {
        preflush = 1;
    }

    if (wt && !noflush)
    {
        postflush = 1;
    }


    // Set up the memory space
    fd = open(fname, O_RDWR | O_CREAT);
    if (fd < 0)
    {
        perror(fname);
        return(-1);
    }

    if (fallocate(fd, 0, 0, size) != 0)
    {
        perror("fallocate");
        return(-1);
    }

    startPtr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (startPtr == MAP_FAILED)
    {
        perror("mmap failed");
        return(-1);
    }

    // Perform the requested operation
    if (preflush)
    {
        // Flush the cache
        printf("flush\n");
        cflush(startPtr, size);
    }

    if (rd)
    {
        // Read Data
        printf("read\n");
        offset = 0;
        dataPtr = startPtr;
        endPtr = (uint32_t *)(startPtr + size);
        while (dataPtr < endPtr)
        {
            if (dataPtr != startPtr)
            {
                if (*dataPtr != data)
                {
                    data = *dataPtr;
                    printf("0x%08x: 0x%08x\n", offset, data);
                    firstRepeat = 1;
                }
                else if (firstRepeat == 1)
                {
                    printf("...\n");
                    firstRepeat = 0;
                }
            }
            else
            {
                // Always print the first line
                data = *dataPtr;
                printf("0x%08x: 0x%08x\n", offset, data);
                firstRepeat = 1;
            }

            offset += sizeof(*dataPtr);
            dataPtr = startPtr+offset;
        }
    }

    if (wt)
    {
        // Write Data
        printf("write\n");
        dataPtr = startPtr;
        endPtr = (uint32_t *)(startPtr + size);
        while (dataPtr < endPtr)
        {
            *dataPtr = data;
            dataPtr++;
        }

    }

    if (postflush)
    {
        // Flush the cache
        printf("flush\n");
        cflush(startPtr, size);
    }

    if (wait)
    {
        // Delay before closing the file
        printf("wait\n");
        sleep (30);
    }

    // Cleanup
    munmap(startPtr, size);
    close(fd);
    return(0);
}
