#include "RadTestGUI.h"


//// executable main ////
int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    RadTestGUI gui;
    gui.show();
    return app.exec();
}

RadTestGUI::~RadTestGUI()
{
  // close log file
  close_logfile();  
}


RadTestGUI::RadTestGUI(QWidget *parent)
  : QWidget(parent), 
    popupText(0),
    radlog(0)
{
  radlog = &cout; // start with *radlog going to cout
  if(DEBUG) cout<<"DEBUG="<<DEBUG<<endl;

  // starting position
  this->move(100,100);


  //// Worker Object ////
  _worker = new RadTestWorker();
  connect(_worker, SIGNAL(counterchanged()), this, SLOT(updatenumbers()));
  connect(_worker, SIGNAL(needReset(int)), this, SLOT(resetRequested(int)));
  

  //// GUI Layout ////

  //// Top labels ////
  topLayout = new QHBoxLayout;
  QLabel *filler = new QLabel("");
  progressIndicator = new QProgressIndicator();
  QLabel *title[2];
  title[0] = new QLabel("SEU Count:");
  title[1] = new QLabel("Runaway Count:");
  for(int i=0; i<2; i++){
    title[i]->setAlignment(Qt::AlignLeft);
    title[i]->setFixedSize(100,20);
    QFont font = title[i]->font();
    font.setPointSize(font.pointSize() - 1);
    title[i]->setFont(font);
  }
  topLayout->addWidget(progressIndicator,0);
  topLayout->addWidget(filler,1);
  topLayout->addWidget(title[0],2);
  topLayout->addWidget(title[1],3);

  //// Function bars ////
  functionbar[0] = new FunctionBar("Not Used");
  functionbar[1] = new FunctionBar("F1: Snap12 Links, FPGA logic/IO");
  functionbar[2] = new FunctionBar("F2: Level-shifting Translator Bits, FPGA logic/IO");
  functionbar[3] = new FunctionBar("F3: Block RAM, FPGA logic/IO");
  functionbar[4] = new FunctionBar("F4a: Finisar - Rx SEU / Cannot-Write Reset");
  functionbar[5] = new FunctionBar("F4b: Finisar - Tx SEU / Cannot-Retrieve Reset");
  functionbar[6] = new FunctionBar("F5: Triple Voting, SEU Mitigation");
  functionbar[7] = new FunctionBar("F7: Resend BRAMs");

  connect(functionbar[0]->checkBox, SIGNAL(toggled(bool)), this, SLOT(turnOnF0(bool)));
  connect(functionbar[1]->checkBox, SIGNAL(toggled(bool)), this, SLOT(turnOnF1(bool)));
  connect(functionbar[2]->checkBox, SIGNAL(toggled(bool)), this, SLOT(turnOnF2(bool)));
  connect(functionbar[3]->checkBox, SIGNAL(toggled(bool)), this, SLOT(turnOnF3(bool)));
  connect(functionbar[4]->checkBox, SIGNAL(toggled(bool)), this, SLOT(turnOnF4a(bool)));
  connect(functionbar[5]->checkBox, SIGNAL(toggled(bool)), this, SLOT(turnOnF4b(bool)));
  connect(functionbar[6]->checkBox, SIGNAL(toggled(bool)), this, SLOT(turnOnF5(bool)));
  connect(functionbar[7]->checkBox, SIGNAL(toggled(bool)), this, SLOT(turnOnF7(bool)));
  
  connect(functionbar[0]->checkBox, SIGNAL(toggled(bool)), _worker, SLOT(turnOnF0(bool)));
  connect(functionbar[1]->checkBox, SIGNAL(toggled(bool)), _worker, SLOT(turnOnF1(bool)));
  connect(functionbar[2]->checkBox, SIGNAL(toggled(bool)), _worker, SLOT(turnOnF2(bool)));
  connect(functionbar[3]->checkBox, SIGNAL(toggled(bool)), _worker, SLOT(turnOnF3(bool)));
  connect(functionbar[4]->checkBox, SIGNAL(toggled(bool)), _worker, SLOT(turnOnF4a(bool)));
  connect(functionbar[5]->checkBox, SIGNAL(toggled(bool)), _worker, SLOT(turnOnF4b(bool)));
  connect(functionbar[6]->checkBox, SIGNAL(toggled(bool)), _worker, SLOT(turnOnF5(bool)));
  connect(functionbar[7]->checkBox, SIGNAL(toggled(bool)), _worker, SLOT(turnOnF7(bool)));
  
  connect(_worker, SIGNAL(f7_needed(bool)), functionbar[7]->checkBox, SIGNAL(toggled(bool))); //let the worker toggle F7 off once it's been done
  
  
  //// Run Control ////
  controlsLayout = new QGridLayout;

  startButton = new QPushButton(tr("Start"));
  connect(startButton, SIGNAL(clicked()), this, SLOT(start()));
  
  pauseButton = new QPushButton(tr("Single Step"));
  connect(pauseButton, SIGNAL(clicked()), this, SLOT(pausestep()));
  
  stopButton = new QPushButton(tr("Stop"));
  connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
  connect(stopButton, SIGNAL(clicked()), progressIndicator, SLOT(stopAnimation()));
  
  quitButton = new QPushButton(tr("Quit"));
  connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
  
  controlsLayout->addWidget(startButton, 0,0);
  controlsLayout->addWidget(pauseButton, 0,1);
  controlsLayout->addWidget(stopButton,  0,2);
  controlsLayout->addWidget(quitButton,  1,2);


  //// comment area ////
  commentLayout = new QHBoxLayout;

  textEdit = new QPlainTextEdit;
  textEdit->setFixedHeight(80);

  timeStampCommentButton = new QPushButton(tr("Add\ncomment\nto logfile"));
  connect(timeStampCommentButton, SIGNAL(clicked()), this, SLOT(insertComment()));
  
  commentLayout->addWidget(timeStampCommentButton, 0);
  commentLayout->addWidget(textEdit, 1);

  
  //// Status display ////
  statusLayout = new QHBoxLayout;

  statusbar = new QLineEdit(time_QString()+tr(": "));
  statusbar->setReadOnly(1);
  statusbar->setAlignment(Qt::AlignLeft);
  QFont font = statusbar->font();
  font.setPointSize(font.pointSize());
  statusbar->setFont(font);
  
  statusLayout->addWidget(statusbar);

  
  //// Main Layout ////
  mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout,1);
  for(int i=1; i<NTESTS; i++) mainLayout->addLayout(functionbar[i]->layout,2);
  mainLayout->addLayout(controlsLayout,1);
  mainLayout->addLayout(commentLayout,1);
  mainLayout->addLayout(statusLayout,0);
  
  setLayout(mainLayout);
  
  setWindowTitle(tr("FPGA Radiation Tests"));

  state=STOPPED;
  this->stop(); // initialize to stopped state

}



void RadTestGUI::start(){
  if (state==RUNNING) return;

  open_logfile();
  *radlog<<time_string()<<" ==> Run tests:";
  statusbar->setText(time_QString()+tr(": Starting..."));
  
  // check that there is something to run
  bool something = 0;
  for(int i=1; i<NTESTS; i++){
    if(functionbar[i]->checkBox->isChecked()){
      *radlog<<"  test"<<i;
      something=1;
    }
  }
  if(!something){
    *radlog<<" ... no tests selected."<<endl;
    statusbar->setText(time_QString()+tr(": Nothing to run."));
    return;
  }
  *radlog<<endl;

  progressIndicator->startAnimation();
  progressIndicator->setAnimationDelay(70);

  if(state==PAUSED) *radlog<<time_string()<<" ==> Resume  ----------------------------------------"<<endl;
  else *radlog<<time_string()             <<" ==> Start  ========================================="<<endl;

  updatenumbers();

  // enable/disable buttons
  disableAllFunctions(1);  // disable toggling tests
  startButton->setDisabled(1); // disable start
  pauseButton->setDisabled(0); // enable pause
  pauseButton->setText(tr("Pause")); // enable pause
  stopButton->setDisabled(0);  // enable stop
  quitButton->setDisabled(1);  // disable quit

  // start the tests
  _worker->start();

  statusbar->setText(time_QString()+tr(": Running"));
  state=RUNNING;
  cout<<"End of start()"<<endl;
}

void RadTestGUI::pausestep(){

  if(state!=RUNNING && state!=RESET){ // step 
    *radlog<<time_string()<<" ==> Step"<<endl;
    statusbar->setText(time_QString()+tr(": Singel step..."));
    _worker->step();
    return;
  }

  *radlog<<time_string()<<" ==> Paused"<<endl;
  statusbar->setText(time_QString()+tr(": Pausing..."));
    
  // pause the tests
  _worker->pause();

  // enable/disable buttons
  disableAllFunctions(1);  // disable toggling tests
  startButton->setDisabled(0); // enable start
  startButton->setText(tr("Resume"));
  pauseButton->setText(tr("Single Step")); // change to Step button
  stopButton->setDisabled(0);  // enable stop
  quitButton->setDisabled(1);  // disable quit

  //updatenumbers();
  state=PAUSED;
  statusbar->setText(time_QString()+tr(": Paused"));
}

void RadTestGUI::resetRequested(int level){
  state=RESET;
  this->pausestep(); // immediately pause

  if(level<=1) *radlog<<time_string()<<" ==> Runaway Errors"<<endl;
  else         *radlog<<time_string()<<" ==> Runaway Errors Still Present!"<<endl;
  

  //updatenumbers();

  // popup window with time stamp
  if(popupText) delete popupText; // clean up any previous popup
  popupText = new MyQTextEdit();
  if(level<=1) popupText->setHtml("<body bgcolor=yellow> <center> <font size=+4> <b> <br>"+tr("Run paused!  Try resuming.")+" <br> </b> <font size=+1> Try to just resume the test. </center> </body>");
  else if(functionbar[4]->checkBox->isChecked()) popupText->setHtml("<body bgcolor=red> <center> <font size=+4> <b> <br>"+tr("Errors still present! Do a hard reset!")+" <br> </b> <font size=+1> Power cycle the mezzanine card. </center> </body>");
  else         popupText->setHtml("<body bgcolor=red> <center> <font size=+4> <b> <br>"+tr("Errors still present! Do a hard reset!")+" <br> </b> <font size=+1> Reset the FPGA and then resume the test. </center> </body>");
  popupText->setReadOnly(1);
  popupText->move(this->x()-20, this->y()+40);
  popupText->resize(700,250);
  popupText->show();

  if(level<=1) statusbar->setText(time_QString()+tr(": Paused due to runaway errors, try resuming."));
  else         statusbar->setText(time_QString()+tr(": Runaway errors still present, do a hard reset."));
}

void RadTestGUI::insertComment(){
  open_logfile();
  *radlog<<time_string()<<" ==> User comment"<<endl;

  QTextBlock firstBlock = textEdit->document()->begin();
  QTextBlock currentBlock = firstBlock;
  while (currentBlock.isValid()) {
    *radlog<<"# "<<currentBlock.text().toStdString()<<endl;
    currentBlock = currentBlock.next();
  }
}

void RadTestGUI::updatenumbers(){
  for(int i=0; i<NTESTS; i++){
    for(int j=0; j<2; j++){
      functionbar[i]->display[j]->setText(QString::number(_worker->counter[i][j]));
    }
  }
  statusbar->setText(time_QString()+tr(": Updated"));
  *radlog<<time_string()<<" ==> Counters updated:"<<endl;
  // actually print the counters to the log file
  for(int i=1; i<NTESTS; i++){
    if(functionbar[i]->checkBox->isChecked() || functionbar[i]->checkBox->isEnabled()){
      *radlog<<left<<setw(50)<<functionbar[i]->name.toStdString()<<": ";
      *radlog<<right<<setw(6)<<functionbar[i]->display[0]->text().toStdString()<<"  ";
      *radlog<<right<<setw(6)<<functionbar[i]->display[1]->text().toStdString()<<endl;
    }
  }
}

void RadTestGUI::stop(){
  
  // final update
  updatenumbers();

  // stop the tests
  _worker->stop();

  *radlog<<time_string()<<" ==> Stopped"<<endl;

  // close log file and revert back to cout
  if(state!=STOPPED) close_logfile();

  // enable/disable buttons
  disableAllFunctions(0);      // enable toggling tests
  startButton->setDisabled(0); // enable start
  startButton->setText(tr("Start"));
  pauseButton->setText(tr("Single Step")); // Change Pause to Single Step
  stopButton->setDisabled(1);  // disable stop
  quitButton->setDisabled(0);  // enable quit
  
  statusbar->setText(time_QString()+tr(": Stopped."));
  state=STOPPED;
}


void RadTestGUI::disableAllFunctions(bool e){
  for(int i=0; i<NTESTS; i++){
    functionbar[i]->checkBox->setDisabled(e);
  }
  if(functionbar[3]->checkBox->isChecked()) functionbar[7]->checkBox->setDisabled(0); // keep f7 enabled when f3 is being used
}

void RadTestGUI::turnOnBar(int i, bool e){
  //// F4a/F4b are locked together
  if(i==4 || i==5){
    functionbar[4]->checkBox->setChecked(e);
    functionbar[4]->display[0]->setDisabled(!e);
    functionbar[4]->display[1]->setDisabled(!e);
    functionbar[5]->checkBox->setChecked(e);
    functionbar[5]->display[0]->setDisabled(!e);
    functionbar[5]->display[1]->setDisabled(!e);
    if(e){ // make sure F1, F2, F3, F5, F7 are off
      for(int j=1; j<4; j++){
	turnOnBar(j,0);
      }
      turnOnBar(6,0);
      turnOnBar(7,0);
    }
  }else{
    functionbar[i]->checkBox->setChecked(e);
    functionbar[i]->display[0]->setDisabled(!e);
    functionbar[i]->display[1]->setDisabled(!e);
    if(e){ // make sure F4 is off
      turnOnBar(4,0); // turns off F4a and F4b
    }
  }
}


QString RadTestGUI::time_QString(){
  return QString(time_string().c_str());
}

void RadTestGUI::open_logfile(){
  if(!lout.is_open()){
    string newlogname = "logs/"+newLogName();
    lout.open(newlogname.c_str());
    if(!lout.is_open()){
      cout<<"ERROR: Could not open log file! "<<newlogname<<endl;
      radlog = &cout;
    }else{
      radlog=&lout;
    }
    _worker->setlog(radlog);
  }
}

void RadTestGUI::close_logfile(){
  // print final summary
  *radlog<<time_string()<<" ==> FINAL SUMMARY:"<<endl;
  *radlog<<" on? "<<left<<setw(50)<<"Test"<<": ";
  *radlog<<right<<setw(6)<<"#SEU"<<"  ";
  *radlog<<right<<setw(6)<<"#Runaways"<<endl;
  for(int i=1; i<NTESTS; i++){
    if(functionbar[i]->checkBox->isChecked()) *radlog<<"  *  ";
    else *radlog<<"     ";
    *radlog<<left<<setw(50)<<functionbar[i]->name.toStdString()<<": ";
    *radlog<<right<<setw(6)<<functionbar[i]->display[0]->text().toStdString()<<"  ";
    *radlog<<right<<setw(6)<<functionbar[i]->display[1]->text().toStdString()<<endl;
  }
  if(lout.is_open()){ 
    lout.close(); // close logfile
  }
  radlog = &cout;
  _worker->setlog(radlog);
}

string RadTestGUI::newLogName(){
  string logname = "RadTest_" + time_string(1) + ".log";
  return logname;
}

