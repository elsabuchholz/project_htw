/*https://stackoverflow.com/questions/26582920/mmap-memcpy-to-copy-file-from-a-to-b*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char **argv){

    int sfd, dfd;
    char *src, *dest;
    size_t filesize = 10*1024; //10MB
    struct timeval start, end;
    double delta;
    FILE * fpt;

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
    gettimeofday(&start, 0);
    memcpy(dest, src, filesize);
    gettimeofday(&end, 0);

    /* GET THE TIME AND PRINT TO STDOUT */
     delta =((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
     /*printf("%f\n",delta);*/
     fpt = fopen ("time.txt", "a");
     fprintf(fpt, "%f\n",delta);

     fclose(fpt);

    /* DUP STDOUT TO /home/l4mdc/out.txt */

    /*int fd;
    int ret;
    fd =open("/home/l4mdc/out.txt", O_CREAT | O_APPEND | O_RDWR);
    ret = dup2(fd, 1);*/


    int out = open("cout.log", O_RDWR|O_CREAT|O_APPEND, 0600);
        if (-1 == out) { perror("opening cout.log"); return 255; }
    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
        if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }

    /* Read the file char-by-char from the mmap */
     unsigned char *f;
     f = (char *) mmap (0, filesize, PROT_READ, MAP_PRIVATE, dfd, 0);
     for (int i = 0; i < filesize; i++) {
         char c;
         c = f[i];
         putchar(c);
     }

    /*dup2(1,fd);
    close(fd);*/

    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);
/* BACK TO STDOUT */


    /*int fd2;
    fd2 =open("/home/l4mdc/out2.txt", O_CREAT | O_APPEND | O_RDWR);
    ret = dup2(fd2, 1);
    close(fd2);*/

    munmap(src, filesize);
    munmap(dest, filesize);

    close(sfd);
    close(dfd);

    return 0;
}
