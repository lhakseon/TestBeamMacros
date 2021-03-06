#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <fstream>
using namespace std;

void eff_run(int run_i, int run_f, int ch, float x_i, float x_f, float y_i, float y_f, float thres, int isvme)
{
 
  //Warning: photek cut is hardcoded
 
  TH1F* h_num = new TH1F("h_num","", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_den = new TH1F("h_den", "", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_dummy = new TH1F("h_dummy", "", run_f - run_i + 1, run_i, run_f+1);

  vector<float> no_pulse;
  vector<float> no_rootfile;
  vector<float> no_tracks;
  vector<float> bad_tracks;
  vector<float> def_bad_tracks;
  float x_dut;
  float y_dut;
  float x_corr;
  for (int i=run_i; i < run_f + 1; i++)
    {
      TString root_filename = Form("/Users/rheller/tb_tmp/v7/recover/DataVMETiming_Run%i_2x2mm_2x8_W6_bv360.root", i);
     // TString root_filename = Form("/data/TestBeam/2018_11_November_CMSTiming/RECO/v5/DataVMETiming_Run%i.root", i);
      //TString root_filename = Form("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DataVMETiming_Run%i_2x2mm_2x8_W6_bv360.root", i);                                 
      //if(isvme==0) TString root_filename = Form("/data/TestBeam/2018_11_November_CMSTiming/DT5742/RECO/v6/DT5742_Run%i.root", i);                                
      //if(isvme==0) root_filename = Form("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DT5742_Run%i_2x2mm_2x8_W6_bv360.root", i); 
      //TString root_filename = Form("/eos/uscms/store/group/cmstestbeam/BTL_ETL/2018_11/data/DT5742/RECO/v6/backup/DT5742_Run%i_2x2mm_2x8_W6_bv360.root", i);
      //TString root_filename = Form("/eos/uscms/store/group/cmstestbeam/BTL_ETL/2018_11/data/DT5742/RECO/v6/DT5742_Run%i.root", i);                               
      //TString root_filename = Form("/eos/uscms/store/group/cmstestbeam/BTL_ETL/2018_11/data/VME/RECO/v6/DataVMETiming_Run%i_2x2mm_2x8_W6_bv360.root", i);         
      TFile *f = TFile::Open(root_filename);
      // if (!f){
      //   root_filename = Form("/eos/uscms/store/user/cmstestbeam/BTL_ETL/2018_11_recover/RECO/v7/recover/DataVMETiming_Run%i.root", i);
      //   f = TFile::Open(root_filename);
      // }
      if(f) //file exist                                                                                                                                            
        {
          TTree *pulse = (TTree*)f->Get("pulse");
          if(pulse) //pulse exist                                                                                                                                   
            {
              TBranch * b_x_dut = pulse->Branch("x_dut",&x_dut,"x_dut/F");
              TBranch * b_y_dut = pulse->Branch("y_dut",&y_dut,"y_dut/F");

              TBranch * b_x_corr = pulse->Branch("x_corr",&x_corr,"x_corr/F");
              if(!b_x_corr) cout<<"Run "<<i<<" missing x_corr"<<endl;
              int n_with_tracks = pulse->GetEntries("x_corr>10");
              if(n_with_tracks>0) //track exist      (This doesn't actually check anything)                                                                                                                       
                {
                  //TH1F * h1 = new TH1F("h1","",100,-400,400); ,"",  20, -0.004, 0.004                                                                                              
		  TH1F * x = new TH1F("x","",120,-0.005,0.005);
                  TH1F * y = new TH1F("y","",120,-0.005,0.005);
                  x->StatOverflows();
                  y->StatOverflows();
                  pulse->Project("x","xSlope","xSlope!=-999");
                  pulse->Project("y","ySlope","ySlope!=-999");
                  int binmax1 = x->GetMaximumBin(); float xMean = x->GetBinCenter(binmax1);
                  float xMeanl = x->GetBinLowEdge(binmax1);
                  float xMeanr = x->GetBinLowEdge(binmax1+1);
                  int binmax2 = y->GetMaximumBin(); float yMean = y->GetBinCenter(binmax2);
                  float yMeanl = y->GetBinLowEdge(binmax2);
                  float yMeanr = y->GetBinLowEdge(binmax2+1);
		  //std::cout<<xMean<<endl;
		  //std::cout<<yMean<<endl;
                  //float xMean = x->GetMean();                                                                                                                     
                  //float yMean = y->GetMean();

                  //TString den_cmd = Form("ntracks==1&&chi2<3&&x_corr>%.2f&&x_corr<%.2f&&y_corr>%.2f&&y_corr<%.2f)", x_i, x_f, y_i, y_f);                                                                                                                                                                                                                                                                                              
                  //TString num_cmd = Form("chidx[%i]>=0&&amp[chidx[%i]]>%.2f&&%s", ch,ch,thres,den_cmd.Data());                                                                                                                                                                                                                                                                                                                        
                  //TString den_cmd = Form("ntracks==1&&chi2<3&&x_dut[2]>%.2f&&x_dut[2]<%.2f&&y_dut[2]>%.2f&&y_dut[2]<%.2f", x_i, x_f, y_i, y_f);
                  //TString num_cmd = Form("amp[%i]>%.2f&&%s", ch,thres,den_cmd.Data());                                                                                                                                                                                                                                                                                                                                                
                  //TString den_cmd = Form("ntracks==1&&chi2<3&&xSlope<%f&&xSlope>%f&&ySlope<%f&&ySlope>%f&&x_dut[2]>%.2f&&x_dut[2]<%.2f&&y_dut[2]>%.2f&&y_dut[2]<%.2f", xMeanr, xMeanl, yMeanr, yMeanl, x_i, x_f, y_i, y_f);

                  TString den_cmd = Form("ntracks==1&&chi2<7&&x_corr>%.2f&&x_corr<%.2f&&y_corr>%.2f&&y_corr<%.2f",  x_i, x_f, y_i, y_f);
                  TString num_cmd = Form("amp[%i]>%.2f&&%s", ch,thres,den_cmd.Data());

                  float num_entries = pulse->GetEntries(num_cmd);
                  float den_entries = pulse->GetEntries(den_cmd);
                  float effi; 
                  if(den_entries>0) effi = num_entries/den_entries;

                  den_cmd = Form("ntracks==1&&chi2<7&&x_corr>%.2f&&x_corr<%.2f&&y_corr>%.2f&&y_corr<%.2f",  7.5, 8.7, 20.6, 22.);
                  num_cmd = Form("amp[%i]>%.2f&&%s", 3,thres,den_cmd.Data());
                  //cout<<den_cmd<<endl;
                  float num_entries_2 = pulse->GetEntries(num_cmd);

                  float den_entries_2 = pulse->GetEntries(den_cmd);
                  float effi2; 
                  if(den_entries_2>0) effi2 = num_entries_2/den_entries_2;
//                  cout<<"effi2 is "<<effi2<<" "<<num_entries_2<<" "<<den_entries_2<<endl;

                  //std::cout<<effi_num<<endl;                                                                                                                      
                  //std::cout<<effi_den<<endl;                                                                                                                      
                  //std::cout<<effi<<endl;                                                                                                                          
                  if (effi<0.7) bad_tracks.push_back(i); //bad efficiency    
                  if (effi < 0.7 && effi2<0.7) def_bad_tracks.push_back(i);              

                  h_num->SetBinContent(h_num->FindBin(i), num_entries);
                  h_den->SetBinContent(h_den->FindBin(i), den_entries);

                }
              else no_tracks.push_back(i);
            }
          else no_pulse.push_back(i);
          f->Close();
        }
      else no_rootfile.push_back(i);
    }

  TCanvas *c1 = new TCanvas("c1", "c1",900,900);
  c1->Divide(1,3);
  c1->cd(1);
  h_num->Draw();
  c1->cd(2);
  h_den->Draw();
  c1->cd(3);
  TGraphAsymmErrors* h_eff = new TGraphAsymmErrors();
  h_eff->Divide(h_num,h_den,"cl=0.683 b(1,1) mode");
  h_dummy->Draw();
  h_eff->Draw("EP");


  TString filename = Form("plots/eff_run_run%i_to%i_ch%i.pdf",run_i,run_f,ch);
  c1->Print(filename);

  for (int i=0;i<no_rootfile.size();i++) std::cout<<"Missing root file "<<no_rootfile[i]<<endl;
  for (int i=0;i<no_pulse.size();i++) std::cout<<"Missing pulse "<<no_pulse[i]<<endl;
  for (int i=0;i<no_tracks.size();i++) std::cout<<"Missing tracks "<<no_tracks[i]<<endl;
  for (int i=0;i<bad_tracks.size();i++) std::cout<<"Bad tracks "<<bad_tracks[i]<<endl;
  for (int i=0;i<def_bad_tracks.size();i++) std::cout<<"Definitely bad tracks "<<def_bad_tracks[i]<<endl;

}
 

