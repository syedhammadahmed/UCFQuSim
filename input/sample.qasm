OPENQASM 2.0;
include "qelib1.inc";
qreg q[16];
creg c[16];
x q[12];
cx q[0],q[6];
h q[6];