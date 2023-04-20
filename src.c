/*************************************************************************/
/*             Western Digital Nvme Driver                               */
/*                                                                       */
/*                                                                       */
/*    Authors: Manjunath Sugappa                                         */
/*    Source file name : WdAllocMemApp.c                                 */
/*    Description      : WdAllocMem Apllication source                   */
/*                                                                       */
/*************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "WdAllocMemPublic.h"

void func_set_memory_type(int fd)
{
    unsigned int mem_type = MEM_TYPE_KMALLOC;
    if (ioctl(fd, WDALLOCMEM_IOC_SET_MEMORY_TYPE, &mem_type) == -1)
    {
        printf("%s: Error calling IOCTL 'WDALLOCMEM_IOC_SET_MEMORY_TYPE' \n", __FUNCTION__);
    }
    else
    {
        printf("%s: Succes calling 'WDALLOCMEM_IOC_SET_MEMORY_TYPE' \n", __FUNCTION__);

    }
}

void GetPhyicalMemInfo(int fd)
{
    PHYSICAL_MEM_INFO physical_mem;

    if (ioctl(fd, WDALLOCMEM_IOC_GET_PHYSICAL_ADDR, &physical_mem) == -1)
    {
        printf("%s: Error calling IOCTL 'WDALLOCMEM_IOC_GET_PHYSICAL_ADDR' \n", __FUNCTION__);
    }
    else
    {
        printf("%s: Physical Addr = %llx, length = %llu, Status = %d \n", __FUNCTION__, physical_mem.PhysicalAddress, physical_mem.length, physical_mem.Status);
    }
}

int main()
{
	int fd;
	int i;
	size_t MemLen;
	unsigned int *MmapVirAddr = NULL;
	char *pDevicepath = "/dev/WdAllocMem";
	/*Open device*/
	if ((fd = open(pDevicepath, O_RDWR | O_SYNC)) < 0)
	{
		printf("OpenDevice failed.\n");
		return -1;
	}
    func_set_memory_type(fd);
	MemLen = NO_OF_PAGES * getpagesize();
    printf("%s: Memlen = %ld \n", __FUNCTION__, MemLen);
	MmapVirAddr = mmap(0, MemLen, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, fd, 0);
	if (MmapVirAddr == MAP_FAILED)
	{
		printf("mmap failed");
		return -1;
	}
    printf("mmap success. mapped VAddr=0x%p size %ld bytes \n", MmapVirAddr, MemLen);
    //GetPhyicalMemInfo(fd);
	for (i = 0; i < (NO_OF_PAGES * getpagesize() / sizeof(int)); i++)
	{
		//printf("data[%d] = 0x%x \n", i, MmapVirAddr[i]);
	}
    sleep(60);
	munmap(MmapVirAddr, MemLen);
	close(fd);

	return 0;
}