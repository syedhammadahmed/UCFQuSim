OPENQASM 2.0;
include "qelib1.inc";
qreg q[5];
creg c[5];

h q[3];
x q[1];
x q[2];
cx q[0],q[1];
cx q[2],q[1];
cx q[4],q[2];
tdg q[2];
t q[3];
cx q[1],q[0];
cx q[1],q[0];
x q[1];
h q[3];
