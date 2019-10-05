/*https://stackoverflow.com/questions/26582920/mmap-memcpy-to-copy-file-from-a-to-b*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){

    int sfd, dfd, i;
    char *src, *dest;
    size_t filesize = 10*1024; //10MB

    /* HELPER FUNCTION TO WRITE ALL STDOUT TO FILE*/

    /*freopen("/home/l4mdc/sometextmem.txt", "a+", stdout);*/

    /* SOURCE */
    sfd = open("/home/l4mdc/sometext.txt", O_RDONLY);

    /* OPTION TO WRITE THE SIZE FROM THE GIVEN FILE*/
    /*size_t filesize;*/
    /*filesize = lseek(sfd, 0, SEEK_END);*/

    src = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, sfd, 0);

    /* DESTINATION */

    dfd = open("/lfs/memcpy", O_RDWR | O_CREAT, 0666);

    ftruncate(dfd, filesize);

    dest = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dfd, 0);

    /* COPY */

    memcpy(dest, src, filesize);

    /* DUP STDOUT TO /home/l4mdc/out.txt */

    int fd;
    int ret;
    fd =open("/home/l4mdc/out.txt", O_CREAT | O_APPEND | O_RDWR);
    ret = dup2(fd, 1);

    /* Read the file char-by-char from the mmap */
     unsigned char *f;
     f = (char *) mmap (0, filesize, PROT_READ, MAP_PRIVATE, dfd, 0);
     for (int i = 0; i < filesize; i++) {
         char c;
         c = f[i];
         putchar(c);
     }

    close(fd);

    munmap(src, filesize);
    munmap(dest, filesize);

    close(sfd);
    close(dfd);

    return 0;
}
