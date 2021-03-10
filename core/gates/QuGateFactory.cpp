//
// Created by SHA on 10/8/19.
//
#include <cctype>
#include "QuGateFactory.h"
#include "gates/Hadamard.h"
#include "gates/CNot.h"
#include "gates/Identity.h"
#include "gates/U3.h"
#include "gates/U2.h"
#include "gates/U1.h"
#include "gates/Rx.h"
#include "gates/Rz.h"
#include "gates/Ry.h"
#include "gates/X.h"
#include "gates/Y.h"
#include "gates/Z.h"
#include "gates/Tdg.h"
#include "gates/CHadamard.h"
#include "gates/T.h"
#include "gates/S.h"
#include "gates/Sdg.h"
#include "gates/CU1.h"
#include "gates/CY.h"
#include "gates/CU3.h"
#include "gates/CZ.h"
#include "gates/Measure.h"
#include "gates/Swap.h"
#include "util/Util.h"

std::shared_ptr<QuGate> QuGateFactory::getQuGate(string mnemonic) {
    std::shared_ptr<QuGate> gate;

    mnemonic = Util::toLower(mnemonic);

    if(mnemonic == "h"){
//        gate = std::make_shared<Hadamard>();
        gate = std::make_shared<Hadamard>();
    }
    else if(mnemonic == "cx"){;
//        gate = std::make_shared<CNot>();
        gate = std::make_shared<CNot>();
    }
    else if(mnemonic == "id"){
        gate = std::make_shared<Identity>();
    }
    else if(mnemonic == "u3"){
        gate = std::make_shared<U3>();
    }
    else if(mnemonic == "u2"){
        gate = std::make_shared<U2>();
    }
    else if(mnemonic == "u1"){
        gate = std::make_shared<U1>();
    }
    else if(mnemonic == "rx"){
        gate = std::make_shared<Rx>();
    }
    else if(mnemonic == "ry"){
        gate = std::make_shared<Ry>();
    }
    else if(mnemonic == "rz"){
        gate = std::make_shared<Rz>();
    }
    else if(mnemonic == "x"){
        gate = std::make_shared<X>();
    }
    else if(mnemonic == "y"){
        gate = std::make_shared<Y>();
    }
    else if(mnemonic == "z"){
        gate = std::make_shared<Z>();
    }
    else if(mnemonic == "s"){
        gate = std::make_shared<S>();
    }
    else if(mnemonic == "sdg"){
        gate = std::make_shared<Sdg>();
    }
    else if(mnemonic == "t"){
        gate = std::make_shared<T>();
    }
    else if(mnemonic == "tdg"){
        gate = std::make_shared<Tdg>();
    }
    else if(mnemonic == "ch"){
        gate = std::make_shared<CHadamard>();
    }
    else if(mnemonic == "cy"){
        gate = std::make_shared<CY>();
    }
    else if(mnemonic == "cz"){
        gate = std::make_shared<CZ>();
    }
    else if(mnemonic == "crz"){
        gate = std::make_shared<Rx>();
    }
    else if(mnemonic == "cu1"){
        gate = std::make_shared<CU1>();
    }
    else if(mnemonic == "cu3"){
        gate = std::make_shared<CU3>();
    }
    else if(mnemonic == "measure"){
        gate = std::make_shared<Measure>();
    }
    else if(mnemonic == "swap"){
        gate = std::make_shared<Swap>();
    }

    return gate;
}
