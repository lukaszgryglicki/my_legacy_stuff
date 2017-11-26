all: pwdmgmt pwdmgmtd
pwdmgmt: pwdmgmt.cc
	g++ -Wall -ansi -pedantic -o pwdmgmt pwdmgmt.cc
	strip -s pwdmgmt
pwdmgmtd: pwdmgmt.cc
	g++ -DMDBG -Wall -ansi -pedantic -g3 -o pwdmgmtd pwdmgmt.cc
clean:
	rm pwdmgmt pwdmgmtd /tmp/pwdmgmt*.log

