OPENQASM 2.0;
include "qelib1.inc";
qreg q[16];
creg c[16];

cx q[0],q[1];
cx q[2],q[3];
cx q[0],q[8];
cx q[8],q[1];
cx q[9],q[3];
cx q[4],q[5];
cx q[6],q[7];

