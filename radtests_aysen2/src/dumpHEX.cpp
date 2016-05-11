#include "eth_lib.h"
#include "utils.h"

int usage(int argc, char *argv[]){
  printf("Continuously read and dump incoming packets to stdout. \n");
  printf("Usage: \n");
  printf("%s <device number>\n", argv[0]);
  printf("<device number> = 1 or 2 (for eth1 or eth2)\n");  
  return 1;
}

int main(int argc,char *argv[])
{
  int n;
  char *tmp;
  char devname[12];

  // check arguments
  if(argc != 2) return usage(argc, argv);

  // set and check device name
  tmp=argv[1];
  if(strcmp(tmp,"1") && strcmp(tmp,"2")) return usage(argc, argv);
  sprintf(devname, "/dev/schar%s",tmp);
  printf("devname %s\n",devname);


  // open device
  eth_open(devname);
  eth_register_mac();

  while(1){ // never ending loop, Ctrl-C to stop
    // receive data
    n=eth_read(0);
    if (n<=6) continue;
    printf(" nread %d \n",n);
    printf(" nrdat %d \n",nrdat);
    
    printf("Dump: \"");
    dumphex(n,rpkt);
    printf("\"\n");
    printf("Use Ctrl-C to stop.\n");
    
  }


  // close device
  eth_close();

  return 0;
}
