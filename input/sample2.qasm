OPENQASM 2.0;
include "qelib1.inc";

qreg q[16];
creg c[16];

cx q[5], q[13];
cx q[14], q[5];
h q[1];
h q[3];
cx q[14], q[15];
h q[1];
h q[3];
x q[2];
x q[2];
cx q[5], q[0];
