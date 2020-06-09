

//performs damage/repair on the node "NodeIndex"
//and increments event time
void UpdateNode(Node &X, double &Time, double TotalRate, std::vector<DeficitVal> &DeficitsValues, int run, int id_track, std::vector<RatesVal> &RatesValues) {

    //perform event
    X.d = (X.d + 1)%2; //0->1, 1->0

    if (SimulateVar::MortalityOnly_bool == false)
	DeficitsValues.emplace_back(Time, X.id, X.d, run-1);
    
    if (X.id == id_track) {
        RatesValues.emplace_back(Time, X.id, X.Rate, X.d, X.f, run-1);
        std::cout << "Time is " << Time << " and d is " << X.d << " (UpdateNode)" <<std::endl;
    }
}
// Function added for repairing node that is almost identical to UpdateNode
void RepairNode(Node &X, double &Time, double TotalRate, std::vector<DeficitVal> &DeficitsValues, int run, int id_track, std::vector<RatesVal> &RatesValues) {
    
    //perform event
    X.d = 0;
    std::cout << "node has been repaired" << std::endl;
    
    if (SimulateVar::MortalityOnly_bool == false)
        DeficitsValues.emplace_back(Time, X.id, X.d, run-1);
    
    if (X.id == id_track) {
        RatesValues.emplace_back(Time, X.id, X.Rate, X.d, X.f, run-1);
        std::cout << "Repair time is " << Time << " and d is " << X.d << std::endl;
    }
}

void UpdateTime(double &Time, double TotalRate) {
    
    Time += TimeToEvent(TotalRate);
}

//update local frailty of a node after updating it
void UpdateLocalFrailty(std::vector<Node> &Network, Node &X) {

	double sum = 0;

	//update neighbours
	for(auto i: X.Neighbours) {

		sum += Network[i].d; 
		if(X.d == 0) Network[i].f -= 1.0/Network[i].k;
		if(X.d == 1) Network[i].f += 1.0/Network[i].k;

	} 

	//update this node
	X.f = sum/X.k;

}
