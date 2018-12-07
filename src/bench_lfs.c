#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <xlocale.h>

int main(int argc, char const *argv) {
  /* code */
  int fd = open("./sometext.txt", O_RDWR, S_IRUSR | S_IWUSR);
  struct stat sb;

  if (fstat(fd,&sb) == -1){
    perror("could not get file size.\n");
  }
  printf("file size is %ld\n",sb.st_size );
  char *file_in_memory = mmap(NULL,sb.st_size, PROT_READ  | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("printing file as an arry of characters.\n\n");

  for (int i=0; i < sb.st_size; i++)
  {
    if ((i % 2) == 0){
      file_in_memory[i] = toupper(file_in_memory[i]);
    }
    printf("%c",file_in_memory[i]);
  }
  printf("\n");

  munmap(file_in_memory, sb.st_size);
  close(fd);

  return 0;
}
