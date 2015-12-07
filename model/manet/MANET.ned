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

import inet.world.radio.ChannelControl;
import inet.environment.common.PhysicalEnvironment;
import inet.physicallayer.ieee802154.packetlevel.Ieee802154NarrowbandScalarRadioMedium;

network MANET
{
    parameters:
        double playgroundSizeX @unit(m) = default(1000m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m) = default(1000m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m) = default(1000m); // z size of the area the nodes are in (in meters)
        
        bool visualize = default(true);
        double playgroundLatitude = default(0.0); // geographic position of the playground's north-west corner
        double playgroundLongitude = default(0.0); // geographic position of the playground's north-west corner
        
        int numNodes;
        
        @display("bgb=469,300,white;bgp=10,50");
    submodules:
		environment: PhysicalEnvironment {}
 		radioMedium: Ieee802154NarrowbandScalarRadioMedium {
            parameters:
                @display("p=61,46");
        }
        
        node[numNodes]: ROSNode {}
        
        syncNode: ROSSyncNode {}
          
    connections allowunconnected:
}
