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
    size_t filesize;

    /* SOURCE */
    sfd = open("/home/l4mdc/sometext.txt", O_RDONLY);
    filesize = lseek(sfd, 0, SEEK_END);

    src = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, sfd, 0);

    /* DESTINATION */
    dfd = open("/lfs/memcpy", O_RDWR | O_CREAT, 0666);

    ftruncate(dfd, filesize);

    dest = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dfd, 0);

    /* COPY */

    memcpy(dest, src, filesize);
    /* Read the file char-by-char from the mmap
     */
     unsigned char *f;
     f = (char *) mmap (0, filesize, PROT_READ, MAP_PRIVATE, dfd, 0);
     for (int i = 0; i < filesize; i++) {
         char c;

         c = f[i];
         putchar(c);
     }


    /* int c;
     FILE *file;
     file = fopen("test.txt", "r");
     if (file) {
       while ((c = getc(file)) != EOF)
        putchar(c);
        fclose(file);
      }

    for (i = 1; i < filesize; ++i) {
        printf("%s\n", i, dest[i]);
    }*/
    munmap(src, filesize);
    munmap(dest, filesize);

    close(sfd);
    close(dfd);

    return 0;
}
