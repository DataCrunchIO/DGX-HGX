import os

gpulist = ["07:00", "0a:00", "47:00", "4d:00", "87:00", "8d:00", "c7:00", "ca:00"]
topology = [[2, 3],
            [8, 9],
            [4, 5],
            [0, 1],
            [10, 11],
            [6, 7]]

def sbr_reset(gpuBDF):
  print("SBR resetting GPU " + str(gpuBDF))
  command = 'exec python nvidia_gpu_tools.py --gpu-bdf="' + str(gpuBDF) + '" --reset-with-sbr --recover-broken-gpu'
  #print(command)
  os.system(command)

def block_nvlink(gpuBDF, linkNum):
  print("  Disabling link " + str(linkNum))
  command = 'exec python nvidia_gpu_tools.py --gpu-bdf="' + str(gpuBDF) + '" --block-nvlink ' + str(linkNum)
  #print(command)
  os.system(command)

#SBR reset all GPUs
for i in range(len(gpulist)):
  sbr_reset(gpulist[i])

#section 1; block nvlinks to switch 4 5 6
for i in [0, 1, 2, 3]:
  print("\n---------------------------")
  print("Disabling links on GPU " + str(i) + " (" + gpulist[i] + ")")
  #0 1 2 active, disable links to 3 4 5
  for j in [3, 4, 5]:
    block_nvlink(gpulist[i], topology[j][0])
    block_nvlink(gpulist[i], topology[j][1])


print("---------------------------\n----Next VM----")
#section 2; block nvlinks to switch 1 2 3
for i in [4, 5, 6, 7]:
  print("\n---------------------------")
  print("Disabling links on GPU " + str(i) + " (" + gpulist[i] + ")")
  #3 4 5 active, disable links to 0 1 2
  for j in [0, 1, 2]:
    block_nvlink(gpulist[i], topology[j][0])
    block_nvlink(gpulist[i], topology[j][1])
