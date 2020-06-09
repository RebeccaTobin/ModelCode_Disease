#pragma once
#include <vector>


//Time to an event given the rates
double TimeToEvent(double TotalRate) {
    
    using namespace Parameters;
    return -1.0/TotalRate * log(RNG(g1));

}


//repair rate given parameters and local frailty
double DamageRate(double f) {

    using namespace Parameters;
    return exp(gammap * f);

}

//Damage rate given parameters and local frailty
double RepairRate(double f) {

    using namespace Parameters;
    return 1/R * exp(-gamman * f);

}

//calculate and set the rate of a node and all of its neighbours, as well as the
//cumulative rate
void CalculateRates(std::vector<Node> &Network, Node &X, EventTree &tree, int j, double &TotalRate, int id_track, double Time, int run, std::vector<RatesVal> &RatesValues) {

    for(int i: X.Neighbours) {

	//remove current from total rate
        TotalRate -= Network[i].Rate;

        //update rates on the network
        if(Network[i].d == 0) Network[i].Rate = DamageRate(Network[i].f);
        
        else if(Network[i].d == 1) Network[i].Rate = RepairRate(Network[i].f); 

        //update rates on the tree
        tree.Update(i, Network[i].Rate);

        //update new total rate
        TotalRate += Network[i].Rate;
        
        if (i == id_track) {
            RatesValues.emplace_back(Time, Network[i].id, Network[i].Rate, Network[i].d, Network[i].f, run-1);
            // diagnostic tool:
            std::cout << "time is " << Time << " and d is " << Network[i].d << std::endl;
        }

    }

    //update the X node
    TotalRate -= X.Rate;
    if(X.d == 0) X.Rate = DamageRate(X.f); 
    else if(X.d == 1) X.Rate = RepairRate(X.f); 

    tree.Update(j, Network[j].Rate);

    TotalRate += X.Rate;

}

//finds rate and returns index of rate to be performed
int FindRate(EventTree &tree, double TotalRate) {

    double val = TotalRate * RNG(g1) ; //temp

    int i = tree.Search(val);
    assert(i < Parameters::N);
    
    return i;

}
