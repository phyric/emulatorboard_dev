#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "eth_lib.h"
#include "utils.h"
#include "commands.h"

int usage(int argc, char *argv[]){
  printf("Transmit command on given device.\n");
  printf("Usage: \n");
  printf("%s <device number>  <command number> [<pageid>]\n", argv[0]);
  printf("<device number> = 1 or 2 (for eth1 or eth2)\n");  
  printf("<command number> = 1-3\n");  
  printf("<pagid> = address for RAM page\n");  
  return 1;
}

int main(int argc,char *argv[])
{
  if(argc < 3) return usage(argc, argv);

  // set and check device name
  char* tmp=argv[1];
  if(strcmp(tmp,"1") && strcmp(tmp,"2")) return usage(argc, argv);
  char devname[12];
  sprintf(devname, "/dev/schar%s",tmp);
  printf("devname %s\n",devname);
  
  int commandnum = atoi( argv[2] );
  printf("commandnum=%d\n",commandnum);

  int adrnum = 0;
  if (argc > 3){
    adrnum = atoi( argv[3] );
    printf("adrnum=%02x\n",adrnum);
  }

  // open device
  eth_open(devname);
  eth_register_mac();
  
  int e=write_command(commandnum,adrnum);
  
  // close device
  eth_close();
  
  return e;
}
