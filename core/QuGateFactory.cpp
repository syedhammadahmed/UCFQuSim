//
// Created by hammad on 10/8/19.
//

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

QuGate* QuGateFactory::getQuGate(string symbol, int *args) {
    return NULL;
}

QuGate* QuGateFactory::getQuGate(string symbol) {
    QuGate* gate = NULL;

    if(symbol == "h"){
        gate = new Hadamard();
    }
    else if(symbol == "cx"){
        gate = new CNot();
    }
    else if(symbol == "id"){
        gate = new Identity();
    }
    else if(symbol == "u3"){
        gate = new U3();
    }
    else if(symbol == "u2"){
        gate = new U2();
    }
    else if(symbol == "u1"){
        gate = new U1();
    }
    else if(symbol == "rx"){
        gate = new Rx();
    }
    else if(symbol == "ry"){
        gate = new Ry();
    }
    else if(symbol == "rz"){
        gate = new Rz();
    }
    else if(symbol == "x"){
        gate = new X();
    }
    else if(symbol == "y"){
        gate = new Y();
    }
    else if(symbol == "z"){
        gate = new Z();
    }
    else if(symbol == "s"){
        gate = new S();
    }
    else if(symbol == "sdg"){
        gate = new Sdg();
    }
    else if(symbol == "t"){
        gate = new T();
    }
    else if(symbol == "tdg"){
        gate = new Tdg();
    }
    else if(symbol == "ch"){
        gate = new CHadamard();
    }
    else if(symbol == "cy"){
        gate = new CY();
    }
    else if(symbol == "cz"){
        gate = new CZ();
    }
    else if(symbol == "crz"){
        gate = new Rx();
    }
    else if(symbol == "cu1"){
        gate = new CU1();
    }
    else if(symbol == "cu3"){
        gate = new CU3();
    }
    else if(symbol == "measure"){
        gate = new Measure();
    }

    return gate;
}
