// Interactive Signal generator with ROOT
//
// (c) Copyright M. Shahab SANJARI 2014
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


#include <vector>

struct ComplexData {
     Double_t re;
     Double_t im;
} cd;

struct RealData{
     Double_t re;
} rd;

Double_t f_r = 50.0; // Hz
Double_t f_s = 1000.0; // Hz
Int_t m = 20; // number of harmonics
Double_t duration = 2 ; // time in seconds

vector<Double_t> make_cos(Double_t f_r, Double_t f_s, Double_t duration)
{
     vector<Double_t> data;
     Double_t t_s = 1 / f_s; // 1/s
     Double_t npoints = duration / t_s;
     Double_t A = 1;

     for (int i = 0; i < npoints; ++i)
	  
	  data.push_back(A * TMath::Cos(2 * TMath::Pi() * f_r * i * t_s));
     return data;
}

void print_signal(vector<Double_t> data){
     for(vector<Double_t>::iterator it = data.begin(); it != data.end(); ++it)
	  cout << *it << endl;
}

void print_signal2(vector<Double_t> data){
     for (int i = 0; i < data.size(); ++i)
	  cout << data.at(i) << endl;
}

TH1D * time_histo(vector<Double_t> data, Double_t duration){
     TH1D * h = new TH1D("h", "Time domain plot;Time [s];Voltage [V]", data.size(), 0, duration);
     for (int i = 1; i <= data.size(); ++i){
	  h->SetBinContent(i, data.at(i-1));
     }
     return h;
     // gROOT->SetStyle("Plain");
     // gROOT->Reset();
     // gStyle->SetPalette(1);
    
     // TCanvas* c = new TCanvas("c", "Plot", 600, 400);
     // c->SetGrid();
     // c->ToggleEditor();
     // c->ToggleEventStatus();
     // c->ToggleToolBar();
     // h->Draw("C");

}

TH1D * freq_mag(TH1D * time_histo)
{
     // frequency points are multiples of fs/nfft, the maximum range is the fs itself, npoint*fs/nfft
     // negative frequencies are on the second half
     Double_t duration = time_histo->GetXaxis()->GetBinUpEdge(time_histo->GetNbinsX())-time_histo->GetXaxis()->GetBinLowEdge(1);
     Double_t fs = time_histo->GetNbinsX() / duration;
     Int_t npoints = time_histo->GetNbinsX();
    
     TH1 * freq_histo = new TH1D("h2", "Frequency domain plot;Frequency [Hz];Power [dBm]", npoints, 0, fs);
     freq_histo->SetAxisRange(0, fs/2, "X");
     freq_histo->Scale(1.0/TMath::Sqrt(npoints));
     TVirtualFFT::SetTransform(0);
     freq_histo = time_histo->FFT(freq_histo, "mag R2C M");
     //NOTE: for "real" frequencies you have to divide the x-axes range with the range of your function
     //(in this case 4*Pi); y-axes has to be rescaled by a factor of 1/SQRT(n) to be right: this is not done automatically!

    
     return (TH1D*)freq_histo;
}

TH1D * freq_ph(TH1D * time_histo)
{
     TH1 * freq_histo = 0;
     TVirtualFFT::SetTransform(0);
     freq_histo = time_histo->FFT(freq_histo, "mag R2C M");

}


vector<Double_t> add_vectors(vector<Double_t> first , vector<Double_t> second)
{

}

vector<Double_t> make_noise()
{

}

vector<Double_t> make_schottky_signal()
{


}
