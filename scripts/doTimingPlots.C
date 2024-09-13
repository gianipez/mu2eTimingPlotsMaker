
void  doTimingPlots_v2(TString Dir, TFile*File,  TString Name, TString LegName){
  
  File->cd();
  //   TH1F*hm0_old = new TH1F("hm0_old", Form("%s default", Name.Data())               ,100, 0, 0.03);
  TH1F*hm0_new;
  if (Name == "tot_event_timing" || Name == "tot_event_timing_noFilters") {
    hm0_new = new TH1F(Form("hm0_%s", Name.Data()), Form("%s ", Name.Data()),100, 0, 100);
  } else{
    hm0_new = new TH1F(Form("hm0_%s", Name.Data()), Form("%s ", Name.Data()),50, 0, 50);  
  }

  //PlotTime("b01s6400/",Form("%s", Name.Data()), hm0_old);
  PlotTime(Form("%s/", Dir.Data()),Form("%s.csv", Name.Data()), hm0_new);

  //  TH1F*hm1_old = new TH1F("hm1_old",Form("%s default", Name.Data())     ,100, 0, 0.15);
  //  TH1F*hm1_new = new TH1F("hm1_new",Form("%s new sequence", Name.Data()),100, 0, 10);//5);

  //PlotTime("b01s6400/",Form("%s", Name.Data()), hm1_old);
  //  PlotTime(Form("%s/", Dir.Data()),Form("%s", Name.Data()), hm0_new);


  //--------------------------------------------------------------------------------
  TLegend *leg = new TLegend(0.65, 0.84, 0.90, 0.9076, NULL, "brNDC");
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(1);
  leg->SetShadowColor(0);
  //leg->AddEntry(hm0_old, Form("%s default", Name.Data())     , "L");
  double x_q[2]={0.5,0.5};
  double y_q[2]={0.};
  const int nq(2);
  hm0_new->GetQuantiles(nq, y_q, x_q);
  
  leg->AddEntry(hm0_new, Form("%s: median = %2.1f", LegName.Data(), y_q[0]), "P");
  
    
  TPaveText*tp = new TPaveText(0.19, 0.899, 0.27, 0.939, "NDC");//(0.1, 0.2, 0.96, 0.99, "NDC");
  tp->SetBorderSize(0);
  tp->SetFillStyle(0);
  tp->SetLineStyle(0);
  TText* mu2eLabel = tp->AddText("#font[72]{Mu2e} #font[42]{Internal preliminary}");
  mu2eLabel->SetTextSize(0.030);
  mu2eLabel->SetTextAlign(13);

  TImage*ots = TImage::Open("logo-OTS-20200407.pdf");
  //TImage*ots = TImage::Open("mu2e_logo.jpg");
  float tw = 0.06;
  float th = 0.06;
  float tx = 0.19;
  float txgap = -0.01;
  float ty = 1.0-0.05-0.06;
 
  TCanvas*c0 = new TCanvas(Form("c0_%s", Name.Data()),Form("%s timing performance", LegName.Data()),0, 0, 900, 900);
  c0->cd()->SetLogy();
  c0->SetLeftMargin(0.15);
  c0->SetRightMargin(0.05);
  c0->SetTopMargin(0.05);
  c0->SetBottomMargin(0.1);

  TPad* p_ots = new TPad(Form("waterpad_ots_%s", Name.Data()),"waterpad_ots",tx+/*tw+*/txgap,ty-th,tx+2*tw+txgap,ty);
  p_ots->SetFillStyle(0);

  //hm0_old->Draw();
  //  hm0_old->GetXaxis()->SetTitle("time/event [s]");
  hm0_new->GetXaxis()->SetTitle("time/event [ms]");
  
  hm0_new->SetMarkerStyle(23);
  hm0_new->SetMarkerColor(kRed);
  hm0_new->SetLineColor  (kRed);
  //  hm0_new->Draw("Psames");
  hm0_new->SetMaximum(pow(hm0_new->GetMaximum(),1.3));
  hm0_new->Draw("P");
  leg->Draw();

  c0->Update();
  //  TPaveStats *stats1 = (TPaveStats*)hm0_old ->GetListOfFunctions()->FindObject("stats");
  TPaveStats *stats2 = (TPaveStats*)hm0_new ->GetListOfFunctions()->FindObject("stats");
  //  stats1->SetTextColor(kBlue); 
  stats2->SetTextColor(kRed); 
  stats2->SetX1NDC(0.66); stats2->SetX2NDC(0.90);
  stats2->SetY1NDC(0.57); stats2->SetY2NDC(0.82);
  // stats1->SetX1NDC(0.72); stats1->SetX2NDC(0.89);
  // stats1->SetY1NDC(0.46); stats1->SetY2NDC(0.70);
  c0->Modified();
  gPad->Update();
  tp   ->Draw();
  //  p_ots->Draw();
  // p_ots->cd();
  //ots->Draw();
  c0->Update();

  //gSystem->mkdir(Form("%s/plots", Dir.Data()));
  c0->Print(Form("%s/plots/%s.png", Dir.Data(), Name.Data()));

  hm0_new->Write();
  c0->Write();
}

void plotAllTiming(TString Dir){
  std::vector<TString> names = {"makeSD",
				"CaloClusterFast",
			//	"FastCaloHitMaker",
				"CaloHitMakerFast",
				"cprDeLowPStopTargHSFilter",
				"cprDeLowPStopTargPS",
				"cprDeLowPStopTargTCFilter",
				"cprDeLowPStopTargKSFilter",
				"cprDeHighPStopTargHSFilter",
				"cprDeHighPStopTargPS",
				"cprDeHighPStopTargTCFilter",
				"cprDeHighPStopTargKSFilter",
                                "aprLowPStopTargHSFilter",
				"aprLowPStopTargPS",
				"aprLowPStopTargTCFilter",
				"aprLowPStopTargKSFilter",
				"aprHighPStopTargHSFilter",
				"aprHighPStopTargPS",
				"aprHighPStopTargTCFilter",
				"aprHighPStopTargKSFilter",
				//"OfflineFragmentReader",
				"tprHelixDeIpaPhiScaledHSFilter",
				"tprHelixDeIpaPhiScaledPS",
				"tprHelixDeIpaPhiScaledTCFilter",
				"tprHelixDeIpaHSFilter",
				"tprHelixDeIpaPS",
				"tprHelixDeIpaTCFilter",
				"tprDeLowPStopTargHSFilter",
				"tprDeLowPStopTargPS",
				"tprDeLowPStopTargTCFilter",
				"tprDeLowPStopTargKSFilter",
				"tprDeHighPStopTargHSFilter",
				"tprDeHighPStopTargPS",
				"tprDeHighPStopTargTCFilter",
				"tprDeHighPStopTargKSFilter",
				"TTCalHelixFinderDe",
				"TTCalHelixMergerDe",
				"TTCalSeedFitDe",
			//	"TTCalSeedFitDep",
				"TTCalTimePeakFinder",
				"TTflagBkgHits",
				"TThelixFinder",
				"TTHelixMergerDe",
				"TTKSFDe",
                                "TTAprHelixFinder",
                                "TTAprHelixMerger",
                                "TTAprKSF",
                                "TTmakeSH",
				"TTmakePH",
				"TTmakeSTH",
                                "TTTZClusterFinder",
				"TTtimeClusterFinder",
				"tot_event_timing",
				"tot_event_timing_noFilters",
				"tot_timing_filters",
				"tot_timing_prescale",
				"tot_timing_SDFilter",
				"tot_timing_TCFilter",
				"tot_timing_HSFilter",
				"tot_timing_TSFilter"};

  //names = {"TTmakeSTH" , "TTmakePH" , "TTflagBkgHits", "CaloClusterFast" , "TTtimeClusterFinder" , "TThelixFinder" , "TTKSFDeM" , "TTKSFDeP" , "TTCalTimePeakFinder" , "TTCalHelixFinderDe" , "TTCalSeedFitDe" "subsystemOutput_init", "subsystemOutput_write", "makeSD", "CaloDigiFromShower", "tot_event_timing", "OfflineFragmentReader", "subsystemOutput_write","subsystemOutput_init"};

  TFile*ff = new TFile(Form("%s/timing_plots.root", Dir.Data()),"recreate");
  for (auto nn : names){
    TString name = nn;// + ".csv";
    doTimingPlots_v2(Dir, ff, name, nn);
  }
  ff->Close();
}
