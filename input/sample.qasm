OPENQASM 2.0;
include "qelib1.inc";
qreg q[16];
creg c[16];
cx q[2],q[10];
h q[0];
cx q[2],q[6];
cx q[8],q[11];
cx q[5],q[12];
h q[0];
h q[0];
h q[0];
cx q[0],q[2];
h q[0];
cx q[9],q[6];
h q[0];


