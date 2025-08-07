#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <map>
#include <vector>
#include <set>

#include "TChain.h"

//---------------------------------------------------------
// Databases:
std::vector<int> databaseids = {0, 1};
//---------------------------------------------------------


TChain* getChain(int databaseid){
    std::string fnames = "testrootfile"+ std::to_string(databaseid) + ".txt";
    std::ifstream infile(fnames);

    TChain* ch = new TChain("chain");

    std::string fname;
    
    while(std::getline(infile, fname)){

        if(fname == "") continue;

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
    }
    return ch;
}


std::map<int, int>* runmap(int databaseid){

    TChain* ch = getChain(databaseid);
    int RunNumber;
    ch->SetBranchAddress("fRunNumber", &RunNumber);

    int n = ch->GetEntries();
    std::cout << "analysing database " << databaseid << ", " << n << " entries ... "<<std::endl;

    //map "runs" maps each RunNumber to the amount of times it appears in the database
    std::map <int, int>* runs = new std::map <int, int>;
    for (int i=0; i<n; i++){
        ch->GetEntry(i);
        
        // Does the key exist?
        if (runs->find(RunNumber) != runs->end()){
            //key exists
            (*runs)[RunNumber] += 1;
        }else
        {
            (*runs)[RunNumber] = 1;
        }
    }
    return runs;
}

void overlap(){
    std::vector<std::map<int, int>*> runmaps; //vector containing the runmap of each single database
    std::map<int,std::vector<int>> rundbs; //maps each run to the ids of databases containing this run
    for (int i=0; i<databaseids.size() ; i++){
        std::map<int, int>* currentmap = runmap(i);
        runmaps.push_back(currentmap);

        for (auto [key, value]: *currentmap){
            if(rundbs.find(key)!=rundbs.end()){
                rundbs[key].push_back(i);
            }else{
                std::vector<int> dbscurrentrun;
                dbscurrentrun.push_back(i);
                rundbs[key] =  dbscurrentrun;
            }
        }
        std::cout << "database " << databaseids[i] << " done." << std::endl ;
    }

    //start analysis
    std::cout << std::endl << std::endl;
    for (auto [runid, dbs]: rundbs){
        if(dbs.size() > 1){
            //overlap existing !!
            std::cout << "overlap existing for run " << runid << " : " << std::endl;
            for(int i=0; i<dbs.size(); i++){
                std::cout << "DB " << dbs[i] << " : " << (*runmaps[dbs[i]])[runid] << "x" << std::endl;
            }
            std::cout << std::endl;
        }
    }
}

