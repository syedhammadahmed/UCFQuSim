OPENQASM 2.0;
include "qelib1.inc";
qreg q[5];
creg c[5];

x q[0];
cx q[0],q[3];
x q[0];
