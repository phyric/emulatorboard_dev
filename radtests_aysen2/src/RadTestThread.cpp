#include "RadTestThread.h"

//void RadTestThread::run(){
void RadTestThread::testmeth(){
  
  cout<<"RadTestThread::run()..."<<endl;
  cout<<"5"<<endl;
  sleep(1); // 5 sec
  cout<<"4"<<endl;
  sleep(1); // 5 sec
  cout<<"3"<<endl;
  sleep(1); // 5 sec
  cout<<"2"<<endl;
  sleep(1); // 5 sec
  cout<<"1"<<endl;
  sleep(1); // 5 sec
  cout<<"0"<<endl;
  
  //exec();
  
  this->finished();
}
