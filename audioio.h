#ifndef __AUDIOIO_H__
#define __AUDIOIO_H__
// stops files being included more than once

// Use low-level C I/O so we can open pipes
#include <stdio.h>

#include <string>

//includes both stdio.h and string standard librarys

class AudioReader { //define the class name as AudioReader
public://define publicly accessable variables of the class
        AudioReader(std::string fname); // will supply a file constructor with a filename as input
        ~AudioReader();// make the destructor for the function public
        double get(void);// supply a get funciton with no stuff passed into.
        bool eof(void) { return eof_; };// provides an end of file getter to get the end of file. This is to stop external libraries setting eof_.

private:// define enxternaly inaccessable variables and/or functions
        FILE* pipe;// pointer to file data type
        static int constexpr bufmax {64};// sets a static integer on compile
        char rdbuf[bufmax];// assigns a list of charicters of length 64
        bool eof_ { false };// makes an end of file boolean and is initialised with false
};

class AudioWriter {
public:
        AudioWriter(std::string fname,int sr);
        ~AudioWriter();
        void write(double sample);
private:
        FILE* pipe;
        int samp_rate;
        int index;
};

#endif
