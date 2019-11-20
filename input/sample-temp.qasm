OPENQASM 2.0;
include "qelib1.inc";

qreg q[16];
creg c[16];

h q[1];
cx q[1], q[3];
cx q[2], q[3];
x q[3];
cx q[2], q[4];
cx q[3], q[4];
cx q[4], q[12];
cx q[5], q[12];
h q[7];
cx q[11], q[13];
cx q[11], q[14];
cx q[11], q[12];
