// Signal generator with ROOT
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


// This file generates a signal and stores it as binary file and ascii file
// change the signal definition as you like
// the ascii file could be viewed with gnuplot and has time values
// the binray file has only values.


#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

bool verbose = false;

struct ComplexData {
     double re;
     double im;
} bindata;

int nframes; // how many frames should be considered in total, minimum 1
int framelength; // how many points inside one frame, default 1024
char* out_filename;

// main programm

int ParseCmdLine(int argc, char *argv[], char **outFileName) {
     int n = 1;
     if (argc != 7) {
	  cout << "Usage:\n\n";
	  cout << "    signalgen -n nframes -w framelength -o spectrumfile\n\n";
	  exit(EXIT_SUCCESS);
     }
     while (n < argc) {
	  if (!strncmp(argv[n], "-o", 2) || !strncmp(argv[n], "-O", 2)) {
	       *outFileName = argv[++n];
	  } else if (!strncmp(argv[n], "-n", 2) || !strncmp(argv[n], "-N", 2)) {
	       nframes = atoi(argv[++n]);
	  } else if (!strncmp(argv[n], "-w", 2) || !strncmp(argv[n], "-W", 2)) {
	       framelength = atoi(argv[++n]);
	  } else if (!strncmp(argv[n], "-h", 2) || !strncmp(argv[n], "-H", 2)) {
	       cout << "Usage:\n\n";
	       cout << "    signalgen -n nframes -w framelength -o spectrumfile\n\n";
	       exit(EXIT_SUCCESS);
	  } else {
	       cout << "Usage:\n\n";
	       cout << "    signalgen -n nframes -w framelength -o spectrumfile\n\n";
	       exit(EXIT_SUCCESS);
	  }
	  ++n;
     }
     return 0;
}
//
//int ParseCmdLine(int argc, char *argv[], char **outFileName) {
//    int n = 1;
//    if (argc != 3) {
//        cout << "Usage:\n\n";
//        cout << "    signalgen -o outputfile\n\n";
//        exit(EXIT_SUCCESS);
//    }
//    while (n < argc) {
//        if (!strncmp(argv[n], "-o", 2) || !strncmp(argv[n], "-O", 2)) {
//            *outFileName = argv[++n];
//        } else if (!strncmp(argv[n], "-h", 2) || !strncmp(argv[n], "-H", 2)) {
//            cout << "Usage:\n\n";
//            cout << "    signalgen -o outputfile\n\n";
//            exit(EXIT_SUCCESS);
//        } else {
//            cout << "Usage:\n\n";
//            cout << "    signalgen -o outputfile\n\n";
//            exit(EXIT_SUCCESS);
//        }
//        ++n;
//    }
//    return 0;
//}
//

int main(int argc, char **argv) {

     ParseCmdLine(argc, argv, &out_filename);

     int npoints = nframes * framelength; // length of the signal in points

     ofstream outfile_bin, outfile_asc;
     string out_filename_str(out_filename);

     outfile_asc.open((out_filename_str + ".time.asc").c_str(), ios::out);
     if (!outfile_asc) {
	  cout << "Cannot create output file.";
	  return 1;
     }

     outfile_bin.open((out_filename_str + ".time.bin").c_str(), ios::out);
     if (!outfile_bin) {
	  cout << "Cannot create output file.";
	  return 1;
     }
     double Fs = 1000.0; // sampling frequency in Hz
     double Ts = 1 / Fs; // sampling time
     int f0 = 50; // signal frequency
     int f1 = 500; // secondary signal frequency
     int t0 = 0; // starting time
     int t1 = 2; // end tine in seconds

     double t, y_re, y_im;
     for (int i = 0; i < npoints; i++) {
	  t = i * Ts; // in seconds
	  y_re = cos((2 * M_PI * f0 + t)*t); // in volts
	  y_im = sin((2 * M_PI * f0 + t)*t); // in volts f(t) = (f1-f0)*(t/t1) + f0, (
	  if (verbose) cout << t << " s " << y_re << " V " << endl;
	  outfile_asc << t << "  " << y_re << endl;
	  bindata.re = y_re;
	  bindata.im = y_im;
	  outfile_bin.write((char *) &bindata, sizeof bindata);
	  cout << "Progress : " << (i + 1) * 100 / npoints << "%\r" << flush;
     }
     outfile_bin.close();
     outfile_asc.close();
     cout << endl << out_filename_str + ".time.bin and " << out_filename_str + ".time.asc generated." << endl;
}
