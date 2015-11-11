# Makefile for master server
# Note: C++11 is needed for mutex locks
#
LDFLAGS = -W -ansi -Wall -Wextra -pedantic -g -std=c++11 -pthread -lrt -o masterserver

masterserver: CNetwork.cpp CServers.cpp Main.cpp ../sdks/UDPWrapper/UDPWrapper.cpp ../sdks/UDPWrapper/clhmpquery.cpp

	g++ $(LDFLAGS) CNetwork.cpp CServers.cpp Main.cpp ../sdks/UDPWrapper/UDPWrapper.cpp ../sdks/UDPWrapper/clhmpquery.cpp -o masterserver
	-echo $?
