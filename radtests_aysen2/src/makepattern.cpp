#include "eth.h"
#include <stdio.h>
#include <stdlib.h>


int usage(int argc, char* argv[]){
  printf("Create a pattern file with and ID = <ID>&0xff\n");
  printf("Usage:\n");
  printf("%s <ID>\n", argv[0]);
  printf("<ID> = An integer >=256 that will be converted to hex to make the pattern ID.\n");
  printf("       ID<=255 makes the STOP pattern file.\n");
  return 1;
}

int main (int argc, char* argv[])
{
  long int n;
  long int size;
  char* outbuf=0;
  char filename[16];
  
  if(argc!=2) return usage(argc, argv);
  
  /* generate pattern */
  size = atoi(argv[1]);
  if(size<=255){
    // A packet must have at least 9 bytes or else it takes 2 sends to be read, thus I add some 0x00 to the end
    size=10;
    outbuf=(char*)calloc(size,sizeof(char));
    if(!outbuf){
      printf("Failed to allocate memory!\n");
      return 1;
    }
    outbuf[0]=PAT_FILE_HDR;
    outbuf[1]=0xff;
    for( n = 2; n < size; n++ ){
      outbuf[n] = 0x00 ;
    }
    sprintf(filename,"STOP.pat");
  }else{
    outbuf=(char*)calloc(size,sizeof(char));
    if(!outbuf){
      printf("Failed to allocate memory!\n");
      return 1;
    }
    outbuf[0]=PAT_FILE_HDR;
    outbuf[1]=(0xff&size);
    sprintf(filename,"%02x.pat",0xff&outbuf[1]);
    for( n = 2; n < size; n++ ){
      outbuf[n] = n&0xff ;
    }
  }
  
  /* open output file */
  FILE *outfile = fopen(filename,"w");
  if(!outfile){
    printf("Could not open output file %s for write!\n",filename);
    return 1;
  }
  
  /* write pattern to file */
  fwrite( outbuf, sizeof( char ), size, outfile );

  /* close file */
  fclose( outfile );
  free(outbuf);
  return 0;
}
