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
#include "FindRepairNode.h"

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
    
    // Get IDs of nodes with degree inputed
    // CODE ONLY WORKS FOR SINGLE TOPOLOGY RIGHT NOW
    std::vector<int> IDs = FindIDs(Network);
    
    
    // Get the repair_id
    //int repair_id = Parameters::repair_id;
    
    //loop through runs to average data
    for(int run = 1; run <= Number; run++) {

	
	if(run == 1) g1.seed(RealSeed); // Set current seed

       	//Set up network for every seed if not single topology 
    if(Topology != "Single") {
	    SetUpNetwork(Network, MortalityNodes, OriginalN);
        //std::vector<int> IDs = FindID(Network, 37); // I think this should go here??
    }
        
    // Pick a repair node
    int repair_id = RandomID(IDs);
        
	EventTree tree(N, 1.0); 
	double TotalRate = N; //sum of rates
	double Time = 0; //current Time (unscaled age)
        
    bool IsDamaged = false;
    bool RepairComplete = false;
    double FirstDamageTime;
        
    //evolve in time until Mortality occurs
	while(true) {
        
        UpdateTime(Time, TotalRate); // Update the time
        
        // If the node to be repaired was damaged, has yet to be repaired, and enough time has passed since it was damaged initially,
        if (IsDamaged == true && RepairComplete == false && Time > (FirstDamageTime + Parameters::tau)) {
            // repair the node and record info in "Rates..." file,
            RepairNode(Network[repair_id], Time, TotalRate, DeficitsValues, run, repair_id, RatesValues);
            // do the usual updates after a node's d has been changed, 
            UpdateLocalFrailty(Network, Network[repair_id]);
            CalculateRates(Network, Network[repair_id], tree, repair_id, TotalRate, repair_id, Time, run, RatesValues);
            // and change bool so that repair happens only once
            RepairComplete = true;
        }
        
        else {
        int Index = FindRate(tree, TotalRate); //Find rate to be performed
        UpdateNode(Network[Index], Time, TotalRate, DeficitsValues, run, repair_id, RatesValues); //perform rate
        
        // If the note to be repaired has been updated and has been damaged for the first time,
        if (Network[Index].id == repair_id && Network[Index].d == 1 && IsDamaged == false) {
            // then record the time of first damage
            FirstDamageTime = Time;
            // and change IsDamaged to true so that no later times are recorded
            IsDamaged = true;
            std::cout << "First damage time is " << FirstDamageTime << " and d is " << Network[Index].d << std::endl;
        }
        
        UpdateLocalFrailty(Network, Network[Index]); //update the local frailty after the node is modified
        
        CalculateRates(Network, Network[Index], tree, Index, TotalRate, repair_id, Time, run, RatesValues); //calculate new rates for the node and its neighbours
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

