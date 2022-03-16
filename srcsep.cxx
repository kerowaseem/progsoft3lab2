#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <math.h>
//#include <sstream>
#include "audioio.h"
#include "Fir1.h"
int main(int argc, char* argv[]){
    if (argc != 4){
        std::cerr << "usage: " << argv[0] << " [wet file] [dry file]"<< std::endl;
    };
    AudioReader wet(argv[1]);
    AudioReader dry(argv[2]);


    int sr(96000);
    float fl(0.3);
    float ss(4.9);
    float lrate(0.002);
    int nc(fl * sr);
    int fnum (1);
    int trial (2*sr); 

    Fir1 fir(nc);
    fir.setLearningRate(lrate);
    int ns (sr*ss);
    std::vector<double> wetVector = std::vector<double>();
    std::vector<double> dryVector = std::vector<double>();
    std::vector<double> error = std::vector<double>();
    for (int i = 0; i < nc; i++)
    {
      wet.get();  
    };

    for (int i = 0; i < nc; i++)
    {
      dry.get();  
    };
    
    while (!wet.eof()){
        wetVector.push_back(wet.get());
    };
    while (!dry.eof()){
        dryVector.push_back(dry.get());
    };
    if (wetVector.size() != dryVector.size()){
        std::cerr << "interference and source samples are different lengths"<< std::endl;
    }
    double msi(0);
    for (int i = 0; i < wetVector.size(); i++)
    {
        msi += pow(wetVector[i],2);
    }
    msi = msi / wetVector.size();
    int actTrial(trial < wetVector.size() ? trial : wetVector.size());

    // if (trial < wetVector.size()){
    //     actTrial = trial;
    // }else
    // {
    //     actTrial = wetVector.size();
    // }
    std::cout << "pre-training ..."<< std::endl;
    for (int i = 0; i < actTrial; i++)
    {
        fir.lms_update(wetVector[i] - fir.filter(dryVector[i]));
    }
    std::cout << "Processing" <<std::endl;
    for (int i = 0; i < dryVector.size(); i++)
    {
        if(i%1000 == 0){std::cout << i << std::endl;};
        error.push_back(wetVector[i] - fir.filter(dryVector[i]));
        fir.lms_update(error[i]);
    }
    double powerGain (0);
    for (int i = 0; i < error.size(); i++)
    {
        powerGain += pow(error[i],2);
    }
    powerGain = powerGain/error.size();
    powerGain = powerGain/msi;
    std::cout << "Power gain: " << powerGain << std::endl;
    AudioWriter FileWriter(argv[3], sr);
    std::cout << error.size() <<std::endl;
    for (int i = 0; i < error.size(); i++)
    {
        FileWriter.write(error[i]);
    }
    

    //std::cout << wetVector << std::endl;
    //std::cout << dryVector << std::endl;
    return 0;
}






