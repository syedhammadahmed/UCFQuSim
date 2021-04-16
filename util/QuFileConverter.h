//
// Created by hammad on 4/16/21.
//

#ifndef UCFQUSIM_QUFILECONVERTER_H
#define UCFQUSIM_QUFILECONVERTER_H

#include <vector>
#include <string>
using namespace std;

class QuFileConverter {
private:
    string inputDirectory;
    string outputDirectory;
    vector<string> inputFiles;

    void readFileNames();
    void convertFile(string file);
    void skipEmptyLines(ifstream &ifs);
    string putSubscripts(string qubitArg);
    string makeHeaderForQASM();
    string getMnemonicForQASM(string mnemonic);
    void convertOneInstruction(string &qcode, string &qasm);
    void createOutputDirectoryIfNotExists();

public:
    QuFileConverter();
    QuFileConverter(string inputDirectory, string outputDirectory);
    void convertAllFiles();
};


#endif //UCFQUSIM_QUFILECONVERTER_H
