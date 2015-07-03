#ifndef _Emu_PC_CSCGEMTestApplication_h_
#define _Emu_PC_CSCGEMTestApplication_h_

#include "xdaq/WebApplication.h"
#include "emu/pc/TMB.h"
//
#include "emu/pc/EmuPeripheralCrateBase.h"

namespace emu { namespace pc {

/** \class CSCGEMTestApplication
 * main web GUI Application class for the CSC-GEM test stand
 */
class CSCGEMTestApplication: public EmuPeripheralCrateBase
{
protected:

  char DirGen[200];
  char DirUp[200];
  char DCFEBN[10];
  char HalfStripN[10];
  char FiberN[10];
  char PatFile[200];
  char NumRuns[20];

  char Fiber0FileName[400];
  char Fiber1FileName[400];
  char Fiber2FileName[400];
  char Fiber3FileName[400];
  char Fiber4FileName[400];
  char Fiber5FileName[400];
  char Fiber6FileName[400];
  char Fiber7FileName[400];

  char Fiber0PatName[200];
  char Fiber1PatName[200];
  char Fiber2PatName[200];
  char Fiber3PatName[200];
  char Fiber4PatName[200];
  char Fiber5PatName[200];
  char Fiber6PatName[200];
  char Fiber7PatName[200];

  bool PatLoadError;
  bool FileLoadedToBoard; //make this an array of 8 later

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

  //Test button for loading the file to the emulator board
  void LoadToEmuBoard(xgi::Input * in, xgi::Output * out );

  //Test button for sending the file from the emulator board
  void SendFromEmuBoard(xgi::Input * in, xgi::Output * out );

  //Clears the data from the emulator board
  void ClearEmuBoard(xgi::Input * in, xgi::Output * out );

  void CrateConfiguration(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);






};

}} // namespaces

#endif
