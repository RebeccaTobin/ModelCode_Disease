// Function that loops through all nodes and records the id's of all nodes that have degree k
#include <stdlib.h>

std::vector<int> FindIDs(std::vector<Node> &Network) {
    
    // Vector of node id's
    std::vector<int> IDs;
    
    int k = Parameters::repair_k;
    
    std::cout << "IDs of nodes with degree " << k << " are:" << std::endl;
    // loop over all nodes
    for(Node X: Network ) {
        // Add id's to vector if node has degree k
        if (X.k == k){
            IDs.push_back(X.id);
            std::cout << X.id << std::endl;
        }
    }
    
    if (IDs.empty() == true){
        std::cout << "There are no nodes with degree " << k << std::endl;
        std::cout << "Program aborted! Try another k" << std::endl;
        abort();
        // Trying to use recursion to make function more flexible, but I couldn't get it to work :,(
        //k -= 1;
        //std::cout << "Now looking for nodes of degree " << k << " (if no further messages than a node of degree " << k << "was found)" << std::endl;
        //IDs = FindID(Network, k);
    }
    
    return IDs;
}

// Function that takes vecotr of ID's and randomly picks one
// Should I pass IDs as a reference??
int RandomID(std::vector<int> IDs) {

    // Get a random index of vector
    int index = floor( RNG(g1)*IDs.size() );
    std::cout << "index chosen is " << index << std::endl;
    std::cout << "repair node chosen is " << IDs[index] << std::endl;
    
    return IDs[index];
}
