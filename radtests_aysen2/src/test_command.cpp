#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "eth_lib.h"
#include "utils.h"
#include "commands.h"

using namespace std;

int usage(int argc, char *argv[]){
  printf("Run command on device.\n");
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
  
  if(commandnum==1){

    cout<<"Test Snap12 Error Counts"<<endl;
    unsigned int snap12_counts[8];    
    int err = retrieve_Snap12ErrorCounts(snap12_counts);
    if(err){
      cout<<"Error "<<err<<" getting snap12 error counts."<<endl;
    }else{
      for(int i=0; i<8; i++)
	cout<<"  snap12_counts["<<i<<"] =  "<<snap12_counts[i]<<endl;
    }
    
  }else if(commandnum==2){

    cout<<"test Translator Error Counts"<<endl;
    unsigned short trans_counts[24];
    int err = retrieve_TranslatorErrorCounts(trans_counts);
    if(err){
      cout<<"Error "<<err<<" getting translator error counts."<<endl;
    }else{
      for(int i=0; i<24; i++)
	cout<<"  trans_counts["<<i<<"] =  "<<trans_counts[i]<<endl;
    }
    
  }else if(commandnum==3){

    cout<<"test RAM Page"<<endl;
    int pageid;
    int startb = 2816;
    int numb = 128;
    int startc = 3072;
    int numc = 32;
    char f3hdr[4];
    char block[RAMPAGE_SIZE];

    for(pageid = startb; pageid < startb + numb; pageid++){
      int err = retrieve_RAMPage(pageid, block, f3hdr);
      if(err){
	cout<<"Error "<<err<<" getting RAM page "<<pageid<<endl;
      }else{
	// check RAM page
	cout<<" page: "<<pageid<<": ";
	//dumphex(RAMPAGE_SIZE,block);
	//save_page_to_file(pageid, block); //run this to make files with contents of RAM pages
	// compare block to file
	char tmpblock[RAMPAGE_SIZE];
	get_page_from_file(pageid,tmpblock);
	if( memcmp(block, tmpblock, RAMPAGE_SIZE) ){
	  cout<<" ERROR!";
	}else{
	  cout<<" good";
	}
	cout<<endl;
      }
    }
    for(pageid = startc; pageid < startc + numc; pageid++){
      int err = retrieve_RAMPage(pageid, block, f3hdr);
      if(err){
	cout<<"Error "<<err<<" getting RAM page "<<pageid<<endl;
      }else{
	// check RAM page
	cout<<" page: "<<pageid<<": ";
	//dumphex(RAMPAGE_SIZE,block);  // just dump it to screen
	//save_page_to_file(pageid, block); //run this to make files with contents of RAM pages
	// compare block to file
	char tmpblock[RAMPAGE_SIZE];
	get_page_from_file(pageid,tmpblock);
	if( memcmp(block, tmpblock, RAMPAGE_SIZE) ){
	  cout<<" ERROR!";
	}else{
	  cout<<" good";
	}
	cout<<endl;
      }
    }
    
  }else{
    cout<<"Unknown test!"<<endl;
  }


  // close device
  eth_close();
  
  return commandnum<0;
}
