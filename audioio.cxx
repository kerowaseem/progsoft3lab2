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
        samp_rate = sr;
        index = 0;
        std::string cmd("sox -t dat -c 1 -r " + std::to_string(samp_rate) + " - '" + fname + "'");
        pipe = popen(cmd.c_str(), "w");
        if (pipe == nullptr){
                throw std::runtime_error("Couldn't open audiofile");
        };
        std::string writestring("; Sample Rate " + samp_rate);
        fputs(writestring.c_str(), pipe);
        writestring = "; Channels 1";
        fputs(writestring.c_str(), pipe);
};
AudioWriter::~AudioWriter() {
        if (pipe != nullptr){
                pclose(pipe);// if there is a pipe destroy it.
        };
               
}
void AudioWriter::write(double sample){
        double time = index/samp_rate;
        std::string timeIntensitySamp(std::to_string(time) + "  " + std::to_string(sample));
        std::cout << timeIntensitySamp << std::endl; 
        fputs(timeIntensitySamp.c_str(), pipe);
        index +=1;


};

        
