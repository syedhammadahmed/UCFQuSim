// Implementation of Deutsch algorithm with two qubits for f(x)=x
OPENQASM 2.0;
include "qelib1.inc";

qreg q[16];
creg c[16];

rz(-0.3) q[0];
cx q[4], q[12];
cx q[14], q[12];
cx q[4], q[12];
h q[1];
h q[3];
