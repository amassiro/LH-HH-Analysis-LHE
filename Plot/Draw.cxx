

void Draw (std::string var = "hbb_mass", int NBIN = 1000, int MIN = 0, int MAX = 1000, std::string varHR = "") {

 if (varHR == "") {
  varHR = var;
 }

 gStyle->SetOptStat(0);

 TFile* f_Sig = new TFile ("/tmp/amassiro/MGraviton_1000_H35H25_WWbb.root");
 TFile* f_TT  = new TFile ("/tmp/amassiro/ttbbjj.root");

 TTree* t_Sig = (TTree*) f_Sig -> Get ("tree");
 TTree* t_TT = (TTree*) f_TT -> Get ("tree");


//--------------------------
 TH1F* h_Sig = new TH1F("Sig","",NBIN,MIN,MAX);
 TH1F* h_TT = new TH1F("TT","",NBIN,MIN,MAX);


//  TString cut = Form ("jetpt1>30 && jetpt2>30 && mjj>300");
 TString cut = Form ("1");
 TString toDraw;


 toDraw = Form ("%s >> Sig",var.c_str());
 t_Sig->Draw(toDraw.Data(),cut.Data(),"goff");
 toDraw = Form ("%s >> TT",var.c_str());
 t_TT->Draw(toDraw.Data(),cut.Data(),"goff");


 h_Sig->SetMarkerColor(kRed);
 h_Sig->SetLineWidth(2);
 h_Sig->SetLineColor(kRed);
 h_Sig->GetXaxis()->SetTitle(varHR.c_str());

 h_TT->SetMarkerColor(kBlue);
 h_TT->SetLineColor(kBlue);
 h_TT->SetLineWidth(2);

 TLegend* leg = new TLegend(0.5,0.7,0.9,0.9);
 leg->AddEntry(h_Sig,"Sig","l");
 leg->AddEntry(h_TT,"TT","l");
 leg->SetFillStyle(0);


 TCanvas* cn = new TCanvas ("cn","cn",800,600);
 h_Sig->DrawNormalized();
 h_TT->DrawNormalized("same");
 leg->Draw();
 cn->SetGrid();

}

