#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

std::mt19937 g1(1); //Random number generator, mersenne twister
std::uniform_real_distribution<double> RNG(0.0,1.0); 

#include "Parameters.h"
#include "Files.h"
#include "DeficitType.h"
#include "Node.h"
#include "DataFile.h" 
#include "OutputDataFile.h" 
#include "SaveData.h"
#include "2Dvector.h"
#include "RandomNetwork.h" 
#include "tree.h" 
#include "GillespieTree.h"
#include "UpdateNode.h"
#include "MortalityNodes.h"
#include "ScaleFreeNetworkTree.h" 
#include "MortalityRules.h"
#include "OutputNetwork.h"
#include "AssortativeMixing.h"
#include "SmallWorld.h"
#include "SetUpNetwork.h"
#include "RatesType.h"

int main(int argc, char *argv[]) {

    using namespace Parameters; //Using the parameters from cmd line
    SetParameters(argc, argv); //Set initial parameters from command line
    SetSimulate();
    std::cout << "Parameters Loaded" << std::endl;
    
    
    //Network
    const int OriginalN = N; //some network types remove unconnected nodes, lowering N. Original N will not change
    std::vector<Node> Network;
    std::vector<int> MortalityNodes(nd);

    
    //Hold all death ages found
    std::vector<double> DeathAges; DeathAges.reserve(Parameters::Number);

    
    //Holds the time of change, id of the node, and state of the node d
    std::vector<DeficitVal> DeficitsValues;
    if (SimulateVar::MortalityOnly_bool == false)
	DeficitsValues.reserve(N * 1000);
    
    std::vector<RatesVal> RatesValues;
    if (SimulateVar::MortalityOnly_bool == false)
        RatesValues.reserve(N * 1000);
    
    //Data file to save intermediate Data
    OutputDataFile File(OriginalN);
    if (SimulateVar::MortalityOnly_bool == false) File.Open(); 

    
    //set up network, if single topology set up network before the simulation
    if(Topology == "Single") {
	g1.seed(Parameters::SingleSeed);
	SetUpNetwork(Network, MortalityNodes, OriginalN);
	std::cout << "Single Network Created" << std::endl;
    }
    int id_track = 10;
    //loop through runs to average data
    for(int run = 1; run <= Number; run++) {

	
	if(run == 1) g1.seed(RealSeed); // Set current seed

       	//Set up network for every seed if not single topology 
	if(Topology != "Single") 
	    SetUpNetwork(Network, MortalityNodes, OriginalN);

	EventTree tree(N, 1.0); 
	double TotalRate = N; //sum of rates
	double Time = 0; //current Time (unscaled age)
	
    bool start_trigger = false;
    bool end_trigger = false;
        
	//evolve in time until Mortality occurs	
	while(true) {
        
        UpdateTime(Time, TotalRate);
        
        // Parameters::StartTime
        if (start_trigger == false && Time > Parameters::StartTime ) {
            //std::cout << Time << std::endl;
            CalculateRatesFull(Network, tree, TotalRate, id_track, Time, run, RatesValues, true);
            start_trigger = true;
        }
        
        // Parameters::EndTime
        if (end_trigger == false && Time > Parameters::EndTime ) {
            //std::cout << Time << std::endl;
            CalculateRatesFull(Network, tree, TotalRate, id_track, Time, run, RatesValues, false);
            end_trigger = true;
        }
        
	    int Index = FindRate(tree, TotalRate); //Find rate to be performed
        UpdateNode(Network[Index], Time, DeficitsValues, run, id_track, RatesValues); //perform rate and increase time
        // for now, just keeping track of node with id 0
	    UpdateLocalFrailty(Network, Network[Index]); //update the local frailty after the node is modified
        
        // Parameters::StartTime && Time < Parameters::EndTime
        if ( Time > Parameters::StartTime && Time < Parameters::EndTime ) {
            CalculateRates(Network, Network[Index], tree, Index, TotalRate, id_track, Time, run, RatesValues, true);
        }
        
        else {
            CalculateRates(Network, Network[Index], tree, Index, TotalRate, id_track, Time, run, RatesValues); //calculate new rates for the node and its neighbours
        }
        

	    //evaluate mortality
	    if(Mortality(Network, MortalityNodes) == 1) break;    
	    
	}
	
	//record death age data
	DeathAges.push_back(Time);
	
	//Reset Rates and fraility if it is single topology
	if(Topology == "Single")
	    for(auto &x: Network) x.Reset();  
      
	//Save interediate Data every 1000 individuals
	if(SimulateVar::MortalityOnly_bool == false && run%1000 == 0 ) { 
	    File.SaveData(DeficitsValues); //possibly change the 1000 (depends on memory restrictions)
        File.SaveData(RatesValues);
	    
	}
	
	if (run%1000 == 0)
	    std::cout << "Run " << run << std::endl;
	
    }
    
    std::cout << "Outputing final data" << std::endl;

    //Save and output final bit of raw Data
    if (SimulateVar::MortalityOnly_bool == false) {
	File.SaveData(DeficitsValues);
	File.FlushDataFile(DeficitsValues);
    File.SaveData(RatesValues);
    File.FlushDataFile(RatesValues);
    }
    
    OutputDeathAges(DeathAges, OriginalN);
    std::cout << "Finished" << std::endl;
    
}

