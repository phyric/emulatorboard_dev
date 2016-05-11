#include "eth_lib.h"
#include "utils.h"


int usage(int argc, char *argv[]){
  printf("Transmit data from pattern file out on given device (no read).\n");
  printf("Usage: \n");
  printf("%s <device number>  <pattern file>\n", argv[0]);
  printf("<device number> = 1 or 2 (for eth1 or eth2)\n");  
  printf("<pattern file> = File with pattern of bytes to send\n");  
  return 1;
}

int main(int argc,char *argv[])
{
  int i,n;
  char *tmp;
  char devname[12];

  if(argc != 3) return usage(argc, argv);

  // set and check device name
  tmp=argv[1];
  if(strcmp(tmp,"1") && strcmp(tmp,"2")) return usage(argc, argv);
  sprintf(devname, "/dev/schar%s",tmp);
  printf("devname %s\n",devname);

  char* patname = argv[2];
  printf("patname=%s\n",patname);

  //// Get pattern from file
  char* inpat=0;
  int ninpat = get_pattern(patname, &inpat, 1); // create buffer, read in pattern
  if(!inpat || ninpat<=0) return -1;

  // open device
  eth_open(devname);
  eth_register_mac();
  
  //// Check if pattern needs to be broken up
  int npieces = 1;
  if(ninpat>MAX_DAT_SIZE){
    npieces = 1 + (ninpat-1)/MAX_DAT_SIZE;
    printf("\nWARNING! Pattern too large for one packet (MAX_DAT_SIZE=%d)\n=>Breaking into %d packets...\n\n", MAX_DAT_SIZE, npieces);
  }

  //// Send the full packets
  for(i=1; i<npieces; i++){
    // copy piece to wdat
    nwdat = MAX_DAT_SIZE;
    memcpy(wdat, inpat, nwdat);
    // send out data
    n=eth_write();
    printf("Packet sent: ");
    dumphex(n,wdat);
    printf("\n");
  }

  //// Send the last packet with everthing that is left
  // copy pattern to wdat
  nwdat = 1+ (ninpat-1)%MAX_DAT_SIZE;
  memcpy(wdat, inpat, nwdat);
  // send out data
  n=eth_write();
  printf("Packet sent: ");
  dumphex(n,wdat);
  printf("\n");
  
  // close device
  eth_close();
  free(inpat); // free buffer with pattern
  
  return nwdat-n;
}
