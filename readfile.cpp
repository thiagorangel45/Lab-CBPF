//
// $Id:$
//
// File: example04.cpp
// Purpose: Read a text file of data into a newly created ntuple, named "mynt".
//  The ntuple created here is a particularly simple one using only floating
//  point numbers.  More complicated structures can be produced using TTree
//  instead of TNtuple.  (See http://root.cern.ch/root/html/TTree.html)
//  
//  This example also shows how to fill histograms from the Ntuple using the
//  Project method of TNtuple (actually, TTree via inheritance).
//
//  To use this macro, copy example04.dat into the same directory as this
//  macro, and then at the root prompt, type .x example04.cpp
//
// Created: 06-NOV-2000   
//
// $Revision:$
//
//
#include <fstream>   // Standard C++ means for reading from a file


// Include files
void readfile() {

  // Open the data file
  const char* filename="vel-03-05-24.asc";
  ifstream infile(filename);
  if( !infile ) {
    cout << "Error opening " << filename << " to read data.  Exiting" << endl;
    return ;
  }

  // Initialize the ntuple to contain two variables named x and y
  TNtuple nt("mynt","This is a test ntuple","x:y");

  // Read entries from the file until there are no more.  This does no 
  // error checking...
  float x[2];               // Ntuple's require an array as the arguement to fill 
//  infile >> x[0];
  int i=0;
  x[0]=i;
  infile >> x[1];    // Read the initial pair
  //x[1]=x[1]&65535; 
  nt.Fill(x);

  while( infile ) {   
//    infile >> x[0];
    i=i+1;
    x[0]=i;
    infile >> x[1];
    //x[1]=x[1]&65535;
    nt.Fill(x);
   
  }

  // Just for kicks, plot the data as y vs. x
  nt.Draw("y:x");

  // Now create two 1D histograms, one for x and one for y.  I'll use two
  // different syntaxes for this.  The first creates an object directly, and 
  // the second creates it and a pointer to it via new.
//  TH1F xhist("xval","X data only",80,-4.0,4.0);
//  nt.Project("xval","x");
//  TH1F *yhist = new TH1F("yval","Y data title",80,0.,10000.0);
//  nt.Project("yval","y");
  TH2F xyhist("xybckval","LifeT",8191,0.,8190.,100,0.,100.);
  nt.Project("xybckval","y:x");

  // And save the ntuple and the histogram
  TFile ntout("MuonVelocity.root","RECREATE");
  nt.Write("Nt");
//  xhist.Write("XData");
//  yhist->Write("YData");
  xyhist.Write("XYBckData");
  ntout.Close();

  // leave
  return;
}
