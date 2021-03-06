#include "RadTestWorker.h"


RadTestWorker::RadTestWorker(QObject *parent) 
  : QObject(parent),
    currenttest(1),
    seqn(0),
    newstart(1),
    f4try(0),
    resendBRAMs(1),
    netcheckdone(0),
    resetlevel(1),
    bump(0),
    radlog(0)
{

  radlog = &cout; // initialize *radlog to cout

  // initializations
  for(int i=0; i<NTESTS; i++){
    dotest[i]=0;
    counter[i][0]=0;
    counter[i][1]=0;
  }
  for(int i=0; i<8; i++){
    snap12_counts[i]=0;
    snap12_counts_last[i]=0;
  }
  for(int i=0; i<12; i++){
    voting_counts[i]=0;
    voting_counts_last[i]=0;
  }
  for(int i=0; i<24; i++){
    trans_counts[i]=0;
    trans_counts_last[i]=0;
  }
  curr_id[0]=-1;
  prev_id[0]=-1;
  pre2_id[0]=-1;
  for(int i=0; i<8; i++){
    eth_counts[i]=0;
    eth_counts_last[i]=0;
  }
  
  // runTest() when timer times out
  connect(&_timer, SIGNAL(timeout()), this, SLOT( runTest()));

}

void RadTestWorker::runTest(){

  if(time(0)%5<2){
    if(!bump){
      cout<<"-"<<endl;
      bump=1;
    }
  }else{
    bump=0;
  }
  
  if(notests()) return;
  else{ // go to the next selected test
    while(!dotest[currenttest]){
      currenttest = (currenttest+1)%NTESTS;
    }
  }

  if(newstart){ // there was a new start
    memcpy(reinit, dotest, NTESTS*sizeof(bool)); // set flag to reinit counters for current tests
    newstart=0;
  }

  //cout<<"test "<<currenttest<<"..."<<endl;
  eth_open("/dev/schar1");
  

  if(currenttest==1){
    ////////////////// Snap12 Test ///////////////////

    if(reinit[currenttest]){
      reinit[currenttest]=0;
      retrieve_Snap12ErrorCounts(snap12_counts_last); // get initial counters after (re)start 
      *radlog<<time_string()<<" ==> (Re)Initializing snap12 counters: "<<endl;
      for(int i=0; i<8; i++)  *radlog<<"    --> snap12_counts_last["<<i<<"] =  "<<snap12_counts_last[i]<<endl;
    }
    
    cout<<"Test Snap12 Error Counts"<<endl;

    SEUcounted=0;
    for(ntry=0; ntry<maxtry; ntry++){
      errnum = retrieve_Snap12ErrorCounts(snap12_counts);
      if(errnum){
	*radlog<<time_string()<<" ***ERROR*** (F1e1) Problem (errnum="<<errnum<<") getting snap12 error counts. (ntry="<<ntry<<")"<<endl;
	//// Note: this is not counted as a SEU.  If this is never successful it will trigger a reset without counting as a SEU
	wait_msec(trywait); continue;// try again
      }
      
      // check if counters changed
      change=0;
      for(int i=0; i<8; i++) change += snap12_counts[i] - snap12_counts_last[i];
      
      if(change){ // they did change...
	
	// dump counters to the log file
	*radlog<<time_string()<<" ***ERROR*** (F1e2) Change in snap12 error counters (ntry="<<ntry<<"): "<<endl;
	for(int i=0; i<8; i++)  *radlog<<"    --> snap12_counts["<<i<<"] =  "<<snap12_counts[i]<<endl;
	memcpy(snap12_counts_last, snap12_counts, 8*sizeof(int)); // update last counts

	if(SEUcounted==0){ // only count as 1 SEU/burst
	  SEUcounted=1;
	  counter[1][0]++;
	  counterchanged();
	}
	wait_msec(trywait); continue;// try again
      }
      break;
    }

    if(ntry==maxtry){ // need reset
      pause();
      if(resetlevel<2){
	counter[1][1]++;
	counterchanged();
      }
      needReset(resetlevel);
      resetlevel=2;
      if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  F1 need reset"<<endl;
      return;
    }
    //////////////////////////////////////////////////
    
    
  }else if(currenttest==2){
    ////////////// Translator Test ///////////////////
    
    if(reinit[currenttest]){
      reinit[currenttest]=0;
      retrieve_TranslatorErrorCounts(trans_counts_last); // get initial counters after (re)start 
      *radlog<<time_string()<<" ==> (Re)Initialize translator counters: "<<endl;
      for(int i=0; i<24; i++) *radlog<<"    --> trans_counts_last["<<i<<"] =  "<<trans_counts_last[i]<<endl;
    }
    
    cout<<"Test Translator Error Counts"<<endl;
    
    SEUcounted=0;
    for(ntry=0; ntry<maxtry; ntry++){
      errnum = retrieve_TranslatorErrorCounts(trans_counts);
      if(errnum){
	*radlog<<time_string()<<" ***ERROR*** (F2e1) Problem (errnum="<<errnum<<") getting translator error counts. (ntry="<<ntry<<")"<<endl;
	//// Note: this is not counted as a SEU.  If this is never successful it will trigger a reset without counting as a SEU
	wait_msec(trywait); continue;// try again
      }
      
      // check if counters changed
      change=0;
      for(int i=0; i<24; i++) change += trans_counts[i] - trans_counts_last[i];

      if(change){ // they did change...

	*radlog<<time_string()<<" ***ERROR*** (F2e2) Change in translator error counters (ntry="<<ntry<<"): "<<endl;
	for(int i=0; i<24; i++) *radlog<<"    --> trans_counts["<<i<<"] =  "<<trans_counts[i]<<endl;
	memcpy(trans_counts_last, trans_counts, 24*sizeof(short)); // move current counts to last counts

	if(SEUcounted==0){ // only count as 1 SEU/burst
	  SEUcounted=1;
	  counter[2][0]++;
	  counterchanged();
	}
	wait_msec(trywait); continue;// try again
      }
      break;
    }

    if(ntry==maxtry){ // need reset
      pause();
      if(resetlevel<2){
	counter[2][1]++;
	counterchanged();
      }
      needReset(resetlevel);
      resetlevel=2;
      if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  F2 need reset"<<endl;
      return;
    }
    //////////////////////////////////////////////////
      
    
  }else if(currenttest==3){
    /////////////// RAM Page Test ////////////////////

    if(reinit[currenttest]){
      reinit[currenttest]=0;
      *radlog<<time_string()<<" ==> (Re)Initializing RAM Page Tests"<<endl;
    }
    cout<<"Test RAM Page"<<endl;
    
    const int startb = firstb;
    const int numb = 256; // 8; //256; //128;

    if(resendBRAMs){
      for(pageid = startb; pageid < startb + numb; pageid++){ // block RAM pages
	errnum = write_RAMpage(pageid);
	//cout<<"errnum = "<<errnum<<"  pageid = "<<pageid<<endl;
	switch(errnum){
	case 0: 
	  continue; // everything okay
	case 1: 
	  if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  F7 could not open file with BRAM page"<<endl;
	  return; // did not find file with RAM page
	case 2: 
	  if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  F7 persistent mismatch between sent and read BRAM page"<<endl;
	  return; // persistent mismatch after reading back, needs reset
	case 3:
	  if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  F7 problem sending BRAM to board"<<endl;
	  return; // problem sending, needs reset
	default: // what happened?
	  cout<<"WARNING: Unknown return value from write_RAMpage!"<<endl;
	}
      }
      f7_needed(0); // toggle f7 back off
    }
    
    for(pageid = startb; pageid < startb + numb; pageid++){ // block RAM pages
      errnum = check_RAMpage(pageid);
      //cout<<"errnum = "<<errnum<<"  pageid = "<<pageid<<endl;
      switch(errnum){
      case 0: 
	continue; // everything okay
      case 1: 
	if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  F3 could not open file with BRAM page"<<endl;
	//continue; // did not find file with RAM page (but should have been created for next time)
	return; // did not find file with RAM page
      case 2: 
	if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  F3 BRAM page does not match"<<endl;
	return; // needs reset
      default: // what happened?
	cout<<"WARNING: Unknown return value from check_RAMpage!"<<endl;
      }
    }
    //////////////////////////////////////////////////

    
  }else if(currenttest==4){
    ////////////// Finisar/Network Test ///////////////////
    
    if(reinit[currenttest]){
      reinit[currenttest]=0;
      *radlog<<time_string()<<" ==> (Re)Initializing Finisar test"<<endl;
      cout<<"Test Finisar"<<endl;
      pageid=firstb-1;
      seqn=0;
    }
    pageid++;
    if(pageid>firstb+255) pageid=firstb;
    seqn++;
    if(seqn%5000==0){ cout<<"trial "<<seqn<<endl; }
    if(seqn<200000 && seqn%10000==0){ *radlog<<"trial "<<seqn<<endl; }
    else if(seqn%100000==0){ *radlog<<"trial "<<seqn<<endl; }

    bool rxerr=0;
    bool txerr=0;
    
    // get block of sequential numbers
    get_sequential_block(pageid,fblock,seqn);
    if(DEBUG>20){
      cout<<"DEBUG[RadTestWorker.cpp]  fblock filled :"; dumphex(20,fblock); cout<<"..."<<endl;
    }
    // try sending the RAM page to the board
    SEUcounted=0;
    for(ntry=0; ntry<maxtry; ntry++){ 
      errnum = send_RAMPage(pageid, fblock);
      if(errnum==0){
	break;
      }else if(errnum==99){ // address words don't match up
	*radlog<<time_string()<<" ***ERROR*** (F4eS1a) Address words don't match when sending RAM page "<<pageid<<"  (ntry="<<ntry<<")"<<endl;
      }else if(errnum==98){ // address word and pageid don't match up
	*radlog<<time_string()<<" ***ERROR*** (F4eS1b) Address word doesn't match pageid when sending RAM page "<<pageid<<"  (ntry="<<ntry<<")"<<endl;
      }else{
	*radlog<<time_string()<<" ***ERROR*** (F4eS2) Problem (errnum="<<errnum<<") sending RAM page "<<pageid<<"  (ntry="<<ntry<<")"<<endl;
	cout<<"***Problem ("<<errnum<<") retrieving RAM page "<<pageid<<"  (ntry="<<ntry<<")  ==>  May need to check PC Gbe LEDs and cycle Mezz power!"<<endl;
      }
      if(SEUcounted==0){
	SEUcounted=1;
	counter[4][0]++; // count as Rx SEU
	counterchanged();
      }
      wait_msec(trywait); continue;// try again
    }
    if(ntry==maxtry){ // need reset
      pause();
      if(resetlevel<2){
	counter[4][1]++; // count as Rx reset
	counterchanged();
      }
      needReset(resetlevel);
      resetlevel=2;
      if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  Finisar Test: problem sending RAM page (pageid="<<pageid<<")"<<endl;
      return;  // problem sending, needs reset
    }


    // read the page back from the board
    f4try++;
    rxerr=0;
    txerr=0;
    for(ntry=0; ntry<maxtry; ntry++){
      errnum = retrieve_RAMPage(pageid, block, f3hdr);
      if(errnum==0){
	break;
      }else if(errnum==99){ // address words don't match up
	*radlog<<time_string()<<" ***ERROR*** (F4eR1a) Address words don't match when retrieving RAM page "<<pageid<<"  (ntry="<<ntry<<")"<<endl;
	*radlog<<"    --> PageID   : "<<HEX(pageid&0x00ff)<<" "<<HEX((pageid&0xff00)>>8)<<DEC()<<endl;
	*radlog<<"    --> Firmware : "; dumphex(2,&f3hdr[2],radlog); *radlog<<endl;
	*radlog<<"    --> Block    : "; dumphex(2,block,radlog); *radlog<<endl;
	// could put some logic here to test which address is off
	if(txerr) rxerr=1; // if it doesn't recover, it's an Rx error
	txerr=1;
      }else if(errnum==98){ // address word and pageid don't match up
	*radlog<<time_string()<<" ***ERROR*** (F4eR1b) Address word doesn't match pageid when retrieving RAM page "<<pageid<<"  (ntry="<<ntry<<")"<<endl;
	*radlog<<"    --> PageID   : "<<HEX(pageid&0x00ff)<<" "<<HEX((pageid&0xff00)>>8)<<DEC()<<endl;
	*radlog<<"    --> Firmware : "; dumphex(2,&f3hdr[2],radlog); *radlog<<endl;
	*radlog<<"    --> Block    : "; dumphex(2,block,radlog); *radlog<<endl;
	// could put some logic here to test which address is off
	if(txerr) rxerr=1; // if it doesn't recover, it's an Rx error
	txerr=1;
      }else{
	*radlog<<time_string()<<" ***ERROR*** (F4eR2) Problem (errnum="<<errnum<<") getting RAM page "<<pageid<<"  (ntry="<<ntry<<")"<<endl;
	cout<<"***Problem ("<<errnum<<") retrieving RAM page "<<pageid<<"  (ntry="<<ntry<<")  ==>  May need to check PC Gbe LEDs and cycle Mezz power!"<<endl;
	// could put some logic here to test details of what is in the packet/block
	if(txerr) rxerr=1; // if it doesn't recover, it's and Rx error
	txerr=1;
      }
      wait_msec(trywait); continue;// try again
    }
    if(f4try<2){
      if(rxerr){ counter[4][0]++; counterchanged(); } // count as Rx SEU
      else if(txerr){ counter[5][0]++; counterchanged(); } // count as Tx SEU
    }
    if(ntry==maxtry){ // need to try again
      *radlog<<time_string()<<" ***ERROR*** (F4eR3) Unable to retrieve RAM pageid "<<pageid<<" (f4try="<<f4try<<")"<<endl;
      if(f4try<3){ // don't need a reset yet
	return; // just start the test again
      }
      // okay, we actually need a reset
      pause();
      if(resetlevel<2){
	counter[5][1]++; // count as Tx reset
	counterchanged();
      }
      cout<<"*** Need to check PC Gbe LEDs and power cycle mezzanine board ***"<<endl;
      needReset(resetlevel);
      resetlevel=2;
      if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  Finisar Test: problem retrieving RAM page (pageid="<<pageid<<")"<<endl;
      return;  // problem sending, needs reset
    }
    f4try=0; // made it
    txerr=0;
    rxerr=0;
    
    if(DEBUG>5){ 
      *radlog<<"DEBUG: Read F4 packet with first four bytes : "; dumphex(4,f3hdr,radlog); *radlog<<endl;
      *radlog<<"DEBUG: And then the next 20 bytes are : "; dumphex(20,block,radlog); *radlog<<endl;
    }
    
    // Check f3hdr // now take care of in command.cpp
    //char command[4];
    //command[0]=0xf3;
    //command[1]=0xf3;
    //command[2]=pageid&0x00ff;
    //command[3]=((pageid+53248)&0xff00)>>8; // return packet code has 0xd000 (53248) added to the pageid
    //if( memcmp(f3hdr, command, 4) ){
    //  *radlog<<time_string()<<" ***ERROR*** (F4eH1) Incorrect packet code from reading RAM pageid "<<pageid<<endl;
    //  *radlog<<"    --> Expected : "; dumphex(4,command,radlog); *radlog<<endl;
    //  *radlog<<"    --> Actual   : "; dumphex(4,f3hdr,radlog); *radlog<<endl;
    //  txerr=1;
    //}
    
    // check RAM page against sent block
    // if there is a mismatch, then we need to do a second read to see if it was a read or write problem
    if( memcmp(block, fblock, RAMPAGE_SIZE) ){
      
      *radlog<<time_string()<<" ***ERROR*** (F4eB1) Error in RAM pageid "<<pageid<<endl;
      for(int i=0; i<RAMPAGE_SIZE; i++){
	if(fblock[i] != block[i]){
	  int nwindow=0; // log nwindow bytes before and after bad byte(s)
	  *radlog<<"    --> byte: FPGA\tFile"<<endl;
	  for(int j=i-nwindow; j<=i+nwindow && j<RAMPAGE_SIZE; j++){
	    if(j<0) continue;
	    *radlog<<"          "<<j<<":  "<<HEX(block[j]&0xff)<<"  \t"<<HEX(fblock[j]&0xff)<<std::dec<<endl;
	  }
	  *radlog<<DEC(); // back to default formatting
	}
      }
      
      // do another read
      errnum = retrieve_RAMPage(pageid, block2, f3hdr);
      if(errnum){
	*radlog<<time_string()<<" ***ERROR*** (FeB2) Problem (errnum="<<errnum<<") with second read of RAM page "<<pageid<<endl;
	// what to do?
      }
      
      if( !memcmp(block2, fblock, RAMPAGE_SIZE) ){
	// second read matches what was originally sent => we had a read problem last time
	*radlog<<time_string()<<"    --> (F4e1) Second read matches original sequence => Tx error"<<endl;
	txerr=1;
      }else if( !memcmp(block2, block, RAMPAGE_SIZE) ){
	// second read matches what was read last time => we had a write problem originally
	*radlog<<time_string()<<"    --> (F4e2) Second read matches first read => Rx error"<<endl;
	rxerr=1;
      }else{
	// second read doesn't match last read or original write!?!
	*radlog<<time_string()<<"    --> (F4e3) Second read does not match original sequence or first read => cout as Tx error"<<endl;
	txerr=1;
      }
    }
    
    if(rxerr){ counter[4][0]++; counterchanged();}
    if(txerr){ counter[5][0]++; counterchanged();}
    
    //////////////////////////////////////////////////
    
    
  }else if(currenttest==5){
    ////////////////// Triple-Voting Test ///////////////////
    
    if(reinit[currenttest]){
      reinit[currenttest]=0;
      retrieve_VotingErrorCounts(voting_counts_last); // get initial counters after (re)start
      *radlog<<time_string()<<" ==> (Re)Initializing voting counters: "<<endl;
      for(int i=0; i<12; i++)  *radlog<<"    --> voting_counts_last["<<i<<"] =  "<<voting_counts_last[i]<<endl;
    }
    
    cout<<"Test Voting Error Counts"<<endl;
    
    SEUcounted=0;
    for(ntry=0; ntry<maxtry; ntry++){
      errnum = retrieve_VotingErrorCounts(voting_counts);
      if(errnum){
	*radlog<<time_string()<<" ***ERROR*** (F5e1) Problem (errnum="<<errnum<<") getting voting error counts. (ntry="<<ntry<<")"<<endl;
	//// Note: this is not counted as a SEU.  If this is never successful it will trigger a reset without counting as a SEU
	wait_msec(trywait); continue;// try again
      }

      if(DEBUG>5){
	cout<<"DEBUG[RadTestWorder.cpp]  F5 error counters:"<<endl;
	for(int i=0; i<=11; i++) cout<<"  voting_counts["<<i<<"] =  "<<voting_counts[i]<<endl;
      }
      //// Some logic for which counts are SEUs and which are not
      //   counters 0-5 :  Not SEUs.  Pre-mitigation counters, just log when they change and if they are runaway
      //   counters 6-8 :  Are SEUs.  Error counts after triple voting.  Count as SEU and as runaway if presistant for three reads.
      //   counters 9-11:  Not SEUs.  Triple voting cycles counters.  Log when they do *not* change.

      // check pre-mitigation counters
      change=0;
      for(int i=0; i<=5; i++) change += voting_counts[i] - voting_counts_last[i];
      if(change){ // they did change...
	// dump counters to the log file
	*radlog<<time_string()<<" ***WARNING*** Change in pre-voting error counters (ntry="<<ntry<<"): "<<endl;
	for(int i=0; i<=5; i++)  *radlog<<"    --> voting_counts["<<i<<"] =  "<<voting_counts[i]<<endl;
	memcpy(voting_counts_last, voting_counts, 6*sizeof(int)); // update last counts
      }	

      // check triple vote cycles counters
      change=0;
      for(int i=9; i<=11; i++) change += voting_counts[i] - voting_counts_last[i];
      if(change>0){  // voting cycle counters increased, as they should
	if(DEBUG>10) cout<<"DEBUG[RadTestWorker.cpp] Voting cycle counters increased (good)"<<endl;
	memcpy((void*)&voting_counts_last[9], (const void*)&voting_counts[9], 3*sizeof(int)); // update last counts
      }else{ // they did *not* increase, print warning...
	// dump counters to the log file
	if(DEBUG>10) cout<<"DEBUG[RadTestWorker.cpp] Voting cycle counters did *not* increase"<<endl;
	*radlog<<time_string()<<" ***WARNING*** Voting cycles counters did not increase (ntry="<<ntry<<"): "<<endl;
	for(int i=9; i<=11; i++)  *radlog<<"    --> voting_counts["<<i<<"] =  "<<voting_counts[i]<<endl;
      	if(change<0) { // they decreased!
	  memcpy((void*)&voting_counts_last[9], (const void*)&voting_counts[9], 3*sizeof(int)); // update last counts
	}
      }
      if(DEBUG>20) for(int i=9; i<=11; i++)  cout<<"    --> voting_counts["<<i<<"] =  "<<voting_counts[i]<<endl;
      
      // check if triple voting counters changed, which will be an SEU 
      // Do this check last so that the voting_counters_last will be updated for the other counters, otherwise they will not be udpated if there is an SEU
      change=0;
      for(int i=6; i<=8; i++) change += voting_counts[i] - voting_counts_last[i];
      if(change){ // they did change...
	// log message that this was an SEU (ERROR)
	*radlog<<time_string()<<" ***ERROR*** (F5e2) Change in voting error counters (ntry="<<ntry<<"): "<<endl;
	for(int i=6; i<=8; i++)  *radlog<<"    --> voting_counts["<<i<<"] =  "<<voting_counts[i]<<endl;
	memcpy((void*)&voting_counts_last[6], (const void*)&voting_counts[6], 3*sizeof(int)); // update last counts
	
	if(SEUcounted==0){ // only count as 1 SEU/burst
	  SEUcounted=1;
	  counter[6][0]++;
	  counterchanged();
	}
	
	wait_msec(trywait); continue;// try again
      }

      // we didn't have any SEUs this try, we can move on
      break;
    }
    
    if(ntry==maxtry){ // need reset
      pause();
      if(resetlevel<2){
	counter[6][1]++;
	counterchanged();
      }
      needReset(resetlevel);
      resetlevel=2;
      if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  F5 need reset"<<endl;
      return;
    }
    //////////////////////////////////////////////////


  }else{
    cout<<"Unknown command."<<endl;
  }
  
  // close device
  eth_close();
  resetlevel=1;
  
  if(currenttest==4){
    wait_msec(0); // just a quick listen for signals, then run test 4 again
  }else{
    currenttest = (currenttest+1)%NTESTS; // increment currenttest number
    wait_msec(testwait); // short pause between tests
  }
}


bool RadTestWorker::notests(){
  bool something=0;
  for(int i=0; i<NTESTS; i++){
    if(dotest[i]){
      something=1;
      break;
    }
  }
  return !something;
}

void RadTestWorker::clear(){
  for(int i=0; i<NTESTS; i++){
    counter[i][0]=0;
    counter[i][1]=0;
  }
}

int RadTestWorker::check_RAMpage(int pageid){
  //// put this in a separate function because it has to be done for block RAM and clb RAM

  SEUcounted=0;
  for(ntry=0; ntry<maxtry; ntry++){
    errnum = retrieve_RAMPage(pageid, block, f3hdr);
    if(errnum){
      *radlog<<time_string()<<" ***ERROR*** (cRe1) Problem (errnum="<<errnum<<") getting RAM page "<<pageid<<"  (ntry="<<ntry<<")"<<endl;
      *radlog<<"--> Read F3 packet with first four bytes : "; dumphex(4,f3hdr,radlog); *radlog<<endl;
      *radlog<<"--> And then the next 20 bytes are : "; dumphex(20,block,radlog); *radlog<<endl;
      //// Note: this is not counted as a SEU.  If this is never successful it will trigger a reset without counting as a SEU
      wait_msec(trywait); continue;// try again
    }
    
    if(DEBUG>5){ 
      *radlog<<"DEBUG: Read F3 packet with first four bytes : "; dumphex(4,f3hdr,radlog); *radlog<<endl;
      *radlog<<"DEBUG: And then the next 20 bytes are : "; dumphex(20,block,radlog); *radlog<<endl;
    }

    // check RAM page against file
    if( get_page_from_file(pageid,fblock)!=0 ){
      // make file for next time
      //memcpy(fblock, block, RAMPAGE_SIZE);
      //save_page_to_file(pageid, fblock);
      if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  check_RAMpage return 1  <== problem getting page from file"<<endl;
      return 1;
    }
    if( memcmp(block, fblock, RAMPAGE_SIZE) ){
    //if( memcmp(block, fblock, RAMPAGE_SIZE-2) ){ // don't check last two bytes
      *radlog<<time_string()<<" ***ERROR*** (cRe2) Error in RAM pageid "<<pageid<<" (ntry="<<ntry<<")"<<endl;
      for(int i=0; i<RAMPAGE_SIZE; i++){
	if(fblock[i] != block[i]){
	  int nwindow=0; // log nwindow bytes before and after bad byte(s)
	  *radlog<<"    --> byte: FPGA\tFile"<<endl;
	  for(int j=i-nwindow; j<=i+nwindow && j<RAMPAGE_SIZE; j++){
	    if(j<0) continue;
	    *radlog<<"          "<<j<<":  "<<HEX(block[j]&0xff)<<"  \t"<<HEX(fblock[j]&0xff)<<std::dec<<endl;
	  }
	  *radlog<<DEC(); // back to default formatting
	}
      }
      if(SEUcounted==0){ // only count as 1 SEU/burst
	SEUcounted=1;
	counter[3][0]++;
	counterchanged();
      }
      wait_msec(trywait); continue;// try again
    }
    break;
  }
  
  if(ntry==maxtry){ // need reset
    pause();
    if(resetlevel<2){
      counter[3][1]++;
      counterchanged();
    }
    needReset(resetlevel);
    resetlevel=2;
    if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  check_RAMpage return 2  <== need reset"<<endl;
    return 2;
  }
  if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  check_RAMpage return 0"<<endl;
  return 0;
}

int RadTestWorker::write_RAMpage(int pageid){
  
  // get RAM page data from file
  if( get_page_from_file(pageid,fblock)!=0 ){
    cout<<"***ERROR*** (wRe1) Could not find file with data pageid "<<pageid<<endl;
    if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  write_RAMpage return 1  <== problem getting page from file"<<endl;
    return 1;
  }
  
  // try sending the RAM page to the board
  for(ntry=0; ntry<maxtry; ntry++){ 
    errnum = send_RAMPage(pageid, fblock);
    if(errnum){
      *radlog<<time_string()<<" ***ERROR*** (wRe2) Problem (errnum="<<errnum<<") sending RAM page "<<pageid<<"  (ntry="<<ntry<<")"<<endl;
      //// Note: this is not counted as a SEU.  If this is never successful it will trigger a reset without counting as a SEU
      wait_msec(trywait); continue;// try again
    }
    break;
  }
  if(ntry==maxtry){ // need reset
    pause();
    if(resetlevel<2){
      counter[7][1]++;
      counterchanged();
    }
    needReset(resetlevel);
    resetlevel=2;
    if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  write_RAMpage return 3  <== problem sending RAM page (pageid="<<pageid<<")"<<endl;
    return 3;
  }

  // read the page back from the board to make sure it all worked
  SEUcounted=0;
  for(ntry=0; ntry<maxtry; ntry++){
    errnum = retrieve_RAMPage(pageid, block, f3hdr);
    if(errnum){
      *radlog<<time_string()<<" ***ERROR*** (wRe3) Problem (errnum="<<errnum<<") getting RAM page "<<pageid<<"  (ntry="<<ntry<<")"<<endl;
      *radlog<<"--> Read F3 packet with first four bytes : "; dumphex(4,f3hdr,radlog); *radlog<<endl;
      *radlog<<"--> And then the next 20 bytes are : "; dumphex(20,block,radlog); *radlog<<endl;
      //// Note: this is not counted as a SEU.  If this is never successful it will trigger a reset without counting as a SEU
      wait_msec(trywait); continue;// try again
    }
    
    if(DEBUG>5){ 
      *radlog<<"DEBUG: Read F3 packet with first four bytes : "; dumphex(4,f3hdr,radlog); *radlog<<endl;
      *radlog<<"DEBUG: And then the next 20 bytes are : "; dumphex(20,block,radlog); *radlog<<endl;
    }

    // check RAM page against file
    // fblock already loaded above
    if( memcmp(block, fblock, RAMPAGE_SIZE) ){
    //if( memcmp(block, fblock, RAMPAGE_SIZE-2) ){ // don't check last two bytes
      *radlog<<time_string()<<" ***ERROR*** (wRe4) Error in RAM pageid "<<pageid<<" (ntry="<<ntry<<")"<<endl;
      for(int i=0; i<RAMPAGE_SIZE; i++){
      	if(fblock[i] != block[i]){
	  int nwindow=0; // log nwindow bytes before and after bad byte(s)
	  *radlog<<"    --> byte: FPGA\tFile"<<endl;
	  for(int j=i-nwindow; j<=i+nwindow && j<RAMPAGE_SIZE; j++){
	    if(j<0) continue;
	    *radlog<<"          "<<j<<":  "<<HEX(block[j]&0xff)<<"  \t"<<HEX(fblock[j]&0xff)<<std::dec<<endl;
	  }
	  *radlog<<DEC(); // back to default formatting
	}
      }
      if(SEUcounted==0){ // only count as 1 SEU/burst
	SEUcounted=1;
	counter[7][0]++;
	counterchanged();
      }
      wait_msec(trywait); continue;// try again
    }
    break;
  }
  
  if(ntry==maxtry){ // need reset
    pause();
    if(resetlevel<2){
      counter[7][1]++;
      counterchanged();
    }
    needReset(resetlevel);
    resetlevel=2;
    if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  write_RAMpage return 2  <== need reset, persistent mismatch between sent and read page (pageid="<<pageid<<")"<<endl;
    return 2;
  }
  if(DEBUG>5) cout<<"DEBUG[RadTestWorker.cpp]  write_RAMpage return 0"<<endl;
  return 0;
}

void RadTestWorker::wait_msec(int msec){
  QTimer tmr;
  tmr.setSingleShot(1);
  QEventLoop lp;
  connect(&tmr,SIGNAL(timeout()),&lp,SLOT(quit()));
  tmr.start(msec);
  lp.exec();
}
