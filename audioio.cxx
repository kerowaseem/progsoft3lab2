#include "audioio.h"

#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>
// including standard ibraries and the header file for the class.
AudioReader::AudioReader(std::string fname) {//define audio reader class
        std::string cmd("sox '" + fname + "' -t dat -c 1 -");//creates the cmd string
        pipe = popen(cmd.c_str(), "r"); // executes the command string
        if (pipe == nullptr)//if no pipe
                throw std::runtime_error("Couldn't open audiofile");//throw error
        // The first two lines of the text supplied by sox state the number of
        // channels and the sample rate. Discard them.
        fgets(rdbuf, bufmax, pipe);//discard line 1
        fgets(rdbuf, bufmax, pipe);//discard line 2
}
        
AudioReader::~AudioReader() {
        if (pipe != nullptr)
                pclose(pipe);// if there is a pipe destroy it.
}
        
double AudioReader::get(void) {
        eof_ = (fgets(rdbuf, bufmax, pipe) == nullptr);//checks for end of file and also gets the next part of the pipe and sets it to rdbuf
        if (!eof_) {// if it is not the end of the file
                std::stringstream ss(rdbuf);//sent a string stream variable ss to the rdbuf char.
                double t, s;// create two doubles t and s
                ss >> t >> s;// split the stringstream into the time and the sample values
                return s;//return the sample
        } else
                return 0;//if end of file return 0
}

AudioWriter::AudioWriter(std::string fname, int sr){
        std::cout << "l1" << std::endl;
        samp_rate = sr;
        std::cout << "l2" << std::endl;
        index = 0;
        std::cout << "l3" << std::endl;
        std::string cmd("sox -t dat -c 1 -r " + std::to_string(samp_rate) + " - '" + fname + "'");
        std::cout << "l4" << std::endl;
        pipe = popen(cmd.c_str(), "w");
        std::cout << "l5" << std::endl;
        if (pipe == nullptr){
                throw std::runtime_error("Couldn't open audiofile");
        };
        std::cout << "l6" << std::endl;
        std::string writestring = ("; Sample Rate " + std::to_string(samp_rate));
        std::cout << "l7" << std::endl;
        fputs(writestring.c_str(), pipe);
        std::cout << "l8" << std::endl;
        writestring = "; Channels 1";
        std::cout << "l9" << std::endl;
        fputs(writestring.c_str(), pipe);
        std::cout << "l10" << std::endl;
};
AudioWriter::~AudioWriter() {
        if (pipe != nullptr){
                pclose(pipe);// if there is a pipe destroy it.
        };
               
}
void AudioWriter::write(double sample){
         
        double time = (double)index/(double)samp_rate;
        std::string timeIntensitySamp(std::to_string(time) + "  " + std::to_string(sample));
        std::cout << timeIntensitySamp << " " << index <<std::endl;
        fputs(timeIntensitySamp.c_str(), pipe);
        index +=1;


};

        
