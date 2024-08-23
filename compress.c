#include <stdio.h>
#include <stdlib.h>

#include "minilzo.h"


int main(int argc, char *argv[])
{
    FILE *fileRead;
	FILE *fileOut;
	unsigned long bufRead[1024*1]; //缓冲读取128Byte
	unsigned long bufWrkMem[1024*16]; //4KB
	unsigned long bufWrite[sizeof(bufRead) + sizeof(bufRead) / 16 + 64 + 3];
	unsigned long ret;
	unsigned long readLen;
	unsigned long writeLen;
	unsigned long param[1500];  //存放压缩后的数据长度
	unsigned long paramCnt;

    printf("bufWrkMem sizeof = %d\n", sizeof(bufWrkMem));

    // int r;
    // lzo_uint in_len;
    // lzo_uint out_len;
    // lzo_uint new_len;

/*
 * Step 1: 初始化压缩工具
 */
    if (lzo_init() != LZO_E_OK)
    {
        printf("internal error - lzo_init() failed !!!\n");
        printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
        return 3;
    }


/*
 * Step 2: 准备输入数据
 */
	fileRead = fopen("testSource.txt", "rb");
	if (NULL == fileRead)
	{
		printf("Opren Read File Fail!!!\n");
	}
    fileOut = fopen("testFileCompress.txt", "wb");
    paramCnt = 0;
    printf("Open Read File success!!!\n");

/*
 * Step 3: compress with LZO1X-1
 */
	while ((readLen = fread(bufRead, 1, sizeof(bufRead), fileRead)) > 0)
	{
	printf("开始压缩!!!\n");

		ret = lzo1x_1_compress(bufRead, readLen, bufWrite, &writeLen, bufWrkMem);
 
		if (ret == LZO_E_OK)
		{
			fwrite(bufWrite, 1, writeLen, fileOut);
			param[paramCnt++] = writeLen;
			printf("compressed %lu bytes into %lu bytes\n", (unsigned long)readLen, (unsigned long)writeLen);
		}
		else
		{
			/* this should NEVER happen */
			printf("internal error - compression failed: %d\n", ret);
			return 2;
		}
        if (writeLen >= readLen)
        {
            printf("This block contains incompressible data.\n");
            return 0;
        }
	}
	printf("压缩完毕!!!\n");
	param[paramCnt++] = sizeof(bufRead);
 
	fclose(fileRead);
	fclose(fileOut);
	
	fileRead = fopen("testFileCompress.txt", "rb");
	if (NULL == fileRead)
	{
		printf("Opren Read File Fail!!!\n");
	}
	fileOut = fopen("testFileDecompress.txt", "wb");
 
	paramCnt = 0;
 
	readLen = param[paramCnt++];
 
	//解压
	while ((fread(bufRead, 1, readLen, fileRead)) > 0)
	{
		ret = lzo1x_decompress(bufRead, readLen, bufWrite, &writeLen, NULL);
		if (ret == LZO_E_OK)
		{
			fwrite(bufWrite, 1, writeLen, fileOut);
			printf("decompressed %lu bytes back into %lu bytes\n", (unsigned long)readLen, (unsigned long)writeLen);
			readLen = param[paramCnt++];
		}
		else
		{
			/* this should NEVER happen */
			printf("internal error - decompression failed: %d\n", ret);
			
			return 1;
		}
	}
 
	fclose(fileRead);
	fclose(fileOut);
 
	return 0;
}
