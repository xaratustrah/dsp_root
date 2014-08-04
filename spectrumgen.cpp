// Spectrum generator with ROOT
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



// This code generates the spectrum of the a desired time signal
// using FFTW3 Library. Output is normalized.
//
// Settings must be changed to the desired values
//
// input file is structured binary double RE double IM
//


#include <math.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <fftw3.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

struct ComplexData {
     double re;
     double im;
} bindata;

bool verbose = false;
bool shift_spectrum = true;

// main program

char* out_filename;
char* in_filename;
int nframes; // how many frames should be considered in total, minimum 1
int fftwidth; // how many points inside one frame, default 1024
//double center_frequency = 244.0e6;

int ParseCmdLine(int argc, char *argv[], char **inFileName, char ** outFileName) {
     int n = 1;
     if (argc != 9) {
	  cout << "Usage:\n\n";
	  cout << "    spectrumgen -n nframes -w fftwidth -i timefile -o outputfile\n\n";
	  exit(EXIT_SUCCESS);
     }
     while (n < argc) {
	  if (!strncmp(argv[n], "-i", 2) || !strncmp(argv[n], "-I", 2)) {
	       *inFileName = argv[++n];
	  } else if (!strncmp(argv[n], "-o", 2) || !strncmp(argv[n], "-O", 2)) {
	       *outFileName = argv[++n];
	  } else if (!strncmp(argv[n], "-n", 2) || !strncmp(argv[n], "-N", 2)) {
	       nframes = atoi(argv[++n]);
	  } else if (!strncmp(argv[n], "-w", 2) || !strncmp(argv[n], "-W", 2)) {
	       fftwidth = atoi(argv[++n]);
	  } else if (!strncmp(argv[n], "-h", 2) || !strncmp(argv[n], "-H", 2)) {
	       cout << "Usage:\n\n";
	       cout << "    spectrumgen -n nframes -w fftwidth -i timefile -o outputfile\n\n";
	       exit(EXIT_SUCCESS);
	  } else {
	       cout << "Usage:\n\n";
	       cout << "    spectrumgen -n nframes -w fftwidth -i timefile -o outputfile\n\n";
	       exit(EXIT_SUCCESS);
	  }
	  ++n;
     }
     return 0;
}

int fft(ComplexData cab1[], ComplexData * cab2) { // array same as pointer way of notation
     // prepare fftw3
     fftw_complex *in, *out;
     fftw_plan p;
     in = (fftw_complex*) fftw_malloc(sizeof (fftw_complex) * fftwidth);
     out = (fftw_complex*) fftw_malloc(sizeof (fftw_complex) * fftwidth);
     p = fftw_plan_dft_1d(fftwidth, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

     for (int i = 0; i < fftwidth; i++) {
	  in[i][0] = cab1[i].re;
	  in[i][1] = cab1[i].im;
     }

     // do fft

     fftw_execute(p);

     if (shift_spectrum) {
	  // shift spectrum here if needed
	  int k = 0;
	  for (int j = fftwidth / 2; j < fftwidth; j++) {
	       cab2[k].re = out [j][0]/ fftwidth; // normalize
	       cab2[k].im = out [j][1]/ fftwidth;
	       //      cout << cab2[k].re << endl;
	       k++;
	  }
	  for (int j = 0; j < fftwidth / 2; j++) {
	       cab2[k].re = out [j][0]/ fftwidth; // normalize
	       cab2[k].im = out [j][1]/ fftwidth;
	       //        cout << cab2[k].re << endl;
	       k++;
	  }
     } else {
	  // don't shift spectrum for complex signals
	  for (int i = 0; i < fftwidth; i++) {
	       cab2[i].re = out[i][0] / fftwidth;
	       cab2[i].im = out[i][1] / fftwidth;
	  } 
     }
     // delete fftw3 plan

     fftw_destroy_plan(p);
     fftw_free(in);
     fftw_free(out);
     return 0;
}

double pwr(double a, double b){
     return 10*log10((a*a+b*b)/0.001);
}
int main(int argc, char **argv) {

     ParseCmdLine(argc, argv, &in_filename, &out_filename);
     cout << "Calculating spectra..." << endl;
     if (verbose) cout << "Starting the process with " << nframes << " total frame(s) each " << fftwidth << " long." << endl;
     if (verbose) cout << "Openning files..." << endl;
     //if (verbose) cout << in_filename << endl << out_filename << endl;

     string in_filename_str(in_filename);
     string out_filename_str(out_filename);

     ifstream infile;
     infile.open(in_filename_str.c_str(), ios::in | ios::binary);
     if (infile.fail()) {
	  cout << "Cannot open input file or no such file." << endl;
	  return 1;
     }

     ofstream outfile_xyz, outfile_bin, outfile_asc;

     outfile_xyz.open((out_filename_str + ".psd.xyz").c_str(), ios::out);
     if (!outfile_xyz) {
	  cout << "Cannot create output file.";
	  return 1;
     }

     outfile_asc.open((out_filename_str + ".psd.asc").c_str(), ios::out);
     if (!outfile_asc) {
	  cout << "Cannot create output file.";
	  return 1;
     }

     outfile_bin.open((out_filename_str + ".psd.bin").c_str(), ios::out);
     if (!outfile_bin) {
	  cout << "Cannot create output file.";
	  return 1;
     }

     int y = 0; // this is the relative frame starts always from zero
     //    ComplexData cable1[1024]; //static way
     ComplexData * cable1; // dynamic way, needs malloc
     cable1 = (ComplexData *) malloc(fftwidth * sizeof (ComplexData));

     ComplexData * cable2; // dynamic array, needs malloc
     cable2 = (ComplexData *) malloc(fftwidth * sizeof (ComplexData));

     double ps;

     while (true) {
	  if (!infile.eof()) {
	       //            infile.read((char *) &cable1[0], sizeof cable1); // static array case // same as "infile.read((char *) &cable1, sizeof cable1);"
	       infile.read((char *) cable1, fftwidth * sizeof (ComplexData)); // same as &cable1[0]
	       // ---------- hier kommt die eigentliche Transformation
	       fft(cable1, cable2); // same as fft(&cable1[0], &cable2[0]);

	       for (int i = 0; i < fftwidth; i++) {
//                ps = pow(cable2[i].re, 2) + pow(cable2[i].im, 2);
		    ps = pwr(cable2[i].re, cable2[i].im);
		    outfile_xyz << i << "  " << y << "  " << ps << endl;
		    outfile_asc << ps << endl;
		    outfile_bin.write((char *) &ps, sizeof ps);
	       }

	       // ----------
	       y++;
	       if (verbose) cout << "\n\nDone processing frame " << y << endl;
	       cout << "Progress : " << y * 100 / nframes << "%\r" << flush;
	       if (y == nframes) break;

	  } else {
	       if (verbose) cout << "End of input file reached ..." << endl;
	       break;
	  }
     }

     cout << endl;

     infile.close();
     if (verbose) cout << endl << "Input File closed." << endl;

     outfile_asc.close();
     outfile_bin.close();
     outfile_xyz.close();
     cout << endl << out_filename_str << " asc, bin and xyz : Files generated." << endl;
     free(cable1); // just for fun, is not needed at the end of the program
     free(cable2);
     return 0;
}
