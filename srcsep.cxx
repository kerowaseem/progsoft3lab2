#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <list>
#include <stdexcept>
//#include <sstream>
#include "audioio.h"
#include "Fir1.h"
int main(int argc, char* argv[]){
    if (argc != 3){
        std::cerr << "usage: " << argv[0] << " [wet file] [dry file]"<< std::endl;
    };
    AudioReader wet(argv[1]);
    AudioReader dry(argv[2]);


    int sr(96000);
    float fl(0.3);
    float ss(4.9);
    float lrate(0.002);
    int nc(fl * sr);

    Fir1 fir(nc);
    fir.setLearningRate(lrate);
    int ns (sr*ss);
    std::list<double> wetList = std::list<double>();
    std::list<double> dryList = std::list<double>();
    for (int i = 0; i < nc; i++)
    {
      wet.get();  
    };

    for (int i = 0; i < nc; i++)
    {
      dry.get();  
    };
    
    double sample(1);
    while (sample != 0){
        sample = wet.get();
        wetList.push_back(sample);
    };
    sample = 1;
    while (sample != 0){
        sample = dry.get();
        dryList.push_back(sample);
    };
    //std::cout << wetList << std::endl;
    //std::cout << dryList << std::endl;
    return 0;
}






