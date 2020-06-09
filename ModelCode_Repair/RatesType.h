#ifndef RATESTYPE_H
#define RATESTYPE_H

class RatesVal {
    
public:
    // the part after the colon is the equivalent of having
    //    age = age;
    //    ...
    // inside the {}
    RatesVal(double age, int id, double Rate, int d, double f, int indiv) : age(age), id(id), Rate(Rate), d(d), f(f), indiv(indiv)  {};
    
    double age;
    int id;
    double Rate;
    int d;
    double f;
    int indiv;
    
};


#endif
