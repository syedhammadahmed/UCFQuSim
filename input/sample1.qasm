// Implementation of Deutsch algorithm with two qubits for f(x)=x
OPENQASM 2.0;
include "qelib1.inc";

qreg q[16];
creg c[16];

h q[1];
h q[3];
cx q[0], q[1];
cx q[1], q[2];
cx q[2], q[4];
h q[1];
h q[3];
