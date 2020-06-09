#ifndef PARAMETERS_H
#define PARAMETERS_H


//All global parameters used.
//Initialized with cmdline arguments by SetParameters(argc, argv)
namespace Parameters {
	
    //Variable parameters
    double R; //Ratio of damage to repair
    int N; //Number of nodes in the network
    double gammap; //Damage rate constant
    double gamman; //repair rate constant
    int nd; //Number of mortality nodes
    int Number; //Number simulated individuals
    double alpha; //scale free distribution exponent
    int avgdeg; //average node degree 
    std::string MortalityCondition; //"OR" or "AND"
    std::string Topology; //"Random" generates a new sampled topology for each individual or "Single" for the same network for all simulated individuals
    std::string Folder; //"Local" uses the Data/ folder. Otherwise use scratch space on a computing cluster.
    std::string NetworkType; //"ScaleFree", or "Random", or "SmallWorld".
    int TaskID; //ID of this run with seed given
    int SingleSeed; //Seed used to make the single topology
    int RealSeed; //Seed used for simulating damage. Found from SeedFile and the TaskID.
    double p_assortativity; //if > 0, will modify the assortativity of the network. https://en.wikipedia.org/wiki/Assortativity
    
    // parameters for the disease
    double RateIncrease; // fractional increase in Gamma0 during disease
    double StartTime;  // at what age the disease starts
    double tau;  // length of disease
    
}	


//get parameters from cmd line and set them
void SetParameters(int argc, char *argv[]) {

	using namespace Parameters;

	Number = atoi(argv[1]); std::cout << "Number: " << Number;
	R = atof(argv[2]); std::cout << ", R: " << R;
	gammap = atof(argv[3]); std::cout << ", gammap: " << gammap;
	gamman = atof(argv[4]); std::cout << ", gamman: " << gamman;
	N = atoi(argv[5]); std::cout << ", N: " << N;
	nd = atoi(argv[6]); std::cout << ", nd: " << nd;
	alpha = atof(argv[7]); std::cout << ", alpha: " << alpha;
	avgdeg = atoi(argv[8]); std::cout << ", avgdeg: " << avgdeg;
	MortalityCondition = argv[9]; std::cout << ", MortalityCondition: " << MortalityCondition;
	Topology = argv[10]; std::cout << ", Topology: " << Topology;
	Folder = argv[11]; 
	NetworkType = argv[12]; std::cout << ", NetworkType: " << NetworkType;
	TaskID = atoi(argv[13]); std::cout << ", TaskID: " << TaskID;
	SingleSeed = atoi(argv[14]);  std::cout << ", SingleSeed: " << SingleSeed;
        p_assortativity = atof(argv[15]);  std::cout << ", p_assortativity: " << p_assortativity;
   
	std::cout << std::endl;
	std::cout << std::endl;
	

	std::ifstream SeedFile;
	SeedFile.open("SeedFile");
	int lineCounter = 0;
	int Seed;
	SeedFile >> Seed;
	while(lineCounter < TaskID) {

	    SeedFile >> Seed;
	    lineCounter++;
	    
	}
	
	std::cout << "Seed: " << Seed << std::endl;
	RealSeed = Seed;

	SeedFile.close();
	
	
}



namespace SimulateVar {

    bool MortalityOnly_bool = false;

}


//if you don't want deficit output, add "MortalityOnly" to "CommandFile_Simulate.txt".
void SetSimulate() {


    std::ifstream File;
    File.open("CommandFile_Simulate.txt");
    std::string line;

    while(std::getline(File, line)){

	std::cout << line << std::endl;

	if(line == "MortalityOnly") SimulateVar::MortalityOnly_bool = true;
	
    }
		
    File.close();

}


#endif
