// Spectrum plotter with ROOT
//
// (c) Copyright M. Shahab SANJARI 2011
//
// this is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// barion is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
//
// along with this software.  If not, see <http://www.gnu.org/licenses/>.


// Compile the code or use
// interactive root session:
// root> .x spectrogramme.cpp
//
// Note: in order for both interactive session and native compile to work, the function name must be the same
// as the file name.
//
// have fun!
//


#include<TApplication.h>
#include<TFrame.h>
#include<TCanvas.h>
#include<TPad.h>
#include<TH2.h>
#include<TH1.h>
#include<TPaletteAxis.h>
#include <TStyle.h>
#include<TList.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// settings

bool verbose = false;
int fFmin = 0;
int fFmax = 1023;


// main programm

char* in_filename;
int nframes; // how many frames should be considered in total, minimum 1
int fftwidth; // how many points inside one frame, default 1024

int ParseCmdLine(int argc, char *argv[], char **inFileName) {
     int n = 1;
     if (argc != 7) {
	  cout << "Usage:\n\n";
	  cout << "    spectrogramme -n nframes -w fftwidth -i spectrumfile\n\n";
	  exit(EXIT_SUCCESS);
     }
     while (n < argc) {
	  if (!strncmp(argv[n], "-i", 2) || !strncmp(argv[n], "-I", 2)) {
	       *inFileName = argv[++n];
	  } else if (!strncmp(argv[n], "-n", 2) || !strncmp(argv[n], "-N", 2)) {
	       nframes = atoi(argv[++n]);
	  } else if (!strncmp(argv[n], "-w", 2) || !strncmp(argv[n], "-W", 2)) {
	       fftwidth = atoi(argv[++n]);
	  } else if (!strncmp(argv[n], "-h", 2) || !strncmp(argv[n], "-H", 2)) {
	       cout << "Usage:\n\n";
	       cout << "    spectrogramme -n nframes -w fftwidth -i spectrumfile\n\n";
	       exit(EXIT_SUCCESS);
	  } else {
	       cout << "Usage:\n\n";
	       cout << "    spectrogramme -n nframes -w fftwidth -i spectrumfile\n\n";
	       exit(EXIT_SUCCESS);
	  }
	  ++n;
     }
     return 0;
}

int spectrogramme() {

     // styles for spectrogramme

     // gStyle->SetPalette(20); // Jet Color scheme
     gStyle->SetPalette(500); // Blue Color scheme
     gStyle->SetCanvasBorderMode(0);
     gStyle->SetFrameBorderMode(0);
     gStyle->SetPadBorderMode(0);
     gStyle->SetPadColor(0);
     gStyle->SetLabelFont(42,"xyz");
     gStyle->SetLabelSize(0.04,"xyz");
     gStyle->SetTitleFont(42,"xyz");
     gStyle->SetTitleSize(0.04,"xyz");
     gStyle->SetTitleFont(42,"");
     //    gStyle->SetLegendFont(42);

     TCanvas *c1 = new TCanvas("c1", "Simple Spectrum Plotter", 200, 10, 1000, 700);
     c1->SetFillColor(0);

     TPad * pad1 = new TPad("pad1", "Shows the spectrum", 0.05, 0.05, 0.95, 0.95, 21);

     pad1->SetFillColor(0);
     pad1->SetBorderMode(0);
     //pad1->SetBorderSize(5);
     pad1->SetBit(TBox::kCannotMove);
     pad1->Draw();
     pad1->cd();

     int npoints = fftwidth * nframes;

     string in_filename_str = string(in_filename);
     string label = "File :" + in_filename_str; 
	
     TH2D * fHist = new TH2D("Spectrum", label.c_str(), fftwidth, fFmin, fFmax, nframes, 0, nframes - 1);

     fHist->SetFillColor(0);
     fHist->SetXTitle("Bins");
     fHist->GetXaxis()->SetTitleOffset(1.1);
     fHist->GetXaxis()->SetTitleSize(0.035);
     fHist->SetYTitle("Frames");
     fHist->SetZTitle("Power [dBm]");
     fHist->SetStats(kFALSE);

     ifstream infile;
     //    infile.open(in_filename, ios::in); // ascii file
     infile.open(in_filename, ios::in | ios::binary); // binary file
     if (!infile) {
	  cout << "Cannot open input file.";
	  return 1;
     }

     double z;
     int x = 0, y = 0;
     int counter = 0;

     while (1) {
	  if (!infile.good()) break;
	  //        infile >> z; // ascii file
	  infile.read((char*) &z, sizeof (z));
	  if (verbose) cout << "value at index " << counter << " is " << z << endl;
	  cout << "Loading file : " << counter * 100 / npoints << "%\r" << flush; // C++ version
	  //        printf("Progress : %d %%\r", counter * 100 / npoints); // C version double %% is needed
	  counter++;
	  fHist->SetBinContent(x, y, z);
	  if (y == nframes) break;
	  else {
	       if (x == fftwidth - 1) {
		    y++;
		    x = 0;
	       } else x++;
	  }
	  if (verbose) cout << "x is: " << x << endl << "y is " << y << endl;
     }
     cout << endl;
     infile.close();
     cout << "Input file closed." << endl;
     pad1->SetLogz(0);
     fHist->Draw("zcol");
     pad1->Update();
     TPaletteAxis *palette = (TPaletteAxis*) fHist->GetListOfFunctions()->FindObject("palette");
     palette->SetBit(TBox::kCannotMove);
     palette->SetLabelSize(0.03);
     c1->ToggleEditor();
     //c1->ToggleToolTips();
     c1->ToggleToolBar();
     c1->ToggleEventStatus();
     //c1->SetCrosshair(1);
     c1->Update();

     // projection of the x axis

     TCanvas *c2 = new TCanvas("c2", "Projection", 200, 10, 1000, 700);
     c2->SetFillColor(0);
     TPad * pad2 = new TPad("pad2", "Shows the projection", 0.05, 0.05, 0.47, 0.95, 21);
     pad2->SetFillColor(0);
     pad2->SetBorderMode(0);
     //pad2->SetBorderSize(5);
     pad2->SetBit(TBox::kCannotMove);
     pad2->Draw();

     TPad * pad3 = new TPad("pad2", "Shows the projection", 0.52, 0.05, 0.95, 0.95, 21);
     pad3->SetFillColor(0);
     pad3->SetBorderMode(0);
     //pad3->SetBorderSize(5);
     pad3->SetBit(TBox::kCannotMove);
     pad3->Draw();

     pad2->cd();
     TH1D *fHistProj;
     fHistProj = fHist->ProjectionX("x-projection",40000,40010);
     fHistProj->SetFillColor(0);
     fHistProj->SetXTitle("Bins");
     fHistProj->GetXaxis()->SetTitleOffset(1.1);
     fHistProj->GetXaxis()->SetTitleSize(0.035);
     fHistProj->SetYTitle("Power [dBm]");
     fHistProj->SetStats(kTRUE); // show stats for integration
     fHistProj->SetLineColor(kRed); // line color is red
     fHistProj->Draw();
     pad2->Update();



     pad3->cd();
     TH1D *fHistProj2;
     fHistProj2 = fHist->ProjectionX("x-projection",45000,45010);
     fHistProj2->SetFillColor(0);
     fHistProj2->SetXTitle("Bins");
     fHistProj2->GetXaxis()->SetTitleOffset(1.1);
     fHistProj2->GetXaxis()->SetTitleSize(0.035);
     fHistProj2->SetYTitle("Power [dBm]");
     fHistProj2->SetStats(kTRUE); // show stats for integration
     fHistProj2->SetLineColor(kRed); // line color is red
     fHistProj2->Draw();
     pad3->Update();

     c2->ToggleEditor();
     //c2->ToggleToolTips();
     c2->ToggleToolBar();
     c2->ToggleEventStatus();
     //c2->SetCrosshair(1);
     c2->Update();

     // subtraction of the spectra

     TCanvas *c3 = new TCanvas("c3", "Projection", 200, 10, 1000, 700);
     c2->SetFillColor(0);
     TPad * pad4 = new TPad("pad2", "Shows the projection", 0.05, 0.05, 0.95, 0.95, 21);
     pad4->SetFillColor(0);
     pad4->SetBorderMode(0);
     //pad4->SetBorderSize(5);
     pad4->SetBit(TBox::kCannotMove);
     pad4->Draw();

     pad4->cd();
     TH1D *fHistProj3 = new TH1D();

     for (int i = 0; i<1024 ; i++)
	  fHistProj3->SetBinContent(i, fHistProj->GetBinContent(i)-fHistProj2->GetBinContent(i));

     fHistProj3->SetFillColor(0);
     fHistProj3->SetXTitle("Bins");
     fHistProj3->GetXaxis()->SetTitleOffset(1.1);
     fHistProj3->GetXaxis()->SetTitleSize(0.035);
     fHistProj3->SetYTitle("Power [dBm]");
     fHistProj3->SetStats(kTRUE); // show stats for integration
     fHistProj3->SetLineColor(kRed); // line color is red
     fHistProj3->Draw();
     pad4->Update();

     c3->ToggleEditor();
     //c3->ToggleToolTips();
     c3->ToggleToolBar();
     c3->ToggleEventStatus();
     //c3->SetCrosshair(1);
     c3->Update();

     // subtraction

     return 0;
}

int main(int argc, char **argv) {

     ParseCmdLine(argc, argv, &in_filename);
     cout << "Generating spectrogramme..." << endl;
     // Instanz der Applikation
     TApplication meineApp("Spectrogramme", &argc, argv);

     // Rechnen
     spectrogramme();

     // und los
     meineApp.Run();

     return 0;
}
