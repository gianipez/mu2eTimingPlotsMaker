void doTimingPlots_v2(TString Dir, TFile* File, TString Name, TString LegName) {
    File->cd();
    TH1F* hm0_new;
    if (Name == "tot_event_timing" || Name == "tot_event_timing_noFilters") {
        hm0_new = new TH1F(Form("hm0_%s", Name.Data()), Form("%s ", Name.Data()), 50, 0, 60);
    } else {
        hm0_new = new TH1F(Form("hm0_%s", Name.Data()), Form("%s ", Name.Data()), 40, 0, 20);
    }

    // Declare a float variable to hold the total time
    float totalTime = 0.0;

    // Call PlotTime with the additional argument
    PlotTime(Form("%s/", Dir.Data()), Form("%s.csv", Name.Data()), hm0_new, totalTime);

    // Rest of the code remains unchanged
    TLegend* leg = new TLegend(0.65, 0.84, 0.90, 0.9076, NULL, "brNDC");
    leg->SetFillStyle(1001);
    leg->SetFillColor(kWhite);
    leg->SetBorderSize(1);
    leg->SetShadowColor(0);

    int numBins = hm0_new->GetXaxis()->GetNbins();
    Double_t* x = new Double_t[numBins];
    Double_t* y = new Double_t[numBins];
    for (int i = 0; i < numBins; i++) {
        x[i] = hm0_new->GetBinCenter(i);
        y[i] = hm0_new->GetBinContent(i);
    }
    double medianOfHisto = TMath::Median(numBins, x, y);

    leg->AddEntry(hm0_new, Form("median = %2.1f", medianOfHisto), "P");

    TPaveText* tp = new TPaveText(0.19, 0.899, 0.27, 0.939, "NDC");
    tp->SetBorderSize(0);
    tp->SetFillStyle(0);
    tp->SetLineStyle(0);
    TText* mu2eLabel = tp->AddText("#font[72]{Mu2e} #font[42]{Internal preliminary}");
    mu2eLabel->SetTextSize(0.030);
    mu2eLabel->SetTextAlign(13);

    TCanvas* c0 = new TCanvas(Form("c0_%s", Name.Data()), Form("%s timing performance", LegName.Data()), 0, 0, 900, 900);
    c0->cd()->SetLogy();
    c0->SetLeftMargin(0.15);
    c0->SetRightMargin(0.05);
    c0->SetTopMargin(0.05);
    c0->SetBottomMargin(0.1);

    hm0_new->GetXaxis()->SetTitle("time/event [ms]");
    hm0_new->SetMarkerStyle(23);
    hm0_new->SetMarkerColor(kRed);
    hm0_new->SetLineColor(kRed);
    hm0_new->SetMaximum(pow(hm0_new->GetMaximum(), 1.3));
    hm0_new->Draw("P");
    leg->Draw();

    c0->Update();
    TPaveStats* stats2 = (TPaveStats*)hm0_new->GetListOfFunctions()->FindObject("stats");
    stats2->SetTextColor(kRed);
    stats2->SetX1NDC(0.66);
    stats2->SetX2NDC(0.90);
    stats2->SetY1NDC(0.57);
    stats2->SetY2NDC(0.82);
    c0->Modified();
    gPad->Update();
    tp->Draw();
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
				"TTflagPH",
				"TTDeltaFinder",
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

  //names = {"TTmakeSTH" , "TTmakePH" , "TTflagPH", "CaloClusterFast" , "TTtimeClusterFinder" , "TThelixFinder" , "TTKSFDeM" , "TTKSFDeP" , "TTCalTimePeakFinder" , "TTCalHelixFinderDe" , "TTCalSeedFitDe" "subsystemOutput_init", "subsystemOutput_write", "makeSD", "CaloDigiFromShower", "tot_event_timing", "OfflineFragmentReader", "subsystemOutput_write","subsystemOutput_init"};

  TFile*ff = new TFile(Form("%s/timing_plots.root", Dir.Data()),"recreate");
  for (auto nn : names){
    TString name = nn;// + ".csv";
    doTimingPlots_v2(Dir, ff, name, nn);
  }
  ff->Close();
}
