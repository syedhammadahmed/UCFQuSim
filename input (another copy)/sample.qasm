OPENQASM 2.0;
include "qelib1.inc";
qreg q[16];
creg c[16];

cx q[0],q[1];
cx q[2],q[1];
cx q[4],q[13];
cx q[5],q[9];
cx q[4],q[5];



