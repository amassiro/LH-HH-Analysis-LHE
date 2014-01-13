// to compile:
//   c++ -o ntupleMaker.exe `root-config --glibs --cflags` -lm ntupleMaker.cpp
//
// to use:
//   ./ntupleMaker.exe fileLHE    outputFileRoot
//

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>


#include "LHEF.h"



#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"


struct ptsort: public std::binary_function<TLorentzVector, TLorentzVector, bool> {
 bool operator () (const TLorentzVector & x, const TLorentzVector & y) {
  return  (x.Perp () < y.Perp () ) ;
 }
} ;

TLorentzVector buildP (const LHEF::HEPEUP & event, int iPart) {
 TLorentzVector dummy ;
 dummy.SetPxPyPzE (
   event.PUP.at (iPart).at (0), // px
   event.PUP.at (iPart).at (1), // py
   event.PUP.at (iPart).at (2), // pz
   event.PUP.at (iPart).at (3) // E
   ) ;
 return dummy ;
}



class myTree {

 public:

  myTree();
  ~myTree() {};

  TTree* tree;

  int njet_;

  float jetpt1_;
  float jeteta1_;
  float jetphi1_;
  float jetmass1_;

  float jetpt2_;
  float jeteta2_;
  float jetphi2_;
  float jetmass2_;

  float jetpt3_;
  float jeteta3_;
  float jetphi3_;
  float jetmass3_;

  float jetpt4_;
  float jeteta4_;
  float jetphi4_;
  float jetmass4_;

  float jetpt5_;
  float jeteta5_;
  float jetphi5_;
  float jetmass5_;

  float jetpt6_;
  float jeteta6_;
  float jetphi6_;
  float jetmass6_;


  float hbb_pt_;
  float hbb_phi_;
  float hbb_eta_;

  // hww
  float pt1_;
  float eta1_;
  float phi1_;
  float mass1_;

  float pt2_;
  float eta2_;
  float phi2_;
  float mass2_;

  void fillTree (std::string fileNameLHE);
  void Write(TFile& out);
  void Init();

};


myTree::myTree(){
 tree = new TTree("tree","tree");

 tree->Branch("pt1",&pt1_,"pt1/F");
 tree->Branch("eta1",&eta1_,"eta1/F");
 tree->Branch("phi1",&phi1_,"phi1/F");
 tree->Branch("mass1",&mass1_,"mass1/F");

 tree->Branch("pt2",&pt2_,"pt2/F");
 tree->Branch("eta2",&eta2_,"eta2/F");
 tree->Branch("phi2",&phi2_,"phi2/F");
 tree->Branch("mass2",&mass2_,"mass2/F");


 tree->Branch("njet",&njet_,"njet/I");

 tree->Branch("jetpt1",&jetpt1_,"jetpt1/F");
 tree->Branch("jeteta1",&jeteta1_,"jeteta1/F");
 tree->Branch("jetphi1",&jetphi1_,"jetphi1/F");
 tree->Branch("jetmass1",&jetmass1_,"jetmass1/F");

 tree->Branch("jetpt2",&jetpt2_,"jetpt2/F");
 tree->Branch("jeteta2",&jeteta2_,"jeteta2/F");
 tree->Branch("jetphi2",&jetphi2_,"jetphi2/F");
 tree->Branch("jetmass2",&jetmass2_,"jetmass2/F");

 tree->Branch("jetpt3",&jetpt3_,"jetpt3/F");
 tree->Branch("jeteta3",&jeteta3_,"jeteta3/F");
 tree->Branch("jetphi3",&jetphi3_,"jetphi3/F");
 tree->Branch("jetmass3",&jetmass3_,"jetmass3/F");

 tree->Branch("jetpt4",&jetpt4_,"jetpt4/F");
 tree->Branch("jeteta4",&jeteta4_,"jeteta4/F");
 tree->Branch("jetphi4",&jetphi4_,"jetphi4/F");
 tree->Branch("jetmass4",&jetmass4_,"jetmass4/F");

 tree->Branch("jetpt5",&jetpt5_,"jetpt5/F");
 tree->Branch("jeteta5",&jeteta5_,"jeteta5/F");
 tree->Branch("jetphi5",&jetphi5_,"jetphi5/F");
 tree->Branch("jetmass5",&jetmass5_,"jetmass5/F");

 tree->Branch("jetpt6",&jetpt6_,"jetpt6/F");
 tree->Branch("jeteta6",&jeteta6_,"jeteta6/F");
 tree->Branch("jetphi6",&jetphi6_,"jetphi6/F");
 tree->Branch("jetmass6",&jetmass6_,"jetmass6/F");

}

void myTree::Init(){
 jetpt1_ = -99;
 jeteta1_ = -99;
 jetphi1_ = -99;
 jetmass1_ = -99;

 jetpt2_ = -99;
 jeteta2_ = -99;
 jetphi2_ = -99;
 jetmass2_ = -99;

 jetpt3_ = -99;
 jeteta3_ = -99;
 jetphi3_ = -99;
 jetmass3_ = -99;

 jetpt4_ = -99;
 jeteta4_ = -99;
 jetphi4_ = -99;
 jetmass4_ = -99;

 jetpt5_ = -99;
 jeteta5_ = -99;
 jetphi5_ = -99;
 jetmass5_ = -99;

 jetpt6_ = -99;
 jeteta6_ = -99;
 jetphi6_ = -99;
 jetmass6_ = -99;

 njet_ = 0;

}


void myTree::fillTree(std::string fileNameLHE){
 std::ifstream ifs (fileNameLHE.c_str ()) ;
 LHEF::Reader reader (ifs) ;

 long ieve = 0 ;
 long selected = 0 ;
 double jetsNum = 0. ;

 std::cout << " reading " << fileNameLHE << std::endl;

 // loop over events
 while ( reader.readEvent () ) {
  ieve++;
  if (ieve % 10000 == 0) std::cout << "event " << ieve << "\n" ;

  TLorentzVector HiggsA, HiggsB;
  int iPartHiggs = -1;

  std::vector<int> finalJets ;
  std::vector<TLorentzVector> v_f_quarks ;
  std::vector<TLorentzVector> v_f_leptons ;
  std::vector<TLorentzVector> v_f_neutrinos ;

  // loop over particles in the event
  // and fill the variables of leptons and quarks
  for (int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart) {

   // look for the Higgs
   if (abs (reader.hepeup.IDUP.at (iPart)) == 25) {
    iPartHiggs = iPart;
    HiggsA.SetPxPyPzE  (
      reader.hepeup.PUP.at (iPart).at (0), // px
    reader.hepeup.PUP.at (iPart).at (1), // py
    reader.hepeup.PUP.at (iPart).at (2), // pz
    reader.hepeup.PUP.at (iPart).at (3) // E
                      );
   }

   if (abs (reader.hepeup.IDUP.at (iPart)) == 35) {
    iPartHiggs = iPart;
    HiggsB.SetPxPyPzE  (
      reader.hepeup.PUP.at (iPart).at (0), // px
    reader.hepeup.PUP.at (iPart).at (1), // py
    reader.hepeup.PUP.at (iPart).at (2), // pz
    reader.hepeup.PUP.at (iPart).at (3) // E
                       );
   }

   // outgoing particles
   if (reader.hepeup.ISTUP.at (iPart) == 1) {
    // quarks
    if (abs (reader.hepeup.IDUP.at (iPart)) < 7) {
     finalJets.push_back (iPart) ;
     TLorentzVector dummy (
       reader.hepeup.PUP.at (iPart).at (0), // px
     reader.hepeup.PUP.at (iPart).at (1), // py
     reader.hepeup.PUP.at (iPart).at (2), // pz
     reader.hepeup.PUP.at (iPart).at (3) // E
                          ) ;
     v_f_quarks.push_back (dummy) ;
    } // quarks
    else if (abs (reader.hepeup.IDUP.at (iPart)) == 11 || abs (reader.hepeup.IDUP.at (iPart)) == 13 || abs (reader.hepeup.IDUP.at (iPart)) == 15) {  // e = 11,   mu = 13,   tau = 15
     TLorentzVector dummy (
       reader.hepeup.PUP.at (iPart).at (0), // px
     reader.hepeup.PUP.at (iPart).at (1), // py
     reader.hepeup.PUP.at (iPart).at (2), // pz
     reader.hepeup.PUP.at (iPart).at (3) // E
                          ) ;
     v_f_leptons.push_back (dummy) ;
    }
    else if (abs (reader.hepeup.IDUP.at (iPart)) == 12 || abs (reader.hepeup.IDUP.at (iPart)) == 14 || abs (reader.hepeup.IDUP.at (iPart)) == 16) { // ve = 12,   vmu = 14,   vtau = 16
     TLorentzVector dummy
       (
       reader.hepeup.PUP.at (iPart).at (0), // px
     reader.hepeup.PUP.at (iPart).at (1), // py
     reader.hepeup.PUP.at (iPart).at (2), // pz
     reader.hepeup.PUP.at (iPart).at (3) // E
       ) ;
     v_f_neutrinos.push_back (dummy) ;
    }
   } // outgoing particles
  } // loop over particles in the event

  // sorting in pt
  sort (v_f_quarks.rbegin (), v_f_quarks.rend (), ptsort ()) ;
  sort (v_f_leptons.rbegin (), v_f_leptons.rend (), ptsort ()) ;

//   TLorentzVector diLepton = v_f_leptons.at (0) + v_f_leptons.at (1) ;
//   TLorentzVector missingEnergy = v_f_neutrinos.at (0) + v_f_neutrinos.at (1) ;
//   TLorentzVector dilepton_plus_dineutrinos = v_f_leptons.at (0) + v_f_leptons.at (1) + v_f_neutrinos.at (0) + v_f_neutrinos.at (1) ;


  //---- fill the ntuple
  Init();

  if (v_f_quarks.size()>0) {
   jetpt1_ = v_f_quarks.at (0).Pt ();
   jeteta1_ = v_f_quarks.at (0).Eta ();
   jetphi1_ = v_f_quarks.at (0).Phi ();
   jetmass1_ = v_f_quarks.at (0).M ();
  }
  if (v_f_quarks.size()>1) {
   jetpt2_ = v_f_quarks.at (1).Pt ();
   jeteta2_ = v_f_quarks.at (1).Eta ();
   jetphi2_ = v_f_quarks.at (1).Phi ();
   jetmass2_ = v_f_quarks.at (1).M ();
  }

  if (v_f_quarks.size()>2) {
   jetpt3_ = v_f_quarks.at (2).Pt ();
   jeteta3_ = v_f_quarks.at (2).Eta ();
   jetphi3_ = v_f_quarks.at (2).Phi ();
   jetmass3_ = v_f_quarks.at (2).M ();
  }

  if (v_f_quarks.size()>3) {
   jetpt4_ = v_f_quarks.at (3).Pt ();
   jeteta4_ = v_f_quarks.at (3).Eta ();
   jetphi4_ = v_f_quarks.at (3).Phi ();
   jetmass4_ = v_f_quarks.at (3).M ();
  }

  if (v_f_quarks.size()>4) {
   jetpt5_ = v_f_quarks.at (4).Pt ();
   jeteta5_ = v_f_quarks.at (4).Eta ();
   jetphi5_ = v_f_quarks.at (4).Phi ();
   jetmass5_ = v_f_quarks.at (4).M ();
  }

  if (v_f_quarks.size()>5) {
   jetpt6_ = v_f_quarks.at (5).Pt ();
   jeteta6_ = v_f_quarks.at (5).Eta ();
   jetphi6_ = v_f_quarks.at (5).Phi ();
   jetmass6_ = v_f_quarks.at (5).M ();
  }

  for (unsigned int iq = 0; iq < v_f_quarks.size(); iq++){
   if (v_f_quarks.at(iq).Pt () > 30) njet_++;
  }

  //---- hww
  if (v_f_leptons.size()>0) {
   pt1_ = v_f_leptons.at (0).Pt ();
   eta1_ = v_f_leptons.at (0).Eta ();
   phi1_ = v_f_leptons.at (0).Phi ();
   mass1_ = v_f_leptons.at (0).M ();
  }
  if (v_f_leptons.size()>1) {
   pt2_ = v_f_leptons.at (1).Pt ();
   eta2_ = v_f_leptons.at (1).Eta ();
   phi2_ = v_f_leptons.at (1).Phi ();
   mass2_ = v_f_leptons.at (1).M ();
  }


  tree->Fill();
 }
}


void myTree::Write(TFile& out){
 out.cd();
 tree->Write();
}


int main (int argc, char **argv) { 
 // Open a stream connected to an event file:
 if (argc < 3) exit (1) ;

 std::cout << " Input  LHE  =" << argv[1] << std::endl;
 std::cout << " Output ROOT =" << argv[2] << std::endl;

 myTree myT;
 myT.fillTree (std::string(argv[1])) ;

 TFile output (argv[2], "recreate") ;
 output.cd() ;
 myT.Write(output);
 output.Close();

}



