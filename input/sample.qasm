OPENQASM 2.0;
include "qelib1.inc";
qreg q[16];
creg c[16];
cx q[2],q[1];
cx q[2],q[1];
cx q[8],q[10];
cx q[5],q[2];
cx q[0],q[2];
cx q[9],q[6];

