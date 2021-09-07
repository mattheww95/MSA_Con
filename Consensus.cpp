#include <iostream>
#include <fstream>
#include <vector>
#include <string>

typedef unsigned long long int __uint64;

struct nucleotides {
    /// From MSAIndex as well :^)
    __uint64 A = 0;
    __uint64 T = 0;
    __uint64 C = 0;
    __uint64 G = 0;
    __uint64 N = 0;
    __uint64 other = 0;
    __uint64 del = 0;
};



void switch_chars(std::string fasta, std::vector<nucleotides> &tracer) {
    // My own code from MSAIndex :^)
    // maybe proper to inline this later will see
    // Leaving as switch statements, as probably faster than applying upper first, need to check this though
    for (__uint64 i = 0; i < fasta.length(); i++) {
        switch (fasta[i])
        {
        case 'A':
            tracer[i].A += 1;
            break;
        case 'T':
            tracer[i].T += 1;
            break;
        case 'C':
            tracer[i].C += 1;
            break;
        case 'G':
            tracer[i].G += 1;
            break;
        case 'N':
            tracer[i].G += 1;
            break;
        case '-':
            tracer[i].del += 1;
            break;
        case 'a':
            tracer[i].A += 1;
            break;
        case 't':
            tracer[i].T += 1;
            break;
        case 'c':
            tracer[i].C += 1;
            break;
        case 'g':
            tracer[i].G += 1;
            break;
        case 'n':
            tracer[i].N += 1;
            break;
        default:
            tracer[i].other += 1;
            break;
        }
    }

}


void IUPAC_call(struct nucleotides cur_base){
    // Identify the IUPAC nucleotide for the mixture
    // Break the struct, probably smarter way to do this...
    // TODO clean this dumb logic up
    // TODO Make sure to use a plurality for when things are below 50% you know what i mean matthew
    //__uint64 A, T, C, G, N = cur_base.A, cur_base.T, cur_base.C, cur_base.G, cur_base.N;
    __uint64 base_freq[4] = {cur_base.A, cur_base.T, cur_base.C, cur_base.G};
    if(cur_base.A > 0 && cur_base.T > 0 && cur_base.C > 0 && cur_base.G > 0){
        std::cout << 'N';
    }else if(cur_base.A > 0 && cur_base.T == 0 && cur_base.C > 0 && cur_base.G > 0){
        std::cout << 'V';
    }else if(cur_base.A == 0 && cur_base.T > 0 && cur_base.C > 0 && cur_base.G > 0){
        std::cout << 'B';
    }else if (cur_base.A > 0 && cur_base.T > 0 && cur_base.C > 0 && cur_base.G == 0) {
        std::cout << 'H';
    }else if (cur_base.A > 0 && cur_base.T > 0 && cur_base.C == 0 && cur_base.G > 0) {
        std::cout << 'D';
    }else if (cur_base.A > 0 && cur_base.T == 0 && cur_base.C > 0 && cur_base.G == 0) {
        std::cout << 'M';
    }else if (cur_base.A == 0 && cur_base.T > 0 && cur_base.C == 0 && cur_base.G > 0) {
        std::cout << 'K';
    }else if (cur_base.A > 0 && cur_base.T > 0 && cur_base.C == 0 && cur_base.G == 0) {
        std::cout << 'W';
    }else if(cur_base.A == 0 && cur_base.T == 0 && cur_base.C > 0 && cur_base.G > 0) {
        std::cout << 'S';
    }else if(cur_base.A == 0 && cur_base.T > 0 && cur_base.C > 0 && cur_base.G == 0 ) {
        std::cout << 'Y';
    }else if (cur_base.A > 0 && cur_base.T == 0 && cur_base.C == 0 && cur_base.G > 0){
        std::cout << 'R';
    }else{
        for(__uint64 i = 0; i < 4; i++){
            if(base_freq[i] != 0){
                switch (i) {
                    case 0:
                        std::cout << 'A';
                        break;
                    case 1:
                        std::cout << 'T';
                        break;
                    case 2:
                        std::cout << 'C';
                        break;
                    case 3:
                        std::cout << 'G';
                        break;
                }

            }
        }

    }

}


int main( int argc, char **argv ){
    /// Code to get a single line fasta from -> http://www.rosettacode.org/wiki/FASTA_format#C.2B.2B
    /// Default output threshold is 0.5 make this a command line arg later
    if( argc <= 1 ){
        std::cerr << "Usage: "<<argv[0]<<" [infile]" << std::endl;
        std::cerr << "Default threshold for creating a consensus seqeunce is 0.5, this is set in the source code and will be a cmd option at another time" << std::endl;
        std::cerr << "Consensus sequence is output to stdout so capture it." << std::endl;
        return -1;
    }

    std::ifstream input(argv[1]);
    if(!input.good()){
        std::cerr << "Error opening '"<<argv[1]<<"'. Bailing out." << std::endl;
        return -1;
    }

    std::string line, name, content;


    // Initialize the vector!
    std::vector<nucleotides> nucs;
    bool is_vector_set = false;

    while( std::getline( input, line ).good() ){
        if( line.empty() || line[0] == '>' ){ // Identifier marker
            if( !name.empty() ){ // Print out what we read from the last entry
                //std::cout << name << " : " << content << std::endl;
                if(!is_vector_set){
                    for(__uint64 i = 0; i < content.length(); i++){
                        nucleotides nucs_;
                        nucs.push_back(nucs_);
                    }
                    is_vector_set = true;
                }else{
                    // Should probably make this the default if statement in the future
                    switch_chars(content, nucs);

                }
                
                name.clear();
            }
            if( !line.empty() ){
                name = line.substr(1);
            }
            content.clear();
        } else if( !name.empty() ){
            if( line.find(' ') != std::string::npos ){ // Invalid sequence--no spaces allowed
                name.clear();
                content.clear();
            } else {
                content += line;
            }
        }
    }
    if( !name.empty() ){ // Print out what we read from the last entry
        //std::cout << name << " : " << content << std::endl;
        // the last entry probably do not need to check if last entry is set.... probably
        switch_chars(content, nucs);
    }

    input.close();
    // finally send out the consensus sequences
    // Need to cout the header
    std::cout << '>' << argv[1] << "_consenses\n";
    for(nucleotides cur_base: nucs){
        // Need to output the larges element of the struct
        __uint64 nuc_struct_sum = cur_base.A + cur_base.T + cur_base.C + cur_base.G + cur_base.N + cur_base.other + cur_base.del;
        // set default threshold to 0.5 make cmd arg when nessecary
        __uint64 consensus_threshold = nuc_struct_sum / 2;
        if(cur_base.A > consensus_threshold){
            std::cout << 'A';
        }else if(cur_base.T > consensus_threshold){
            std::cout << 'T';
        }else if (cur_base.G > consensus_threshold) {
            std::cout << 'G';
        }else if (cur_base.C > consensus_threshold) {
            std::cout << 'C';
        }else if(cur_base.N > consensus_threshold){
            std::cout << 'N';
        }else if (cur_base.del > consensus_threshold) {
            std::cout << 'N'; // changed the del to a N as it behaves in programs better
        }else{
            // Need to make this a plurality call
            IUPAC_call(cur_base);
        }

    }

    std::cout << std::endl;
    return 0;
}
