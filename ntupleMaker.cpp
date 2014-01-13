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
  float jetpt2_;
  float jeteta1_;
  float jeteta2_;
  float jetphi1_;
  float jetphi2_;
  float jetmass1_;
  float jetmass2_;


  float hbb_pt_;
  float hbb_phi_;
  float hbb_eta_;

  void fillTree (std::string fileNameLHE);
  void Write(TFile& out);
  void Init();

};


myTree::myTree(){
 tree = new TTree("tree","tree");

 tree->Branch("njet",&njet_,"njet/I");
 tree->Branch("jetpt1",&jetpt1_,"jetpt1/F");
 tree->Branch("jeteta1",&jeteta1_,"jeteta1/F");
 tree->Branch("jetphi1",&jetphi1_,"jetphi1/F");
 tree->Branch("jetmass1",&jetmass1_,"jetmass1/F");
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
// the sum pf the two quarks

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

  for (unsigned int iq = 0; iq < v_f_quarks.size(); iq++){
   if (v_f_quarks.at(iq).Pt () > 30) njet_++;
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



