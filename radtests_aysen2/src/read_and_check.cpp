#include "eth_lib.h"
#include "utils.h"

/*

Continuously read in packets and compare to some standard patterns.
The first 2 bytes identify the pattern
+ The first byte must be PAT_FILE_HDR, indicating this is a pattern (other packets are ignored)
+ The second byte is the pattern ID number
Program will stop with Ctrl+C or if the pattern ID is 0xff
*/


int usage(int argc, char *argv[]){
  printf("Usage: \n");
  printf("%s <device number> <pattern dir>\n", argv[0]);
  printf("<device number> = 1 or 2 (for eth1 or eth2)\n");  
  printf("<pattern dir> = directory with pattern files\n");  
  return 1;
}

int main(int argc,char *argv[])
{
  int i,n;
  char *tmp;
  char devname[12];
  char *patdir;
  char *inpat=0;
  char patname[256];
  int n_rec=0, n_pat=0, n_test=0;
  int nerr_pkt=0, nerr_byte=0;

  // check arguments
  if(argc != 3) return usage(argc, argv);

  // set and check device name
  tmp=argv[1];
  if(strcmp(tmp,"1") && strcmp(tmp,"2")) return usage(argc, argv);
  sprintf(devname, "/dev/schar%s",tmp);
  printf("devname %s\n",devname);

  // set patdir
  patdir = argv[2];
  
  // open device
  eth_open(devname);
  eth_register_mac();
  
  while(1){ // never ending loop
    // receive data
    n=eth_read(0);
    n_rec++;
    printf(" nread %d \n",n);
    printf(" nrdat %d \n",nrdat);

    if(nrdat<3) continue;
    if((rdat[0]&0xff) != PAT_FILE_HDR) continue;
    if((rdat[1]&0xff) == 0xff) break;
    n_pat++;
    sprintf(patname,"%s/%02x.pat",patdir,rdat[1]);
    printf("patname %s\n",patname);
    int ninpat = get_pattern(patname, &inpat, 0); // create buffer, read in pattern
    if(!inpat || ninpat<=0){
      printf("Could not open this pattern file! => skip.\n");
      nerr_pkt++; // count as packet error
      continue;
    }
    if(ninpat>MAX_DAT_SIZE){
      printf("\n\nWARNING! Pattern too large!  Must be less than MAX_DAT_SIZE=%d\n=>Truncating to %d!\n\n", MAX_DAT_SIZE, MAX_DAT_SIZE);
      ninpat=MAX_DAT_SIZE;
    }
    
    // compare packet with pattern
    n_test++;
    if(ninpat != nrdat){
      printf("ERROR! Pattern had %d bytes, but packet has %d bytes!\n",ninpat, nrdat);
      nerr_pkt++; // packet error
      continue;
    }

    if(memcmp(inpat,rdat,ninpat)){
      nerr_pkt++; // packet error
      
      printf("\n*** ERROR *** \n");
      printf("Following bytes don't match:\n");
      printf("i:   pt: rd:\n");
      // compare each byte
      for(i=0; i<ninpat;i++){
	if((0xff&inpat[i]) != (0xff&rdat[i])){
	  nerr_byte++; // bytes don't match!
	  printf("%04d  %02x  %02x",i, 0xff&inpat[i], 0xff&rdat[i]);
	  printf("\n");
	}
      }
      printf("\n");
    }else printf("Success.\n");
    
    free(inpat); // free memory with pattern
    printf("Send stop pattern (STOP.pat) to stop and report error counts.\n");
  }
  
  printf("Packets Received:     %d\n", n_rec);
  printf("Packets w Pattern ID: %d\n", n_pat);
  printf("Patterns Tested:      %d\n", n_test);
  printf("Packet Errors:        %d\n", nerr_pkt);
  printf("Byte Errors:          %d\n", nerr_byte);
  
  // close device
  eth_close();

  return 0;
}
