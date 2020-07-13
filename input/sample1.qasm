// Implementation of Deutsch algorithm with two qubits for f(x)=x
OPENQASM 2.0;
include "qelib1.inc";

qreg q[16];
creg c[16];

cx q[2], q[6];
cx q[1], q[0];
h q[1];
h q[3];
cx q[1], q[3];
h q[1];
h q[3];
x q[2];
x q[2];
cx q[5], q[3];
cx q[2], q[3];
cx q[6], q[8];
