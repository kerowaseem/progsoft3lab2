/** testreader
 * 
 * Read the named audio file and print it out sample by sample.
 */

#include "audioio.h"

#include <iostream>

int main(int argc, char* argv[])
{
        if (argc != 2) {
                std::cerr << "Usage: " << argv[0] << " audio_file\n";
                return 1;
        }
                
        AudioReader ar(argv[1]);
        
        int sample_count {0};
        
        while (!ar.eof())
                std::cout << sample_count++ << '\t' << ar.get() << std::endl;
}
