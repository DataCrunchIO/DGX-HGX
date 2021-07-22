# DGX-HGX
DGX-HGX Tools

This tool lists and (de)activates available fabric manager partitions when you're utilising the Shared NVSwitch Virtualization Model on a DGX/HGX system.

Based on the sample code found at Chapter 7:
https://docs.nvidia.com/datacenter/tesla/pdf/fabric-manager-user-guide.pdf

Requirements:
On a VM, you will need:
1. NVidia driver
1. NVidia Fabric manager
1. You will need nvidia-fabricmanager-dev-<version> (eg. **apt install nvidia-fabricmanager-dev-470**) to get the needed headers for the tool.

How to use:
1. Create a VM and pass through all GPUs and switches.
1. Stop the fabric manager service in case it is running: **systemctl stop nvidia-fabricmanager**
1. Edit /usr/share/nvidia/nvswitch/fabricmanager.cfg, set **FABRIC_MODE=1** and **FABRIC_MODE_RESTART=0** (it won't work for me when FABRIC_MODE_RESTART=1).
1. Start the fabric manager service: **systemctl start nvidia-fabricmanager**
1. Git clone this tool and **make**
1. List supported partitions and activate.
