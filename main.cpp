
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

void mem_usage(){
  const unsigned int vsize_pos = 23;
  const unsigned int rss_pos = 24;
  const unsigned int buffer_size = 24;
  unsigned long vsize;
  signed long rss;
  FILE * fd;
  int sub;
  unsigned int sub_n;
  unsigned int sub_pos_n;
  char buff[buffer_size];
  fd = fopen ("/proc/2765/stat", "r");
  if (fd != NULL){
    sub_n = 0;
    sub_pos_n = 0;
    memset(buff, 0x00, buffer_size);
    while (true){
      sub = fgetc(fd);
      if (sub != 0x20 && sub != 0x0A && sub != EOF){
        buff[sub_n] = (char) sub;
        if (sub_n < buffer_size - 2) sub_n++;
      } else {
        if (sub_n > 0){
          buff[sub_n + 1] = 0x00;
          sub_n = 0;
          sub_pos_n++;
          switch(sub_pos_n){
            case vsize_pos:
            sscanf(buff, "%lu", &vsize);
            //std::cout << vsize << ": " << sub_pos_n << std::endl;
            std::cout << "Virtual memory size: " << vsize / 1024 / 1024 << " Mb" << std::endl;
            break;
            case rss_pos:
            sscanf(buff, "%ld", &rss);
            rss = rss * sysconf(_SC_PAGE_SIZE);
           // std::cout << rss << ": " << sub_pos_n << std::endl;
            std::cout << "Resident Set Size: " << rss / 1024 / 1024 << " Mb" << std::endl;
            break;
          }
          memset(buff, 0x00, buffer_size);
        }
      }
      if (sub == EOF) break;
    }
    fclose(fd);
  }
}

int main(){
  mem_usage();
  return 0;
}
