//
// Created by hammad on 10/8/19.
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
#include "Util.h"

QuGate* QuGateFactory::getQuGate(string mnemonic) {
    QuGate* gate = NULL;

    mnemonic = Util::toLower(mnemonic);

    if(mnemonic == "h"){
        gate = new Hadamard();
    }
    else if(mnemonic == "cx"){
        gate = new CNot();
    }
    else if(mnemonic == "id"){
        gate = new Identity();
    }
    else if(mnemonic == "u3"){
        gate = new U3();
    }
    else if(mnemonic == "u2"){
        gate = new U2();
    }
    else if(mnemonic == "u1"){
        gate = new U1();
    }
    else if(mnemonic == "rx"){
        gate = new Rx();
    }
    else if(mnemonic == "ry"){
        gate = new Ry();
    }
    else if(mnemonic == "rz"){
        gate = new Rz();
    }
    else if(mnemonic == "x"){
        gate = new X();
    }
    else if(mnemonic == "y"){
        gate = new Y();
    }
    else if(mnemonic == "z"){
        gate = new Z();
    }
    else if(mnemonic == "s"){
        gate = new S();
    }
    else if(mnemonic == "sdg"){
        gate = new Sdg();
    }
    else if(mnemonic == "t"){
        gate = new T();
    }
    else if(mnemonic == "tdg"){
        gate = new Tdg();
    }
    else if(mnemonic == "ch"){
        gate = new CHadamard();
    }
    else if(mnemonic == "cy"){
        gate = new CY();
    }
    else if(mnemonic == "cz"){
        gate = new CZ();
    }
    else if(mnemonic == "crz"){
        gate = new Rx();
    }
    else if(mnemonic == "cu1"){
        gate = new CU1();
    }
    else if(mnemonic == "cu3"){
        gate = new CU3();
    }
    else if(mnemonic == "measure"){
        gate = new Measure();
    }
    else if(mnemonic == "swap"){
        gate = new Swap();
    }

    return gate;
}
