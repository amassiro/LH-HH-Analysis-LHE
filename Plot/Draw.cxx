

void Draw (std::string var = "hbb_mass", int NBIN = 1000, int MIN = 0, int MAX = 1000, std::string varHR = "") {

 if (varHR == "") {
  varHR = var;
 }

 gStyle->SetOptStat(0);

 int nSig = 5;
 TFile* f_Sig[100];
 TTree* t_Sig[100];
 TH1F*  h_Sig[100];

 std::vector<std::string> vNameSig;
 std::vector<std::string> vNameSigHR;
 std::vector<double> vXsecSig;

//
//  BR Hbb = 5.77E-01
//  BR Hww = 2.15E-01
//  BR Hwwlvlv = 1.0105e-02 = 2.15E-01*0.047 // l = e/m
//
//  *1000.--> fb
 //

 double LUMI = 1000.; //---- 1 ab

 vNameSig.push_back("/tmp/amassiro/XHH/MGraviton_1450_ww_lvlv.root");        vNameSigHR.push_back("X 1450 GeV");   vXsecSig.push_back(1.68E-05 * 0.14 * 5.77E-01*1.0105e-02/20000. * 1000.);
 vNameSig.push_back("/tmp/amassiro/XHH/MGraviton_1050_ww_lvlv.root");        vNameSigHR.push_back("X 1050 GeV");   vXsecSig.push_back(9.85E-05 * 0.30 * 5.77E-01*1.0105e-02/20000. * 1000.);
 vNameSig.push_back("/tmp/amassiro/XHH/MGraviton_850_ww_lvlv.root");         vNameSigHR.push_back("X  850 GeV");   vXsecSig.push_back(3.00E-04 * 0.35 * 5.77E-01*1.0105e-02/20000. * 1000.);
 vNameSig.push_back("/tmp/amassiro/XHH/MGraviton_850_ww_lvlv.root");         vNameSigHR.push_back("X  850 GeV");   vXsecSig.push_back(3.00E-04 * 0.35 * 5.77E-01*1.0105e-02/20000. * 1000.);
 vNameSig.push_back("/tmp/amassiro/XHH/MGraviton_300_ww_lvlv.root");         vNameSigHR.push_back("X  300 GeV");   vXsecSig.push_back(2.14E-01 * 0.02 * 5.77E-01*1.0105e-02/20000. * 1000.);


 nSig = vNameSig.size();

 TString name;
 for (int iSig = 0; iSig < nSig; iSig++) {
  f_Sig[iSig] = new TFile (vNameSig.at(iSig).c_str());
 }


 //---- background

//  TFile* f_ttjj  = new TFile ("/tmp/amassiro/ttbbjj.root");
 TFile* f_ttjj  = new TFile ("/tmp/amassiro/ttbbjj_novbf.root");
 TFile* f_wwbbjj  = new TFile ("/tmp/amassiro/wwbbjj.root");


 double ttjj_xsec = 11.789 * 1000./10000.; //---> without VBF cuts
//  double ttjj_xsec = 2.0715 * 1000./10000.;
 double wwbbjj_xsec = 456.11 * 1000./10000.;

 //---- trees
 TTree* t_ttjj = (TTree*) f_ttjj -> Get ("tree");
 TTree* t_wwbbjj = (TTree*) f_wwbbjj -> Get ("tree");
 for (int iSig = 0; iSig < nSig; iSig++) {
  t_Sig[iSig] = (TTree*) f_Sig[iSig] -> Get ("tree");
  std::cout << " t_Sig[" << iSig << "] = " << t_Sig[iSig]->GetEntries() << std::endl;
 }


//--------------------------
 TH1F* h_ttjj = new TH1F("ttjj","",NBIN,MIN,MAX);
 TH1F* h_wwbbjj = new TH1F("wwbbjj","",NBIN,MIN,MAX);
 for (int iSig = 0; iSig < nSig; iSig++) {
  name = Form ("hSig_%d",iSig);
  h_Sig[iSig] = new TH1F(name.Data(),vNameSigHR.at(iSig).c_str(),NBIN,MIN,MAX);
 }


//  TString cut = Form ("jetpt1>30 && jetpt2>30 && mjj>400 && detajj>3.5");
//  TString cut = Form ("1");
 TString cut = Form (" \
   jetpt1>30 && jetpt2>30 \
   && bjetpt1>25 && bjetpt2>25 \
   && abs(bjeteta1)<2.5 && abs(bjeteta2)<2.5 \
   && abs(jeteta1)<4.5 && abs(jeteta2)<4.5  \
   && pt1>20 && pt2>10 && abs(eta1)<2.5 && abs(eta2)<2.5 \
   && hbb_mass > 110 && hbb_mass < 140 \
   && mll < 70 \
   && ptll > 30 \
   && hww_mt < 125 \
   && hbb_pt > 100 \
   ");

//  && mjj>300 && detajj>3.0  \

 
 TString toDraw;
 TString weight = Form ("1");

 for (int iSig = 0; iSig < nSig; iSig++) {
//   std::cout << t_Sig[iSig] -> GetEntries() << std::endl;
  toDraw = Form ("%s >> hSig_%d",var.c_str(),iSig);
//   std::cout << "toDraw = " << toDraw.Data() << std::endl;

  weight = Form ("(%s) * %f",cut.Data(),vXsecSig.at(iSig)*LUMI);
  t_Sig[iSig]->Draw(toDraw.Data(),weight.Data(),"goff");
 }

 toDraw = Form ("%s >> ttjj",var.c_str());
 weight = Form ("(%s) * %f",cut.Data(),ttjj_xsec*LUMI);
 t_ttjj->Draw(toDraw.Data(),weight.Data(),"goff");
 toDraw = Form ("%s >> wwbbjj",var.c_str());
 weight = Form ("(%s) * %f",cut.Data(),wwbbjj_xsec*LUMI);
 t_wwbbjj->Draw(toDraw.Data(),weight.Data(),"goff");



 //---- estetica
 for (int iSig = 0; iSig < nSig; iSig++) {
  h_Sig[iSig]->SetMarkerColor (kRed+iSig);
  h_Sig[iSig]->SetLineColor   (kRed+iSig);
  h_Sig[iSig]->SetLineWidth(2);
  h_Sig[iSig]->SetLineStyle(1+iSig);
  h_Sig[iSig]->GetXaxis()->SetTitle(varHR.c_str());
 }

 h_ttjj->SetMarkerColor(kBlue);
 h_ttjj->SetLineColor(kBlue);
 h_ttjj->SetLineWidth(2);
 h_ttjj->GetXaxis()->SetTitle(varHR.c_str());

 h_wwbbjj->SetMarkerColor(kGreen);
 h_wwbbjj->SetLineColor(kGreen);
 h_wwbbjj->SetLineWidth(2);

 TLegend* leg = new TLegend(0.5,0.7,0.9,0.9);
 for (int iSig = 0; iSig < nSig; iSig++) {
  leg->AddEntry(h_Sig[iSig],vNameSigHR.at(iSig).c_str(),"l");
 }
 leg->AddEntry(h_ttjj,"ttjj","l");
 leg->AddEntry(h_wwbbjj,"wwbbjj","l");
 leg->SetFillStyle(0);

 //---- normalized
 TCanvas* cn = new TCanvas ("cn","cn",800,600);
 for (int iSig = 0; iSig < nSig; iSig++) {
  if (iSig ==0) h_Sig[iSig]->DrawNormalized();
  else  h_Sig[iSig]->DrawNormalized("same");
 }
 h_ttjj->DrawNormalized("same");
 h_wwbbjj->DrawNormalized("same");
 leg->Draw();
 cn->SetGrid();

 //---- lumi scaled
 TCanvas* cnlumi = new TCanvas ("cnlumi","cnlumi",800,600);
 h_ttjj->Draw();
 h_wwbbjj->Draw("same");
 for (int iSig = 0; iSig < nSig; iSig++) {
  h_Sig[iSig]->Draw("same");
 }
 leg->Draw();
 cn->SetGrid();

}



