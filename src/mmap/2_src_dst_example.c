/*https://stackoverflow.com/questions/26582920/mmap-memcpy-to-copy-file-from-a-to-b*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv){

    int sfd, dfd;
    char *src, *dest;
    size_t filesize;

    /* SOURCE */
    sfd = open(/home/l4mdc/sometext.txt, O_RDONLY);
    filesize = lseek(sfd, 0, SEEK_END);

    src = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, sfd, 0);

    /* DESTINATION */
    dfd = open(/lfs/memcpy, O_RDWR | O_CREAT, 0666);

    ftruncate(dfd, filesize);

    dest = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dfd, 0);

    /* COPY */

    memcpy(dest, src, filesize);

    munmap(src, filesize);
    munmap(dest, filesize);

    close(sfd);
    close(dfd);

    return 0;
}
