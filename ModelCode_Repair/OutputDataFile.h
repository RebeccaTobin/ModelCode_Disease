#include "RatesType.h"

class OutputDataFile : public DataFile {

public:

    using DataFile::DataFile; //Same constructor for names

    //save current data to temporary data files for later analysis
    void SaveData(std::vector<DeficitVal> &DeficitsValues);
    void SaveData(std::vector<RatesVal> &RatesValues);
    
    //close output file and remove memory used by vectors
    void FlushDataFile(std::vector<DeficitVal> &DeficitsValues) {

	DeficitsOut.close();

    };
    
    void FlushDataFile(std::vector<RatesVal> &RatesValues) {
        
        RatesOut.close();
        
    };

    void Open() { //Open the output file

        DeficitsOut.open(DeficitsName.c_str());
        RatesOut.open(RatesName.c_str());
	
    }

    std::ofstream DeficitsOut;
    std::ofstream RatesOut;

};
