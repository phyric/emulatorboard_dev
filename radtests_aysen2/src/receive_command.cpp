#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "eth_lib.h"
#include "utils.h"
#include "commands.h"

int usage(int argc, char *argv[]){
  printf("Transmit command on given device.\n");
  printf("Usage: \n");
  printf("%s <device number>\n", argv[0]);
  printf("<device number> = 1 or 2 (for eth1 or eth2)\n");  
  return 1;
}

int main(int argc,char *argv[])
{
  if(argc != 2) return usage(argc, argv);

  // set and check device name
  char* tmp=argv[1];
  if(strcmp(tmp,"1") && strcmp(tmp,"2")) return usage(argc, argv);
  char devname[12];
  sprintf(devname, "/dev/schar%s",tmp);
  printf("devname %s\n",devname);
  
  // open device
  eth_open(devname);
  eth_register_mac();
  
  char* buf;
  //int commandnum=read_command(&buf);
  int commandnum;
  while (1) {
    commandnum = read_command(&buf);
    if (commandnum >= 0) break;
  }

  std::cout<<"Received command "<<commandnum<<std::endl;
  dumphex(10, buf); // dump first 10 bytes
  std::cout<<"... "<<std::endl;

  switch(commandnum){
    unsigned short pageid;
    unsigned int counts1[8];
    unsigned short counts2[24];

  case 1:
    std::cout<<"Snap12 Error Counts:"<<std::endl;
    memcpy(counts1, (const void*)&buf[2], 8*4);
    for(int i=0; i<8; i++)
      std::cout<<"  count"<<i<<"  "<<counts1[i]<<std::endl;
    break;
  case 2:
    std::cout<<"Translator Error Counts:"<<std::endl;
    memcpy(counts2, (const void*)&buf[2], 24*2);
    for(int i=0; i<24; i++)
      std::cout<<"  count"<<i<<"  "<<counts2[i]<<std::endl;
    break;
  case 3:
    std::cout<<"Block RAM:"<<std::endl;
    //unsigned short pageid = *(unsigned short*)&buf[0]; // bytes are reversed in array :-(
    pageid = (buf[0]<<8) | (buf[1]);
    std::cout<<"pageid "<<pageid<<std::endl;
    dumphex(nrdat-2, &buf[2]); // dump the block
    std::cout<<std::flush<<std::endl;
    break;
  case 4:
    std::cout<<"Finisar test:"<<std::endl;
    dumphex(nrdat, buf); // dump the packet
    std::cout<<std::flush<<std::endl;
    break;
  default:
    std::cout<<"Unknown command."<<std::endl;
    break;
  }

  // close device
  eth_close();
  
  return commandnum<0;
}
