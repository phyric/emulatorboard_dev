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
	eth_open("/dev/schar2");
	eth_reset();

	int e = write_command(7,pageid, block);
	cout << "write command 7 status = " << e << endl;

	char* pkt;
	e = read_command(&pkt);
	cout << "read command 7 status = " << e << endl;

	eth_close();

  	return e;
}
