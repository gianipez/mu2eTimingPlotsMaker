double timeLimit(double mu2eCPUs, double gm2CPUs, double nMu2e=14., double nGM2=22.){
  double nEvts      = 245e3;
  double spill_time = 1400.;//ms
  
  double nTotCPUs = mu2eCPUs*nMu2e + gm2CPUs*nGM2;
  printf("Time limit is %2.3f\n", spill_time/(nEvts/(nTotCPUs)));
  return spill_time/(nEvts/(nTotCPUs));
}

void doTimeLimit(){
  
  double nMu2eNodes = 14.;
  double nGM2Nodes  = 22;
  
  double nMu2eCPUs[8] = {18, 20, 22, 24, 26, 28, 32, 36};
  double nGM2CPUs[9]  = {12, 14, 16, 18, 20, 22, 24, 30, 34};
  int    nTypes(8);

  TH2F*h2    = new TH2F("h2","Requirement on the processing time; nMu2eCPUs/node; nGM2CPUs/node", 10, 17, 37, 12, 11, 35);
  TH2F*h2Exp = new TH2F("h2Exp","Experimentalprocessing time; nMu2eCPUs/node; nGM2CPUs/node", 5, 17, 27, 5, 11, 21);
  
  int counter(0);
  for (int i=0; i<10; ++i){
    for (int j=0; j<12; ++j){
      h2->Fill(18+2*i, 12+2*j, timeLimit(18+2*i, 12+2*j, nMu2eNodes, nGM2Nodes));
    }
  }

  TGraph*grMu2e21BB = new TGraph();
  grMu2e21BB->SetTitle("Mu2eNodes1BB");

  TGraph*grMu2e22BB = new TGraph();
  grMu2e22BB->SetTitle("Mu2eNodes2BB");

  TGraph*grGM21BB  = new TGraph();
  grGM21BB->SetTitle("GM2Nodes");

  TGraph*grGM22BB  = new TGraph();
  grGM22BB->SetTitle("GM2Nodes2BB");
  
  int nTasksMu2e[15] = {1, 2, 4, 8, 10, 12, 14, 16, 20, 22, 24, 26, 28, 32, 36};
  int nTasksGM2[14]  = {1, 2, 4, 8, 10, 12, 14, 16, 18, 20, 22, 24, 30, 34};
  
  TH1F*h;
  TFile*ff;
  for (int i=0; i<15; ++i){
    ff = TFile::Open(Form("input/ntasks_%d/csv_2BB/timing_plots.root", nTasksMu2e[i]));
    if (ff == NULL) {
      printf("Mu2e ntasks %d not found!\n", i);
      continue;
    }
    h = (TH1F*)ff->Get("hm0_tot_event_timing");
    grMu2e22BB->SetPoint(i, nTasksMu2e[i], h->GetMean());

    ff = TFile::Open(Form("input/ntasks_%d/csv_1BB/timing_plots.root", nTasksMu2e[i]));
    if (ff == NULL) {
      printf("Mu2e ntasks %d 1BB not found!\n", nTasksMu2e[i]);
      continue;
    }
    h = (TH1F*)ff->Get("hm0_tot_event_timing");
    grMu2e21BB->SetPoint(i, nTasksMu2e[i], h->GetMean());
  }

  for (int i=0; i<14; ++i){
    ff = TFile::Open(Form("input_gm2/ntasks_%d/csv_2BB/timing_plots.root", nTasksGM2[i]));
    h = (TH1F*)ff->Get("hm0_tot_event_timing");
    grGM22BB->SetPoint(i, nTasksGM2[i], h->GetMean());

    ff = TFile::Open(Form("input_gm2/ntasks_%d/csv_1BB/timing_plots.root", nTasksGM2[i]));
    if (ff == NULL) {
      printf("GM2 ntasks %d 1BB not found!\n", nTasksGM2[i]);
      continue;
    }
    h = (TH1F*)ff->Get("hm0_tot_event_timing");
    grGM21BB->SetPoint(i, nTasksGM2[i], h->GetMean());
  }

  TGraph*grAprMu2e2BB = new TGraph();
  grAprMu2e2BB->SetTitle("Mu2eNodes2BBApr");

  TGraph*grAprCaloMu2e2BB = new TGraph();
  grAprCaloMu2e2BB->SetTitle("Mu2eNodes2BBAprCalo");

  TGraph*grAprGM22BB  = new TGraph();
  grAprGM22BB->SetTitle("GM2Nodes2BBApr");
  
  int nTasksAprMu2e[8] = {4, 8, 12, 16, 20, 26, 30, 36};
  int nTasksAprGM2[7]  = {4, 8, 12, 16, 20, 26, 32};
 
  for (int i=0; i<8; ++i){
    ff = TFile::Open(Form("input_apr/ntasks_%d/csv_2BB/timing_plots.root", nTasksAprMu2e[i]));
    if (ff == NULL) {
      printf("Mu2e APR ntasks %d 2BB not found!\n", nTasksAprMu2e[i]);
      continue;
    } 
    h = (TH1F*)ff->Get("hm0_tot_event_timing");
    grAprMu2e2BB->SetPoint(i, nTasksAprMu2e[i], h->GetMean());

    ff = TFile::Open(Form("input_aprCalo/ntasks_%d/csv_2BB/timing_plots.root", nTasksAprMu2e[i]));
    if (ff == NULL) {
      printf("Mu2e APR Calo ntasks %d 2BB not found!\n", nTasksAprMu2e[i]);
      continue;
    } 
    h = (TH1F*)ff->Get("hm0_tot_event_timing");
    grAprCaloMu2e2BB->SetPoint(i, nTasksAprMu2e[i], h->GetMean());

    ff = TFile::Open(Form("input_aprGm2/ntasks_%d/csv_2BB/timing_plots.root", nTasksAprGM2[i]));
    if (ff == NULL) {
      printf("GM2 Apr ntasks %d 1BB not found!\n", nTasksAprGM2[i]);
      continue;
    }
    h = (TH1F*)ff->Get("hm0_tot_event_timing");
    grAprGM22BB->SetPoint(i, nTasksAprGM2[i], h->GetMean());
  }
 
  
  TCanvas*c0 = new TCanvas("c0","", 0, 0,  900, 900);
  //c0->cd()->SetGridx();
  //c0->cd()->SetGridy();
  c0->SetLeftMargin(0.15);
  c0->SetRightMargin(0.05);
  c0->SetTopMargin(0.05);
  c0->SetBottomMargin(0.1);
  
  gStyle->SetPaintTextFormat("2.2f");
  h2->Draw("col text");

  TCanvas*c1 = new TCanvas("c1","", 0, 0,  900, 900);
  c1->cd()->SetGridx();
  c1->cd()->SetGridy();
  c1->SetLeftMargin(0.15);
  c1->SetRightMargin(0.05);
  c1->SetTopMargin(0.05);
  c1->SetBottomMargin(0.1);
  grMu2e22BB->SetLineColor(kRed);
  grMu2e22BB->SetMarkerColor(kRed);
  grMu2e22BB->SetMarkerStyle(21);
  grMu2e22BB->Draw("AP");
  grMu2e22BB->GetYaxis()->SetRangeUser(1, 12);
  grMu2e22BB->GetYaxis()->SetTitle("ms/event");
  grMu2e22BB->GetXaxis()->SetTitle("nCPUs/node");

  grMu2e21BB->SetLineColor(kBlue);
  grMu2e21BB->SetMarkerColor(kBlue);
  grMu2e21BB->SetMarkerStyle(24);
  grMu2e21BB->Draw("Psame");
  
  grAprMu2e2BB->SetLineColor(kMagenta);
  grAprMu2e2BB->SetMarkerColor(kMagenta);
  grAprMu2e2BB->SetMarkerStyle(20);
  grAprMu2e2BB->Draw("Psame");
  
  grAprCaloMu2e2BB->SetLineColor(kOrange);
  grAprCaloMu2e2BB->SetMarkerColor(kOrange);
  grAprCaloMu2e2BB->SetMarkerStyle(28);
  //grAprCaloMu2e2BB->Draw("Psame");
  
  TLegend *leg = new TLegend(0.15, 0.80, 0.46, 0.95, NULL, "brNDC");
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(1);
  leg->SetShadowColor(0);
  leg->AddEntry(grMu2e22BB, "Mu2e, 2 batch-mode", "P");
  leg->AddEntry(grMu2e21BB, "Mu2e, 1 batch-mode", "P");
  leg->AddEntry(grAprMu2e2BB, "Mu2e (Apr+Cpr), 2 batch-mode", "P");
  //  leg->AddEntry(grAprCaloMu2e2BB, "Mu2e (Apr+Calo-prefilter), 2 batch-mode", "P");
  leg->Draw();
  
  TCanvas*c2 = new TCanvas("c2","", 0, 0,  900, 900);
  c2->cd()->SetGridx();
  c2->cd()->SetGridy();
  c2->SetLeftMargin(0.15);
  c2->SetRightMargin(0.05);
  c2->SetTopMargin(0.05);
  c2->SetBottomMargin(0.1);
  grGM22BB->SetLineColor(kRed);
  grGM22BB->SetMarkerColor(kRed);
  grGM22BB->SetMarkerStyle(21);
  grGM22BB->Draw("AP");
  grGM22BB->GetYaxis()->SetRangeUser(1, 16);
  grGM22BB->GetYaxis()->SetTitle("ms/event");
  grGM22BB->GetXaxis()->SetTitle("nCPUs/node");

  grGM21BB->SetLineColor(kBlue);
  grGM21BB->SetMarkerColor(kBlue);
  grGM21BB->SetMarkerStyle(24);
  grGM21BB->Draw("Psame");
  
  grAprGM22BB->SetLineColor(kMagenta);
  grAprGM22BB->SetMarkerColor(kMagenta);
  grAprGM22BB->SetMarkerStyle(20);
  grAprGM22BB->Draw("Psame");
  
  TLegend *leg0 = new TLegend(0.15, 0.80, 0.46, 0.95, NULL, "brNDC");
  leg0->SetFillStyle(1001);
  leg0->SetFillColor(kWhite);
  leg0->SetBorderSize(1);
  leg0->SetShadowColor(0);
  leg0->AddEntry(grMu2e22BB, "GM2, 2 batch-mode", "P");
  leg0->AddEntry(grMu2e21BB, "GM2, 1 batch-mode", "P");
  leg0->AddEntry(grAprGM22BB, "GM2 (Apr+Cpr), 2 batch-mode", "P");

  leg0->Draw();
  
}
