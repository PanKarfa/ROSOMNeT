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

import inet.mobility.static.StationaryMobility;
import inet.linklayer.ieee802154.Ieee802154NarrowbandNic;


module ROSNode {
	parameters:
		@node;
		@networkNode();
		*.interfaceTableModule = "";
        
	gates:
		input radio80211In @directIn;
		input radio802154In @directIn;
		inout direct[];
        
	submodules:
		// Application
		app: ROSForwarderApplication {
			parameters:
				@display("p=70,30;i=app");
		}
        
		// Mobility module
		mobility: StationaryMobility {
			parameters:
				@display("p=53,111");
		}
        
		// NIC
		nic802154: Ieee802154NarrowbandNic {
			parameters:
				@display("p=132,406,row,60;q=queue");
		}

	connections allowunconnected:
		nic802154.upperLayerOut --> app.lower802154LayerIn;
		nic802154.upperLayerIn <-- app.lower802154LayerOut;
}

