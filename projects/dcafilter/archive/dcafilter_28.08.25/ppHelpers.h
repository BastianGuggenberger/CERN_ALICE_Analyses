// define constants and data items and include ppHelpers (-> pph)
const int nmasses = 5;
const int nelemsMax = 8;

//const double acc_xy = 15.0e-03;
//const double acc_z = 12.5e-03;

#include "ppDataItems.h"
#include "ppDataItems.C"

class rnMaps
{
  public:
  
    rnMaps()
    {            
      // [0] fraction of accepted events
      rnmaps.push_back(new std::map<int, std::pair<float, float>>);
      // [1] Vx
      rnmaps.push_back(new std::map<int, std::pair<float, float>>);
      // [2] Vy
      rnmaps.push_back(new std::map<int, std::pair<float, float>>);
      // [3] Vz
      rnmaps.push_back(new std::map<int, std::pair<float, float>>);      
    }

    void add(int ind, int rnnum, float val)
    {
      auto m = (std::map<int, std::pair<float, float>>*) rnmaps[ind];
      auto vtmp = std::make_pair(0., 0.);
      if (m->find(rnnum) != m->end())
      {
        vtmp = (*m)[rnnum];
      }
      (*m)[rnnum] = std::make_pair(vtmp.first+1., vtmp.second+val);
    }
    
    std::map<int, float>* get(int ind)
    {
      auto vals = new std::map<int, float>;

      auto m = (std::map<int, std::pair<float, float>>*) rnmaps[ind];
      for (auto it = m->begin(); it != m->end(); ++it)
      {
        (*vals)[it->first] = ((*m)[it->first]).second/((*m)[it->first]).first;
      }
      
      return vals;
    }
    
    std::vector<std::map<int, std::pair<float, float>>*> rnmaps;
};


// handling of json file with configuration parameters
template<typename T>
struct is_std_vector : std::false_type {};

// Specialization for std::vector with any type and allocator
template<typename T, typename Allocator>
struct is_std_vector<std::vector<T, Allocator>> : std::true_type {};

// Helper variable template to simplify usage
template<typename T>
inline constexpr bool is_std_vector_v = is_std_vector<T>::value;

// Trait to detect if T has a nested value_type
template<typename T, typename = void>
struct has_value_type : std::false_type {};

template<typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

#include "rapidjson/document.h"
#include <fstream>
#include <type_traits>
#include <iostream>
#include <string>
class ppConfiguration
{
  public:
    ppConfiguration(){};

    ppConfiguration(TString jfname)
    {
      // open json file
      std::ifstream file(jfname.Data());
      if (!file.is_open()) {
        std::cerr << "Cannot open JSON file " << jfname << std::endl;
        return;
      }
      
      // read json file and set public variables
      std::stringstream buffer;
      buffer << file.rdbuf();
      std::string jsonStr = buffer.str();

      if (doc.Parse(jsonStr.c_str()).HasParseError()) {
        std::cerr << "JSON parse error" << std::endl;
        return;
      }
    }
    
    // to call cc do like
    //  auto val  = ppc->cc<int>("nPVmin") or
    //  auto vals = ppc->cc<std::vector<int>>("pids") or
    //  auto val  = ppc->cc<float>("FV0AMax")
    template<typename TV>
    TV cc(TString key)
    {
      if (doc.HasMember(key.Data())) {

        // an array
        if constexpr (has_value_type<TV>::value) {
          using ElementType = typename TV::value_type;
          TV arr;
          if (doc[key.Data()].IsArray()) { 
            auto vals = doc[key.Data()].GetArray();
            for (int ii=0; ii<vals.Size(); ++ii) {
              auto& val = vals[ii];
              arr.push_back(toVal<ElementType>(&val));
            }
          } else {
            auto& val = doc[key.Data()];
            arr.push_back(toVal<ElementType>(&val));
          }
          return arr;
        } else {
          auto& val = doc[key.Data()];
          return toVal<TV>(&val);
        }

      } else {
        std::cerr << "Configuration parameter " << key.Data() << " does not exist!" << std::endl;
        return TV{};
       }
    }

  private:
    rapidjson::Document doc;

    template<typename TV, typename EL>
    TV toVal(EL elem)
    {
      // Integer values
      if (elem->IsInt()) {
        auto val = elem->GetInt();
        if constexpr (std::is_integral_v<TV>) {
          return (TV) val;
        } else if constexpr (std::is_floating_point_v<TV>) {
          return (TV) val;
        } else if constexpr (std::is_same_v<TV, TString>) {
          return Form("%d", val);

        } else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<TV>>, std::string>) {
          return std::to_string(val);
        }
      }
      
      // Float values
      if (elem->IsFloat()) {
        auto val = elem->GetDouble();
        if constexpr (std::is_integral_v<TV>) {
          return (TV) val;
        } else if constexpr (std::is_floating_point_v<TV>) {
          return (TV) val;
        } else if constexpr (std::is_same_v<TV, TString>) {
          return Form("%f", val);
        } else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<TV>>, std::string>) {
          return std::to_string(val);
        }
      }

      // String values
      if (elem->IsString()) { 
        auto val = elem->GetString();
        if constexpr (std::is_integral_v<TV>) {
          return (TV)std::stoi(val);
        } else if constexpr (std::is_floating_point_v<TV>) {
          return (TV)std::stoi(val);
        } else if constexpr (std::is_same_v<TV, TString>) {
          return Form("%s", val);
        } else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<TV>>, std::string>) {
          return (std::string)val;
        }
      }
      
      std::cerr << "Configuration parameter of unklnown type!" << std::endl;
      return TV{};
    }
};

class ppHelpers
{
  public:
    ppHelpers(){};

    // some constants
    int nbIVM = 4000;
    float bIVMmin = 0.;
    float bIVMmax = 5.;
    
    int nbp = 1000;
    float bpmin = 0.;
    float bpmax = 5.;
    
    int nbnS = 6000;
    float bnSmin = -30.;
    float bnSmax = 30.;

    TChain* getChain(std::string fnames)
    {
      // open file with list of root files
      ifstream infile;
      infile.open(fnames.c_str());

      // create a TChain
      TChain* ch = new TChain("twotauchain");
      std::string fname;
      std::getline(infile, fname);
      while (fname != "")
      {
        // open root file
        TFile ff(fname.c_str(),"READ");

        // update the TChain
        // loop over all directories
        for (auto key : *ff.GetListOfKeys())
        {
          auto dname = std::string(((TKey*) key)->GetName());
          if (!dname.starts_with("DF")) continue;
          
          auto tname = fname+"/"+std::string(((TKey*) key)->GetName())+"/O2dgfewtrack";
          ch->Add(tname.c_str());
        }
        ff.Close();
        std::getline(infile, fname);
      }
      infile.close();
  
      // print number of trees and number of events
      std::cout << "Number of trees   : " << ch->GetNtrees()  << std::endl;
      std::cout << "Number of events  : " << ch->GetEntries() << std::endl;
      
      // assign branches
#include "ppBranchAssignment.h"

      // inspect branches
      auto branches = ch->GetListOfBranches();
      std::cout << "Number of branches: " << branches->GetEntries() << std::endl;
      for (auto obj : *branches) {
        auto bname = ((TObjString*)obj)->GetName();
        auto branch = ch->GetBranch(bname);
        std::cout << "  Branch " << bname << " of type " << branch->GetLeaf(bname)->GetTypeName() << "[" <<  branch->GetLeaf(bname)->GetLen() << "]" << std::endl;
      }

      return ch;
    };

    // prepare all histograms
    void getHistos(std::vector<TH1D*> &hs1d, std::vector<TH2D*> &hs2d)
    {
      // 1D ========================================================================
      // [0]
      hs1d.push_back(new TH1D("evselbits", ";event selection bit;Number of events", 10, -0.5, 9.5));
      // [1]
      hs1d.push_back(new TH1D("ULS IVM", ";IVM [GeV/c^{2}];Number of events", nbIVM, bIVMmin, bIVMmax));
      // [2]
      hs1d.push_back(new TH1D("LS IVM", ";IVM [GeV/c^{2}];Number of events", nbIVM, bIVMmin, bIVMmax));
      // [3]
      hs1d.push_back(new TH1D("Vx", ";V_{x};Number of events", 400, -0.1, 0.1));
      // [4]
      hs1d.push_back(new TH1D("Vy", ";V_{y};Number of events", 400, -0.1, 0.1));
      // [5]
      hs1d.push_back(new TH1D("Vz", ";V_{z};Number of events", 600, -30., 30.));
      // [6]
      hs1d.push_back(new TH1D("Occupancy", ";Occupancy;Number of events", 60, -10., 50.));
      // [7]
      hs1d.push_back(new TH1D("FT0AmplitudeA", ";FT0A amplitude;Number of events", 60, -10., 50.));
      // [8]
      hs1d.push_back(new TH1D("FT0AmplitudeC", ";FT0C amplitude;Number of events", 60, -10., 50.));
      // [9]
      hs1d.push_back(new TH1D("FV0AmplitudeA", ";FV0A amplitude;Number of events", 60, -10., 50.));
      // [10]
      hs1d.push_back(new TH1D("FT0ATime", ";FT0A time;Number of events", 240., -10., 50.));
      // [11]
      hs1d.push_back(new TH1D("FT0CTime", ";FT0C time;Number of events", 240., -10., 50.));
      // [12]
      hs1d.push_back(new TH1D("FV0ATime", ";FV0A time;Number of events", 240., -10., 50.));

      // 2D ========================================================================
      // [0]
      hs2d.push_back(new TH2D("Number of contributors versus number of tracks", "Number of events;Number of contributors;Number of tracks", 100, 0., 100., 100, 0., 100.));
      // [1]
      hs2d.push_back(new TH2D("Vertex position", "Number of events; V_{z} [#mum]; V_{T} [cm]", 200, -25., 25., 200, -0.15, 0.15));

      // nsigma verus momentum
      // [2]
      hs2d.push_back(new TH2D("nSigma_TPC_el", "Number of events; track momentum [GeV/c]; n#sigma_{TPC, el}", nbp, bpmin, bpmax, nbnS, bnSmin, bnSmax));
      // [3]
      hs2d.push_back(new TH2D("nSigma_TPC_pi", "Number of events; track momentum [GeV/c]; n#sigma_{TPC, pi}", nbp, bpmin, bpmax, nbnS, bnSmin, bnSmax));
      // [4]
      hs2d.push_back(new TH2D("nSigma_TPC_ka", "Number of events; track momentum [GeV/c]; n#sigma_{TPC, ka}", nbp, bpmin, bpmax, nbnS, bnSmin, bnSmax));
      // [5]
      hs2d.push_back(new TH2D("nSigma_TPC_pr", "Number of events; track momentum [GeV/c]; n#sigma_{TPC, pr}", nbp, bpmin, bpmax, nbnS, bnSmin, bnSmax));
      // [6]
      hs2d.push_back(new TH2D("nSigma_TOF_el", "Number of events; track momentum [GeV/c]; n#sigma_{TOF, el}", nbp, bpmin, bpmax, nbnS, bnSmin, bnSmax));
      // [7]
      hs2d.push_back(new TH2D("nSigma_TOF_pi", "Number of events; track momentum [GeV/c]; n#sigma_{TOF, pi}", nbp, bpmin, bpmax, nbnS, bnSmin, bnSmax));
      // [8]
      hs2d.push_back(new TH2D("nSigma_TOF_ka", "Number of events; track momentum [GeV/c]; n#sigma_{TOF, ka}", nbp, bpmin, bpmax, nbnS, bnSmin, bnSmax));
      // [9]
      hs2d.push_back(new TH2D("nSigma_TOF_pr", "Number of events; track momentum [GeV/c]; n#sigma_{TOF, pr}", nbp, bpmin, bpmax, nbnS, bnSmin, bnSmax));

      // IVM plots
      // [10]
      hs2d.push_back(new TH2D("IVMpT", "Number of events; IVM [GeV/c^{2}]; Pair p_{T} [GeV/c]", nbIVM, bIVMmin, bIVMmax, nbp, bpmin, bpmax));
      // [11]
      hs2d.push_back(new TH2D("IVMtrpT", "Number of events; IVM [GeV/c^{2}]; Track p_{T} [GeV/c]", nbIVM, bIVMmin, bIVMmax, nbp, bpmin, bpmax));
      // [12]
      hs2d.push_back(new TH2D("IVMVsdcaxy", "Number of events; IVM [GeV/c^{2}]; track dca_{xy} [#mum]", nbIVM, bIVMmin, bIVMmax, 200, 0., 200.));
      // [13]
      hs2d.push_back(new TH2D("IVMVsdcaz", "Number of events; IVM [GeV/c^{2}]; track dca_{z} [#mum]", nbIVM, bIVMmin, bIVMmax, 200, 0., 200.));
      // [14]
      hs2d.push_back(new TH2D("IVMVsaco", "Number of events; IVM [GeV/c^{2}]; acoplanarity", nbIVM, bIVMmin, bIVMmax, 200, 0., 1.));
      
      // tracks
      // [15]
      hs2d.push_back(new TH2D("etaVspTall", "Number of events; #eta; track p_{T} [GeV/c]", 240, -1.2, 1.2, nbp, bpmin, bpmax));
      // [16]
      hs2d.push_back(new TH2D("etaVspTwTOF", "Number of events; #eta; track_{with TOF} p_{T} [GeV/c]", 240, -1.2, 1.2, nbp, bpmin, bpmax));
      
      // dca xy vs z
      // [17]
      hs2d.push_back(new TH2D("dcazVsdcaxy", "Number of events; track dca_{z} [#mum]; track dca_{xy} [#mum]", 200, 0., 200. ,200, 0., 200.));
      
      // configure the display of the histograms
      for (auto h : hs1d)
      {
        h->SetTitle("");
        h->SetStats(0);
        h->SetLineColor(c2u[5]);
      }
      for (auto h : hs2d)
      {
        h->SetTitle("");
        h->SetStats(0);
      }
    }

    // plot the default histograms
    void plotHistograms(TCanvas* cv, TString annText, std::vector<TH1D*> &hs1d, std::vector<TH2D*> &hs2d, std::string pdfname, double IVMmin, double IVMmax)
    {
      TLatex txt;
      txt.SetTextSize(0.03);
      txt.SetTextAlign(32);

      // display the 1D histograms

      hs1d[1]->GetXaxis()->SetRangeUser(IVMmin,IVMmax);

      // linear scale
      cv->SetLogy(false);
      //loop for more plots
        hs1d[1]->Draw("HIST");
        txt.DrawLatexNDC(0.85,0.97,annText);
        cv->Update(); cv->SaveAs(pdfname.data());
    
      // log scale
      cv->SetLogy(true);
      //loop for more plots
        hs1d[1]->Draw("HIST");
        txt.DrawLatexNDC(0.85,0.97,annText);
        cv->Update(); cv->SaveAs(pdfname.data());
        
    
      // plot 2D histograms
      cv->SetLogy(false);
      cv->SetLogz(true);
      //loop for more plots
        hs2d[17]->Draw("HIST");
        txt.DrawLatexNDC(0.85,0.97,annText);
        cv->Update(); cv->SaveAs(pdfname.data());
      
    }

    // is track compatible with electron hypothesis?
    bool isElectron(int ind, std::vector<float> lsTPCmin, std::vector<float> lsTPCmax, std::vector<float> lsTOFmin, std::vector<float> lsTOFmax)
    {

      // is electron
      if (TrkTPCnSigmaEl[ind] < lsTPCmin[0] || TrkTPCnSigmaEl[ind] > lsTPCmax[0]) return false;

      // no muon, pion
      if (TrkTPCnSigmaMu[ind] > lsTPCmin[1] && TrkTPCnSigmaMu[ind] < lsTPCmax[1]) return false;
      if (TrkTPCnSigmaPi[ind] > lsTPCmin[2] && TrkTPCnSigmaPi[ind] < lsTPCmax[2]) return false;
           
      auto hasTOF = TrkTOFexpMom[ind] > 0.;
      if (hasTOF) {
        // no muon, pion, kaon, proton
        if (TrkTOFnSigmaMu[ind] > lsTOFmin[1] && TrkTOFnSigmaMu[ind] < lsTOFmax[1]) return false;
        if (TrkTOFnSigmaPi[ind] > lsTOFmin[2] && TrkTOFnSigmaPi[ind] < lsTOFmax[2]) return false;
        if (TrkTOFnSigmaKa[ind] > lsTOFmin[3] && TrkTOFnSigmaKa[ind] < lsTOFmax[3]) return false;
        if (TrkTOFnSigmaPr[ind] > lsTOFmin[4] && TrkTOFnSigmaPr[ind] < lsTOFmax[4]) return false;
      }

      return true;
    }

    // is track compatible with muon hypothesis?
    bool isMuon(int ind, std::vector<float> lsTPCmin, std::vector<float> lsTPCmax, std::vector<float> lsTOFmin, std::vector<float> lsTOFmax)
    {
      // is muon
      if ( TrkTPCnSigmaMu[ind] < lsTPCmin[1] || TrkTPCnSigmaMu[ind] > lsTPCmax[1] ) return false;

      // not electron
      if ( TrkTPCnSigmaEl[ind] > lsTPCmin[0] && TrkTPCnSigmaEl[ind] < lsTPCmax[0] ) return false;
      
      auto hasTOF = TrkTOFexpMom[ind] > 0.;
      if (hasTOF) {
        // is muon
        if ( TrkTOFnSigmaMu[ind] < lsTOFmin[1] || TrkTOFnSigmaMu[ind] > lsTOFmax[1] ) return false;
        
        // no electron, kaon, proton
        if ( TrkTOFnSigmaEl[ind] > lsTOFmin[0] && TrkTOFnSigmaEl[ind] < lsTOFmax[0] ) return false;
        if ( TrkTOFnSigmaKa[ind] > lsTOFmin[3] && TrkTOFnSigmaKa[ind] < lsTOFmax[3] ) return false;
        if ( TrkTOFnSigmaPr[ind] > lsTOFmin[4] && TrkTOFnSigmaPr[ind] < lsTOFmax[4] ) return false;
      }

      return true;
    }

    // is track compatible with pion hypothesis?
    bool isPion(int ind, std::vector<float> lsTPCmin, std::vector<float> lsTPCmax, std::vector<float> lsTOFmin, std::vector<float> lsTOFmax)
    {
      // is pion
      if ( TrkTPCnSigmaPi[ind] < lsTPCmin[2] || TrkTPCnSigmaPi[ind] > lsTPCmax[2] ) return false;

      // not electron
      if ( TrkTPCnSigmaEl[ind] > lsTPCmin[0] && TrkTPCnSigmaEl[ind] < lsTPCmax[0] ) return false;
      
      auto hasTOF = TrkTOFexpMom[ind] > 0.;
      if (hasTOF) {
        // is pion
        if ( TrkTOFnSigmaPi[ind] < lsTOFmin[2] || TrkTOFnSigmaPi[ind] > lsTOFmax[2] ) return false;
        
        // no electron, kaon, proton
        if ( TrkTOFnSigmaEl[ind] > lsTOFmin[0] && TrkTOFnSigmaEl[ind] < lsTOFmax[0] ) return false;
        if ( TrkTOFnSigmaKa[ind] > lsTOFmin[3] && TrkTOFnSigmaKa[ind] < lsTOFmax[3] ) return false;
        if ( TrkTOFnSigmaPr[ind] > lsTOFmin[4] && TrkTOFnSigmaPr[ind] < lsTOFmax[4] ) return false;
      }

      return true;
    }

    // is track compatible with kaon hypothesis?
    bool isKaon(int ind, std::vector<float> lsTPCmin, std::vector<float> lsTPCmax, std::vector<float> lsTOFmin, std::vector<float> lsTOFmax)
    {
      // is kaon
      if (TrkTPCnSigmaKa[ind] < lsTPCmin[3] || TrkTPCnSigmaKa[ind] > lsTPCmax[3]) return false;
    
      // not proton
      //if (TrkTPCnSigmaPr[ind] > lsTPCmin[4] && TrkTPCnSigmaPr[ind] < lsTPCmax[4]) return false;
      
      auto hasTOF = TrkTOFexpMom[ind] > 0.;
      if (hasTOF) {
        // is kaon
        if ( TrkTOFnSigmaKa[ind] < lsTOFmin[3] || TrkTOFnSigmaKa[ind] > lsTOFmax[3] ) return false;
        
        // no electron, muon, pion, proton
        if (TrkTOFnSigmaEl[ind] > lsTOFmin[0] && TrkTOFnSigmaEl[ind] < lsTOFmax[0]) return false;
        if (TrkTOFnSigmaMu[ind] > lsTOFmin[1] && TrkTOFnSigmaMu[ind] < lsTOFmax[1]) return false;
        if (TrkTOFnSigmaPi[ind] > lsTOFmin[2] && TrkTOFnSigmaPi[ind] < lsTOFmax[2]) return false;
        if (TrkTOFnSigmaPr[ind] > lsTOFmin[4] && TrkTOFnSigmaPr[ind] < lsTOFmax[4]) return false;
      }

      return true;
    }

    // is track compatible with proton hypothesis?
    bool isProton(int ind, std::vector<float> lsTPCmin, std::vector<float> lsTPCmax, std::vector<float> lsTOFmin, std::vector<float> lsTOFmax)
    {
      // is no kaon
      if (TrkTPCnSigmaKa[ind] > lsTPCmin[3] && TrkTPCnSigmaKa[ind] < lsTPCmax[3]) return false;
    
      // is proton
      if (TrkTPCnSigmaPr[ind] < lsTPCmin[4] || TrkTPCnSigmaPr[ind] > lsTPCmax[4]) return false;
      
      
      auto hasTOF = TrkTOFexpMom[ind] > 0.;
      if (hasTOF) {
        // is proton
        if ( TrkTOFnSigmaPr[ind] < lsTOFmin[4] || TrkTOFnSigmaPr[ind] > lsTOFmax[4] ) return false;
        
        // no electron, muon, pion, kaon
        if (TrkTOFnSigmaEl[ind] > lsTOFmin[0] && TrkTOFnSigmaEl[ind] < lsTOFmax[0]) return false;
        if (TrkTOFnSigmaMu[ind] > lsTOFmin[1] && TrkTOFnSigmaMu[ind] < lsTOFmax[1]) return false;
        if (TrkTOFnSigmaPi[ind] > lsTOFmin[2] && TrkTOFnSigmaPi[ind] < lsTOFmax[2]) return false;
        if (TrkTOFnSigmaKa[ind] > lsTOFmin[3] && TrkTOFnSigmaKa[ind] < lsTOFmax[3]) return false;
      }

      return true;
    }

    // check single track to have proper PID
    bool isGoodTrack(int ind, int hypo)
    {
      switch(hypo)
      {
        case 1:
          return isElectron(ind, std::vector<float>{-2.,-20.,-20., -3., -3.},
                                 std::vector<float>{ 3.,  3.,  3.,  6., 20.},
                                 std::vector<float>{-3., -3., -3., -3., -3.},
                                 std::vector<float>{ 2.,  3.,  3.,  3.,  3.}
          );
          break;
        case 2:          
          return isMuon(ind, std::vector<float>{-2., -3., -3., -3., -3.},
                             std::vector<float>{20.,  3.,  2.,  6., 20.},
                             std::vector<float>{-3., -2., -2., -3., -3.},
                             std::vector<float>{ 2.,  3.,  3.,  3.,  3.}
          );
          break;
        case 3:          
          return isPion(ind, std::vector<float>{-2., -3., -3., -3., -3.},
                             std::vector<float>{20.,  3.,  2.,  6., 20.},
                             std::vector<float>{-3., -2., -2., -3., -3.},
                             std::vector<float>{ 2.,  3.,  3.,  3.,  3.}
          );
          break;
        case 4:
          return isKaon(ind, std::vector<float>{-3., -3., -3., -3., -3.},
                             std::vector<float>{ 3.,  3.,  3.,  3.,  3.},
                             std::vector<float>{-3., -3., -3., -3., -3.},
                             std::vector<float>{ 3.,  3.,  3.,  3.,  3.}
          );
          break;
        case 5:
          return isProton(ind, std::vector<float>{-3., -3., -3., -3., -3.},
                               std::vector<float>{ 3.,  3.,  3.,  3.,  3.},
                               std::vector<float>{-3., -3., -3., -3., -3.},
                               std::vector<float>{ 3.,  3.,  3.,  3.,  3.}
          );
          break;
        default:
          return true;
          break;
      }
    }

    // check track pair to have proper PID
    bool isGoodTuple(ppConfiguration* ppc, std::vector<float> &tmasses, bool check = false)
    {
      
      // for the moment we are expecting 2 tracks
      // ULS or LS
      // if (TrkSign[0]*TrkSign[1] >= 0) return false;
      // if (TrkSign[0]*TrkSign[1] <  0) return false;

      // pT and eta of tracks
      auto pTLims = ppc->cc<std::vector<float>>("pTLims");
      auto etaLims = ppc->cc<std::vector<float>>("etaLims");
      for (int ii=0; ii<2; ++ii)
      {
        auto v = ROOT::Math::XYZVector(TrkPx[ii], TrkPy[ii], TrkPz[ii]);
        if (v.Rho()<pTLims[0] || v.Rho()>pTLims[1]) return false;
        if (v.eta()<etaLims[0] || v.eta()>etaLims[1]) return false;
      }      
      
      // require TOF hits
      // at least one track with TOF
      float fwTOF = 0.;
      for (int ii=0; ii<2; ++ii) fwTOF += (TrkTOFexpMom[ii] > 0 ? 1. : 0.);
      if (fwTOF/2. < ppc->cc<float>("fwTOF")) return false;
                  
      // dcaxy and acoplanarity
      auto dcaxy = 1.E3 * (TrkDCAxy[0] > TrkDCAxy[1] ? TrkDCAxy[0] : TrkDCAxy[1]);
      auto dcaz  = 1.E3 * (TrkDCAz[0] > TrkDCAz[1] ? TrkDCAz[0] : TrkDCAz[1]);
      auto aco = acoplanarity();

      /*
      if (dcaxy > 20.) return false;
      if (dcaz  > 20.) return false;
      if (aco > 0.9) return false;
      */

      // PID
      std::vector<int> hypos = ppc->cc<std::vector<int>>("pids");
      tmasses.clear();
      if (check)
      {
        if (isGoodTrack(0, hypos[0]) && isGoodTrack(1, hypos[1]))
        {
          tmasses.push_back(pmasses[std::abs(hypos[0])-1]);
          tmasses.push_back(pmasses[std::abs(hypos[1])-1]);
        } else {
          if (isGoodTrack(1, hypos[0]) && isGoodTrack(0, hypos[1])) {
            tmasses.push_back(pmasses[std::abs(hypos[1])-1]);
            tmasses.push_back(pmasses[std::abs(hypos[0])-1]);
          } else {
            return false;
          }
        }
      } else {
        tmasses.push_back(pmasses[std::abs(hypos[0])-1]);
        tmasses.push_back(pmasses[std::abs(hypos[1])-1]);
      }

      return true;
    }

    // check event
    bool isGoodEvent(ppConfiguration* ppc, double alpha, double acc_xy, double acc_z)
    {
      if (NumContrib < ppc->cc<int>("nPVmin") || NumContrib > ppc->cc<int>("nPVmax")) return false;
      
      // event selection bits
      //if (Trs != 1) return false;
      //if (Trofs != 1) return false;
      //if (Hmpr != 1) return false;
      //if (Tfb != 1) return false;
      //if (ItsRofb != 1) return false;
      //if (Sbp != 1) return false;
      //if (ZvtxFT0vsPv == 1) return false;
      //if (VtxITSTPC != 1) return false;

      // Occupancy
      //if (OccupancyInTime > 0.) return false;
      
      // TotalFT0Amplitude[A,C]
      if (TotalFT0AmplitudeA > ppc->cc<float>("FT0AMax") ||
          TotalFT0AmplitudeC > ppc->cc<float>("FT0CMax") ||
          TotalFV0AmplitudeA > ppc->cc<float>("FV0AMax")
      ) return false;
      
      //dca filter
      for (int ii=0; ii<NumContrib; ii++){
        double c = 1.0/(acc_xy*acc_xy); //acc_xy represents 1. half axis of the ellipse
        double d = 1.0/(acc_z*acc_z); //acc_z represents 2. half axis of the ellipse
        if(c * (TrkDCAxy[ii]*TrkDCAxy[ii]) + d * (TrkDCAz[ii]*TrkDCAz[ii]) > alpha){
            return false;
          }
      }
            
      return true;
    }

    // return the most probable mass
    bool getMasses(int nTrks, float nslim, std::vector<float> &tmasses)
    {
      const int nmasses = 5;
      float pmasses[nmasses] = {0.000510998902, 0.1056583745, 0.13957039, 0.493677, 0.93827208816};

      // assume the hypothesis with the lowest nSigma_[TPC, TOF]
      tmasses.clear();
      for (int ii=0; ii<nTrks; ii++)
      {
        std::vector<float> nss = {std::abs(TrkTPCnSigmaEl[ii]), std::abs(TrkTPCnSigmaMu[ii]), std::abs(TrkTPCnSigmaPi[ii]), std::abs(TrkTPCnSigmaKa[ii]), std::abs(TrkTPCnSigmaPr[ii]), std::abs(TrkTOFnSigmaEl[ii]), std::abs(TrkTOFnSigmaMu[ii]), std::abs(TrkTOFnSigmaPi[ii]), std::abs(TrkTOFnSigmaKa[ii]), std::abs(TrkTOFnSigmaPr[ii])};
       auto minns = std::min_element(nss.begin(), nss.end());
       int index = std::distance(nss.begin(), minns);
       if (nss[index] > nslim) return false;
       tmasses.push_back(pmasses[index % nmasses]);
      }
      return true;
    }
    
    float acoplanarity()
    {
      float aco = -999.;
      auto v1 = TVector3(TrkPx[0], TrkPy[0], TrkPz[0]);
      auto v2 = TVector3(TrkPx[1], TrkPy[1], TrkPz[1]);
      auto phi1 = v1.Phi();
      auto phi2 = v2.Phi();

      auto dphi = std::fabs(TVector2::Phi_mpi_pi(phi1 - phi2));
      if (dphi > TMath::Pi()) dphi = 2*TMath::Pi() - dphi;

      aco = 1.0 - dphi / TMath::Pi();
      
      return aco;
    }
};

