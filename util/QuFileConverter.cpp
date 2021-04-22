//
// Created by hammad on 4/16/21.
//

#include <dirent.h>
#include <fstream>
#include <core/gates/QuGate.h>
#include <sstream>
#include <core/gates/QuGateFactory.h>
#include <sys/stat.h>
#include "QuFileConverter.h"
#include "Constants.h"
#include "Util.h"

void QuFileConverter::convertAllFiles() {
    for (auto file: inputFiles) {
        convertFile(file);
    }
}

QuFileConverter::QuFileConverter(string inputDirectory, string outputDirectory): inputDirectory(inputDirectory), outputDirectory(outputDirectory) {
    readFileNames();
}

void QuFileConverter::readFileNames() {
    DIR* dir = NULL;
    struct dirent *dirPtr;

    dir = opendir(inputDirectory.c_str());
    while ((dirPtr = readdir(dir)) != NULL) {
        string temp = string(dirPtr->d_name);
        if(temp != "." && temp != "..")
            inputFiles.push_back(temp);
    }
    closedir(dir);
}

void QuFileConverter::skipEmptyLines(ifstream& ifs){
    string line = "";
    do {
        if (isalnum(ifs.peek()))
            return;
        getline(ifs, line);
    } while (!ifs.eof() && line == "");

}

void QuFileConverter::convertFile(string file) {
    ifstream ifs;
    ofstream ofs;

    string quGate = "";
    string qubitArgs = "";
    int operandIndexes[QuGate::MAX_OPERANDS] = {-1, -1, -1};
    int pos1 = 0;
    int pos2 = 0;
    int i = 0;
    string qubits = "";

    std::vector<pair<int ,int>>::iterator it;
    string header = "";
    file = file.substr(0, file.size() - (Constants::QCODE_FILE_EXTENSION.size()));
    string theInputFileName = Constants::QCODE_INPUT_FILES_DIRECTORY_RPATH + file + Constants::QCODE_FILE_EXTENSION;

    createOutputDirectoryIfNotExists();
    string theOutputFileName = Constants::QCODE_OUTPUT_FILES_DIRECTORY_RPATH + file + Constants::FILE_EXTENSION;
    ifs.open(theInputFileName);
    skipEmptyLines(ifs);
    getline(ifs, header);
    stringstream lineStream(header);
    getline(lineStream, qubits, ' '); // mnemonic for qu-gate e.g. h for Hadamard, x for NOT, cx for C-NOT, etc.
    getline(lineStream, qubits, ' '); // mnemonic for qu-gate e.g. h for Hadamard, x for NOT, cx for C-NOT, etc.
    skipEmptyLines(ifs);

    if (stoi(qubits) <= Constants::QX4_N) {  // convert only files with <= 5 qubits
        ofs.open(theOutputFileName, std::ofstream::out | std::ofstream::trunc);

        string qasmHeader = makeHeaderForQASM();
        ofs << qasmHeader << endl;

        while (!ifs.eof()) {
            string line;
            string newLine;
            getline(ifs, line);
            if (line == "") continue;
            convertOneInstruction(line, newLine);
            ofs << newLine << endl;
        }
        ofs.close();
    }
    ifs.close();
}

QuFileConverter::QuFileConverter() {

}


void QuFileConverter::convertOneInstruction(string &qcode, string &qasm) {
    stringstream lineStream(qcode);
    vector<string> tokens = Util::tokenize(qcode);
    string mnemonic = getMnemonicForQASM(Util::toLower(tokens[0]));
    cout << mnemonic << endl;
    qasm += mnemonic + " ";
    qasm += putSubscripts(tokens[1]);
    if (tokens.size() > 2) {
        qasm += "," + putSubscripts(tokens[2]);
    }
    qasm += ";";
}

string QuFileConverter::makeHeaderForQASM() {
    return Constants::QASM_HEADER_5_BITS;

}

string QuFileConverter::getMnemonicForQASM(string mnemonic) {
    if (mnemonic == "tdag")
        mnemonic = "tdg";
    else if (mnemonic == "cnot")
        mnemonic = "cx";

    return mnemonic;
}

string QuFileConverter::putSubscripts(string qubitArg) {
    qubitArg.insert(1,"[");
    qubitArg.insert(qubitArg.size(),"]");
    return qubitArg;
}

void QuFileConverter::createOutputDirectoryIfNotExists() {
    DIR* dir = NULL;
    string directory = Constants::QCODE_OUTPUT_FILES_DIRECTORY_RPATH;
    dir = opendir(directory.c_str());
    if(dir == NULL)
//        _mkdir(directory.c_str());
        mkdir(directory.c_str(), 0777);
}
