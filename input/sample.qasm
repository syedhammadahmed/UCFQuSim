OPENQASM 2.0;
include "qelib1.inc";
qreg q[16];
creg c[16];
cx q[1],q[8];
cx q[0],q[9];