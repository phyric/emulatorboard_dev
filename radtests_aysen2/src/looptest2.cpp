#include "eth_lib.h"
#include "utils.h"


int usage(int argc, char *argv[]){
  printf("Automated loop test.  The given number of loop tests will be performed using the hard coded list of pattern files. \n");
  printf("Usage: \n");
  printf("%s <device number>  <iterations>\n", argv[0]);
  printf("<device number> = 1 or 2 (for eth1 or eth2)\n");  
  printf("<iterations> = the number of loop tests to do\n");  
  return 1;
}

int main(int argc,char *argv[])
{
  int i,n,t;
  int ntests, nerrors;
  char *tmp;
  char devname[12];
  char *inpat=0;

  if(argc != 3) return usage(argc, argv);

  // set and check device name
  tmp=argv[1];
  if(strcmp(tmp,"1") && strcmp(tmp,"2")) return usage(argc, argv);
  sprintf(devname, "/dev/schar%s",tmp);
  printf("devname %s\n",devname);

  // set ntests
  ntests = atoi(argv[2]);
  nerrors=0;
  if(ntests>20000){ 
    printf("Cap the number of tests at 20000.\n");
    ntests=20000;
  }

  // open device
  eth_open(devname);
  eth_register_mac();

  for(t=0; t<ntests; t++){
    
    char patname[] = "pats/test1KB.test";
    if(t%3==1) strcpy(patname,"pats/test2KB.test");
    if(t%3==2) strcpy(patname,"pats/test8KB.test");
    
    // Get pattern from file
    int ninpat = get_pattern(patname, &inpat, 1); // create buffer, read in pattern
    if(!inpat || ninpat<=0) return -1;
    
    if(ninpat>MAX_DAT_SIZE){
      printf("\n\nWARNING! Pattern too large!  Must be less than MAX_DAT_SIZE=%d\n=>Truncating to %d!\n\n", MAX_DAT_SIZE, MAX_DAT_SIZE);
      ninpat=MAX_DAT_SIZE;
    }
    
    // copy pattern to wdat
    nwdat = ninpat;
    memcpy(wdat, inpat, nwdat);
    
    // send out data
    n=eth_write();
    
    // receive data
    n=eth_read(2);
    
    printf("p,w,r:%d,%d,%d\n",ninpat,nwdat,nrdat);
    
    // compare what we sent with what we received
    if(ninpat != nwdat){
      printf("ERROR! Patter has %d bytes, but only sent %d bytes!?!?\n",ninpat, nwdat);
      return 1;
    }
    if(ninpat != nrdat){
      printf("ERROR! Sent %d bytes, but received %d bytes!\n",ninpat, nrdat);
      return 1;
    }

    //// Insert an error
    //rdat[10] = 0xff;

    // compare arrays
    if(memcmp(inpat,rdat,ninpat)){ // Compare data to pattern
      printf("\n*** ERROR *** \n");
      printf("Following bytes don't match:\n");
      printf("i:   pt: wr: rd:\n");
      // compare each byte
      for(i=0;i<ninpat;i++){
	if((0xff&inpat[i]) != (0xff&rdat[i])){
	  // bytes don't match!
	  printf("%04d  %02x  %02x  %02x",i, 0xff&inpat[i], 0xff&wdat[i], 0xff&rdat[i]);
	  printf("\n");
	  nerrors++;
	}
      }
    }
    printf("\n");
    free(inpat); // free buffer with pattern
  }

  printf("Iterations: %d\n", ntests);
  printf("Errors: %d\n", nerrors);
  
  // close device
  eth_close();

  return 0;
}
