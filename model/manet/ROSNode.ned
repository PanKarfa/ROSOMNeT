//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package manet;

import inet.nodes.inet.WirelessHost;
import inet.applications.IPingApp;
import inet.applications.ISCTPApp;
import inet.applications.ITCPApp;
import inet.applications.IUDPApp;
import inet.transport.ISCTP;
import inet.transport.ITCP;
import inet.transport.IUDP;
import inet.util.PcapRecorder;
import inet.networklayer.ipv4.RoutingTable;
import inet.networklayer.common.InterfaceTable;
import inet.mobility.contract.IMobility;
import inet.linklayer.IWirelessNic;
import inet.linklayer.IWiredNic;
import inet.linklayer.IExternalNic;
import inet.base.NotificationBoard;
import inet.nodes.inet.NetworkLayer;
import inet.transport.udp.UDP;

// This host is an example on how a host with INET network stack
// can be equipped with mobility- and wireless-modules from MiXiM. 
//
// It contains an INET network stack and a MiXiM 802.15.4 NIC (TI_CC2420)
// as well as a battery module from MiXiM which is used by the NIC.
//
// This host's NIC is able to operate in a multi-ConnectionManager
// network, i.e. more than one physical network in parallel (non-interfering).
// TODO: factor out battery into INET

module ROSNode {
     parameters:
        @display("i=device/pc2");
        @node;
        @labels(node,wireless-node);
        
        //NIC
        string nic802154Type = default("");
        
    gates:
     	input radio80211In @directIn;
     	input radio802154In @directIn;
        inout direct[];
        
    submodules:
        //Application
        app: ROSForwarderApplication {
            parameters:
                @display("p=70,30;i=app");
        }
        
        // Mobility module
        mobility: <mobilityType> like IMobility {
            parameters:
                @display("p=53,111");
        }
                    
        //NIC
        nic802154: <nic802154Type> like IWirelessNic {
            parameters:
                @display("p=132,406,row,60;q=queue");
        }
         
    connections allowunconnected:
 
        //# Connect MANET branch

        if (nic802154Type != "") {
            nic802154.upperLayerOut --> app.lower802154LayerIn;
            nic802154.upperLayerIn <-- app.lower802154LayerOut;
        }
}

