DEPENDENCIES

    - VSOMEIP Library is necessary for this tool to work, follow instruction found at https://github.com/GENIVI/vsomeip to install
    - CMAKE version 3.8 or above

COMPILATION & SETUP

For compilation, call:
    mkdir build
    cd build
    cmake ..
    make

Configuration for TCAM interface, replace <INTERFACE> with your physical interface name:  
    #VLAN 205 for audio stream
    - sudo ip addr add 198.19.102.162/27 brd 198.19.102.191 dev <INTERFACE>.205
    - sudo ip link set dev <INTERFACE>.205 up

    #VLAN 172 for ConvAPI commands
    - sudo ip addr add 198.19.101.65/27 brd 198.19.101.95 dev <INTERFACE>.172
    - sudo ip link set dev <INTERFACE>.172 up

Configure multicast route, replace <INTERFACE> with the name of the host interface connected to target with TCAM interface: 
    - sudo ip route add multicast 224.224.224.245 dev <INTERFACE>.172

USAGE

To start the command line tool, in build folder type:
    - ./RemoteCtrlHostSim ../vsomeip_client.json

Logging/debugging can be enabled in vsomeip_client.json, change key "logging" from false to true.