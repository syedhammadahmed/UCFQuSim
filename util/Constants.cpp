//
// Created by SHA on 10/21/2019.
//

#include "Constants.h"
#include <string>

using namespace std;

const string Constants::FILE_EXTENSION = ".qasm";
const string Constants::INPUT_FILES_DIRECTORY_RPATH = "../input/";
const string Constants::OUTPUT_FILES_DIRECTORY_RPATH = "../output/";

const string Constants::QCODE_FILE_EXTENSION = ".qcode";
const string Constants::QCODE_INPUT_FILES_DIRECTORY_RPATH = "../input/qcode/";
const string Constants::QCODE_OUTPUT_FILES_DIRECTORY_RPATH = "../input/input_from_qcode/";

const string Constants::QASM_HEADER_5_BITS = "OPENQASM 2.0;\n"
                                      "include \"qelib1.inc\";\n"
                                      "qreg q[5];\n"
                                      "creg c[5];\n";

const string Constants::QASM_HEADER_16_BITS = "OPENQASM 2.0;\n"
                                             "include \"qelib1.inc\";\n"
                                             "qreg q[16];\n"
                                             "creg c[16];\n";