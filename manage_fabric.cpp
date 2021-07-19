#include <iostream>
#include <string.h>

#include "nv_fm_agent.h"

int main(int argc, char** argv)
{
	fmReturn_t fmReturn;
	fmHandle_t fmHandle = NULL;
	char hostIpAddress[16] = { 0 };
	unsigned int operation = 0;
	fmFabricPartitionId_t partitionId = 0;
	fmFabricPartitionList_t partitionList = { 0 };
	std::cout << "Select Shared Fabric Partition Operation: \n";
	std::cout << "0 - List Supported Partition \n";
	std::cout << "1 - Activate a Partition \n";
	std::cout << "2 - Deactivate a Partition \n";
	std::cin >> operation;
	if (operation > 2) {
		std::cout << "Invalid input.\n" << std::endl;
		return FM_ST_BADPARAM;
	}
	std::cout << std::endl;
	if (operation > 0) {
		std::cout << "Input Shared Fabric Partition ID: \n";
		std::cin >> partitionId;

		if (partitionId >= FM_MAX_FABRIC_PARTITIONS) {
			std::cout << "Invalid partition ID." << std::endl;
			return FM_ST_BADPARAM;
		}
	}

	std::cout << std::endl;
	std::cout << "Connecting to Localhost (= 127.0.0.1) \n";
	std::string buffer;
	//std::cin >> buffer;
	buffer = "127.0.0.1";
	if (buffer.length() > sizeof(hostIpAddress) - 1) {
		std::cout << "Invalid IP address.\n" << std::endl;
		return FM_ST_BADPARAM;
	}
	else {
		buffer += '\0';
		strncpy(hostIpAddress, buffer.c_str(), 15);
	}

	/* Initialize Fabric Manager API interface library */
	fmReturn = fmLibInit();
	if (FM_ST_SUCCESS != fmReturn) {
		std::cout << "Failed to initialize Fabric Manager API interface library." << std::endl;
		return fmReturn;
	}
	else {
		std::cout << "Initialized Fabric Manager API interface library." << std::endl;
	}

	/* Connect to Fabric Manager instance */
	fmConnectParams_t connectParams;
	strncpy(connectParams.addressInfo, hostIpAddress, sizeof(hostIpAddress));
	connectParams.timeoutMs = 1000; // in milliseconds
	connectParams.version = fmConnectParams_version;
	connectParams.addressIsUnixSocket = 0;
	fmReturn = fmConnect(&connectParams, &fmHandle);

	if (fmReturn != FM_ST_SUCCESS) {
		std::cout << "Failed to connect to Fabric Manager instance." << std::endl;
		return fmReturn;
	}
	else {
		std::cout << "Connected to Fabric Manager instance." << std::endl;
	}

	if (operation == 0) {
		/* List supported partitions */
		partitionList.version = fmFabricPartitionList_version;
		fmReturn = fmGetSupportedFabricPartitions(fmHandle, &partitionList);
		if (fmReturn != FM_ST_SUCCESS) {
			std::cout << "Failed to get partition list. fmReturn: " << fmReturn << std::endl;
			std::cout << "partitionList.version: " << partitionList.version << std::endl;
		}
		else {
			std::cout << std::endl;
			std::cout << "Total number of partitions supported: " << partitionList.numPartitions << std::endl;
			std::cout << "Max number of partitions: " << partitionList.maxNumPartitions << std::endl;
			std::cout << "Available GPUs: " << partitionList.partitionInfo->numGpus << std::endl;
			std::cout << std::endl;
			std::cout << "----------------------------------" << std::endl << std::endl;
			/* Partition meta data */
			for (int i = 0; i < partitionList.numPartitions; i++) {
				std::cout << "Metadata of partition " << i << ": " << std::endl;
				std::cout << "Partition ID: " << partitionList.partitionInfo[i].partitionId << std::endl;
				std::cout << "Number of GPUs in this partition: " << partitionList.partitionInfo[i].numGpus << std::endl;

				int numGpusInPartition = partitionList.partitionInfo[i].numGpus;
				for (int j = 0; j < numGpusInPartition; j++) {
					std::cout << "    PCI Bus ID: " << partitionList.partitionInfo[i].gpuInfo[j].pciBusId << std::endl;
					std::cout << "    Physical ID: " << partitionList.partitionInfo[i].gpuInfo[j].physicalId << std::endl;
					std::cout << "    numNvLinksAvailable: " << partitionList.partitionInfo[i].gpuInfo[j].numNvLinksAvailable << std::endl;
					std::cout << "    GPU " << j << " uuid " << partitionList.partitionInfo[i].gpuInfo[j].uuid << std::endl;
					std::cout << std::endl;
				}

				std::cout << "----------------------------------" << std::endl << std::endl;
			}
		}
	}
	else if (operation == 1) {
		/* Activate a partition */
		fmReturn = fmActivateFabricPartition(fmHandle, partitionId);
		if (fmReturn != FM_ST_SUCCESS) {
			std::cout << "Failed to activate partition. fmReturn: " << fmReturn << std::endl;
			std::cout << "Error: " << fmReturn << std::endl;
		}
	}
	else if (operation == 2) {
		/* Deactivate a partition */
		fmReturn = fmDeactivateFabricPartition(fmHandle, partitionId);
		if (fmReturn != FM_ST_SUCCESS) {
			std::cout << "Failed to deactivate partition. fmReturn: " << fmReturn << std::endl;
		}
	}
	else {
		std::cout << "Unknown operation." << std::endl;
	}

	/* Clean up */
	fmDisconnect(fmHandle);
	fmLibShutdown();
	return fmReturn;
}
