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
std::vector<int> databaseids = {0};
//---------------------------------------------------------


TChain* getChain(int databaseid){
    std::string fnames = "rootfiles/rootfile"+ std::to_string(databaseid) + ".txt";
    std::ifstream infile(fnames);

    TChain* ch = new TChain("chain");

    std::string fname;
    int bigcounter = 0;
    while((std::getline(infile, fname))){

        bigcounter += 1;
        std::cout<< "DB"<< databaseid << ":  " << bigcounter << " rootfiles added" << std::endl;

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
        if ((i % 1000000) == 0 ){
            std::cout << 100 * double(i)/double(n) << " % " << std::endl;
        }
        // Does the key exist?
        if (runs->find(RunNumber) != runs->end()){
            //key exists
            (*runs)[RunNumber] += 1;
        }else
        {
            //key does not exist
            (*runs)[RunNumber] = 1;
        }
    }

    //safe runmap for safety
    ofstream secure("runmaps/runmapdb"+std::to_string(databaseid)+".txt");
    for (auto [key, value]: *runs){
        secure << key << "," << value << std::endl;
    }
    return runs;
}

void overlap(){
    std::map<int, std::map<int, int>*> runmaps; //map containing the runmap of each single database as values and the databases as keys
    std::map<int,std::vector<int>> rundbs; //maps each run to the ids of databases containing this run
    for (int i: databaseids){
        std::map<int, int>* currentmap = runmap(i);
        runmaps[i] = currentmap;

        for (auto [key, value]: *currentmap){
            if(rundbs.find(key)!=rundbs.end()){
                rundbs[key].push_back(i);
            }else{
                std::vector<int> dbscurrentrun;
                dbscurrentrun.push_back(i);
                rundbs[key] =  dbscurrentrun;
            }
        }
        std::cout << "database " << i << " done." << std::endl ;
    }

    //start analysis
    std::cout << std::endl << std::endl;

    ofstream results("results/overlap.txt");

    for (auto [runid, dbs]: rundbs){
        if(dbs.size() > 1){
            //overlap existing !!
            std::cout << "overlap existing for run " << runid << " : " << std::endl;
            results << "overlap existing for run " << runid << " : " << std::endl;
            for(int i: dbs){
                std::cout << "DB " << i << " : " << (*runmaps[i])[runid] << "x" << std::endl;
                results << "DB " << i << " : " << (*runmaps[i])[runid] << "x" << std::endl;
            }
            std::cout << std::endl;
            results << std::endl;
        }
    }
}

