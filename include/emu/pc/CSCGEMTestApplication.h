#ifndef _Emu_PC_CSCGEMTestApplication_h_
#define _Emu_PC_CSCGEMTestApplication_h_

#include "xdaq/WebApplication.h"
#include "emu/pc/TMB.h"
#include "GEMPatternGen.h"
#include "PatternGen.h"
//#include "CSCConstants.h"
#include "pattern_convert.h"
#include "Yuriy_CLCT_PatternGen.h"


//
#include "emu/pc/EmuPeripheralCrateBase.h"
#include "emu/pc/ChamberUtilities.h"
#include "emu/pc/EmuPeripheralCrateConfig.h"


namespace emu { namespace pc {

/** \class CSCGEMTestApplication
 * main web GUI Application class for the CSC-GEM test stand
 */
class CSCGEMTestApplication: public EmuPeripheralCrateBase
{
protected:
  //parsing section
  xdata::String XML_or_DB_;
  xdata::String xmlFile_;
  int parsed;
  std::string Valid_config_ID;
  int xml_or_db;
  std::vector<Crate*> crateVector;
  unsigned int total_crates_;
  int this_crate_no_;
  xdata::String standalone;
  bool standalone_;
  std::vector<DAQMB*> dmbVector;
  std::ostringstream OutputDMBTests[10][30];
  int current_crate_;
  std::ostringstream OutputTMBTests[10][30];
  Crate *thisCrate;
  std::ostringstream ChamberTestsOutput[10][30];
  std::ostringstream OutputCCBTests[30];
  std::string ThisCrateID_;
  CCB* thisCCB ;
  MPC * thisMPC;
  std::vector<Chamber*> chamberVector;
  //std::vector<DDU*> dduVector;
  //std::vector<TMBTester>   tmbTestVector;
  ChamberUtilities MyTest[10][30];
  int able_to_load_alct[9];
  EmuPeripheralCrateConfig * EPCConfig;
  int TMB_, DMB_,RAT_;

  char CFEBtxtMounNchar[6];
  int  CFEBtxtMounNint;
  bool StartCFEBtxt;
  char CFEBtxtfileName[100];
  char CFEBtxtfileDir[200];
  char CFEBtxtprefix[60];
  char BxArray[512][5];
  char KeyStripArray[512][5];
  char PatternNumArray[512][5];
  char NHitsArray[512][5];

  char GemtxtMounNchar[6];
  int  GemtxtMounNint;
  bool startgemtxt;

  char DirGen[200];
  char DirGenPre[200];
  char DirYuriyConvtxt[200];
  char DirYuriyCLCTConvPat[200];
  char DirGEMPat[200];
  char DirCfebPat[200];
  char DirYuriyCLCTConvtxt[200];
  char DirGEMtxt[200];
  char DirCfebtxt[200];
  char DirUp[200];
  char YuriyConvtxtFile[200];
  char YuriyCLCTConvtxtFile[200];
  char GEMtxtFile[200];
  char CfebtxtFile[200];
  //char YuriyConvPatFile[200];
  char CfebPatFile[200];
  char DCFEBN[10];
  char HalfStripN[10];
  char FiberN[10];
  char PatFile[200];
  char PatFilePre[200];
  char NumRuns[20];
  char TwoMLogFileDir[200];
  char TwoMLogFilePrefix[100];
  char NumRuns2[20];
  char testingarray[512][5];
  char FileNameFiberN[8][400];

  char Fiber0PatName[200];
  char Fiber1PatName[200];
  char Fiber2PatName[200];
  char Fiber3PatName[200];
  char Fiber4PatName[200];
  char Fiber5PatName[200];
  char Fiber6PatName[200];
  char Fiber7PatName[200];

  bool ConvertBool;
  bool PatLoadError;
  bool PatPreLoadError[7];
  bool FileLoadedToBoard; //make this an array of 8 later
  bool FilePreLoadedToBoard;
  std::vector<TMB*>   tmbVector;


public:

  //XDAQ_INSTANTIATOR();
  static xdaq::Application* instantiate(xdaq::ApplicationStub* s) throw (xdaq::exception::Exception);

  /// constructor
  CSCGEMTestApplication(xdaq::ApplicationStub * s);

  /// home-page of the application
  void Default(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);


private:

  //the location of the emulator board connection to the computer
  char* EmuBoardLocation;

  //Functions needed for parsing
  void SetCurrentCrate(int crate);

  //Parser
  bool ParsingXML();

  // Generate DCFEB data
  void GenerateDCFEBData(xgi::Input * in, xgi::Output * out );

  // Upload generated DCFEB data on emulator board's RAMs
  void UploadDCFEBData(xgi::Input * in, xgi::Output * out );

  // Check generated DCFEB data on emulator board's RAMs
  void CheckDCFEBData(xgi::Input * in, xgi::Output * out );

  // Initiate data transmission from the emulator board to the OTMB
  void TransmitDCFEBData(xgi::Input * in, xgi::Output * out );

  // Readout trigger results from OTMB
  void ReadOutTriggerResults(xgi::Input * in, xgi::Output * out );

  // Readout trigger results from OTMB
  void F0(xgi::Input * in, xgi::Output * out );

  //gets the desired number of mouns to go into the gem text file,
  //and enables the input for the CFEB text file
  void GenCFEBPattxtFileMounN(xgi::Input * in, xgi::Output * out );

  //Creates a Text File of the CFEB Pattern
  void GenCFEBPattxtFile(xgi::Input * in, xgi::Output * out );

  //gets the desired number of mouns to go into the gem text file,
  //and enables the input for the gem text file
  void GenGEMPattxtFileMounN(xgi::Input * in, xgi::Output * out );

  //Creates a Text File of the GEM Pattern
  void GenGEMPattxtFile(xgi::Input * in, xgi::Output * out );

  //toggles whether or not the Convert buttons are displayed
  void ConvertBoolToggle(xgi::Input * in, xgi::Output * out );

  void YuriyPatConvert(xgi::Input * in, xgi::Output * out );

  //Converts Tao's GEM .txt files to GEM .pat files
  void GemPatternConverter(xgi::Input * in, xgi::Output * out );

  void Yuriy_CLCT_PatternConverter(xgi::Input * in, xgi::Output * out );

  void CfebPatternConverter(xgi::Input * in, xgi::Output * out );

  //Test button for loading the file to the emulator board
  void LoadToEmuBoard(xgi::Input * in, xgi::Output * out );

  //Test button for loading the file to the emulator board
  void LoadCfebPatPrefix(xgi::Input * in, xgi::Output * out );

  //Test button for sending the file from the emulator board
  void SendFromEmuBoard(xgi::Input * in, xgi::Output * out );

  //Function to help add date/time to filenames
  //std::string fmt(const std::string& fmt, ...);

  //Special Test button for sending 2 muons files and storing the results
  void SendAndStore2M(xgi::Input * in, xgi::Output * out );

  //Clears the data from the emulator board
  void ClearEmuBoard(xgi::Input * in, xgi::Output * out );

  void OutputCLCTInfo(xgi::Input * in, xgi::Output * out );

  void OutputMPCFifo(xgi::Input * in, xgi::Output * out );

  void CrateConfiguration(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);





};

}} // namespaces

#endif
