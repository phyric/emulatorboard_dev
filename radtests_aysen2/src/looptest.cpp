#include "eth_lib.h"
#include "utils.h"


int usage(int argc, char *argv[]){
  printf("Loop test.  Data from pattern file will be sent out and then read back and compared to original pattern.\n");
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
  char* inpat=0;

  if(argc != 3) return usage(argc, argv);

  // set and check device name
  tmp=argv[1];
  if(strcmp(tmp,"1") && strcmp(tmp,"2")) return usage(argc, argv);
  sprintf(devname, "/dev/schar%s",tmp);
  printf("devname %s\n",devname);

  // set pattern name
  char* patname = argv[2];
  printf("patname %s\n",patname);

  // Get pattern from file
  int ninpat = get_pattern(patname, &inpat, 1); // create buffer, read in pattern
  if(!inpat || ninpat<=0) return -1;
  if(ninpat>MAX_DAT_SIZE){
    printf("\n\nWARNING! Pattern too large!  Must be less than MAX_DAT_SIZE=%d\n=>Truncating to %d!\n\n", MAX_DAT_SIZE, MAX_DAT_SIZE);
    ninpat=MAX_DAT_SIZE;
  }

  //// Send data out ////

  // copy pattern to wdat
  nwdat = ninpat;
  memcpy(wdat, inpat, nwdat);

  // open device
  eth_open(devname);
  eth_register_mac();

  // send out data
  n=eth_write();

  //// Get data back ////
  
  // receive data
  n=eth_read(2);

  /* inject errors...
  for(i=0;i<n;i++){
    if(i%1000==3) rdat[i] = 0xff; // insert some "errors"
  }
  */  

  printf("ninpat %d\n",ninpat);
  printf("nwdat  %d\n",nwdat);
  printf("nrdat  %d\n",nrdat);
  
  // compare what we sent with what we received
  if(ninpat != nwdat){
    printf("ERROR! Pattern has %d bytes, but only sent %d bytes!?!?\n",ninpat, nwdat);
    return 2;
  }
  if(ninpat != nrdat){
    printf("ERROR! Sent %d bytes, but received %d bytes!\n",ninpat, nrdat);
    return 2;
  }

  // compare arrays
  int paterror = memcmp(inpat,rdat,ninpat);
  if(paterror){
    printf("\n*** ERROR *** \n");
    printf("Following bytes don't match:\n");
    printf("i:   pt: wr: rd:\n");
    // compare each byte
    for(i=0; i<ninpat;i++){
      if((0xff&inpat[i]) != (0xff&rdat[i])){
	// bytes don't match!
	printf("%04d  %02x  %02x  %02x",i, 0xff&inpat[i], 0xff&wdat[i], 0xff&rdat[i]);
	printf("\n");
      }
    }
    printf("\n");
  }else printf("Success.\n");

  // close device
  eth_close();
  free(inpat); // free buffer with pattern

  return paterror;
}
