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

	// open device
	eth_open("/dev/schar2");
	eth_reset();

	int q = write_command(3, pageid);
	cout << "write command 3 status = " << q << endl;
	
	char* pkt;
	int p = read_command(&pkt);		 
	cout << "read command status = " << p << endl;

	char block_back[RAMPAGE_SIZE];
	char hdr[4];
	memcpy(block_back, (const void*)&pkt[4], RAMPAGE_SIZE);

 	cout << "block_back = " << block_back << endl;

	// close device
	eth_close();
  
  	return q;
}
