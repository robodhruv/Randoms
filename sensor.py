# This program captures UDP packets on port identified in the variable
# port_no. The port will remain blocked until the script is killed.

from twisted.internet.protocol import DatagramProtocol
from twisted.internet import reactor

port_no = 5555;

class Echo(DatagramProtocol):

    def datagramReceived(self, data, addr):
        print("received %r from %s" % (data, addr))
        self.transport.write(data, addr)

reactor.listenUDP(port_no, Echo())
reactor.run()
