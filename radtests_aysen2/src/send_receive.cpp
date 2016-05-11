#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "eth_lib.h"
#include "utils.h"
#include "commands.h"

using namespace std;

int main(int argc,char *argv[])
{
	int pageid = atoi(argv[1]);
	cout << "pageid = " << pageid << endl;

	char block[RAMPAGE_SIZE];
	FILE* infile = fopen(argv[2],"r");
	fread(block, sizeof(char), RAMPAGE_SIZE, infile);
	memcpy(wdat,block,RAMPAGE_SIZE);
	cout << "block = " << block << endl;

	//nwdat = 4+RAMPAGE_SIZE;

	// open device
	eth_open("/dev/schar1");
	eth_reset();

	int e = write_command(7,pageid, block);
	cout << "write command 7 status = " << e << endl;

	char* pkt;
	e = read_command(&pkt);
	cout << "read command 7 status = " << e << endl;

	//eth_close();

	
	//eth_open(devname);
	//eth_reset();

	int q = write_command(3, pageid);
	cout << "write command 3 status = " << q << endl;

	int p = read_command(&pkt);		 
	cout << "read command status = " << p << endl;

	char block_back[RAMPAGE_SIZE];
	char hdr[4];
	memcpy(block_back, (const void*)&pkt[4], RAMPAGE_SIZE);

	cout << "header = " << memcpy(hdr, (const void*)&pkt[0], 4) << endl;
 	cout << "block_back = " << block_back << endl;
	cout << "block_back size = " << sizeof(block_back) << endl;

	//int n=eth_read(0);
	//cout << "n read = " << n << endl;

	// close device
	eth_close();
  
  	return q;
}
