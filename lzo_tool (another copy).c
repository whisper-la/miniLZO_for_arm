
#include "lzo_tool.h"


int lzo_tool_init()
{
	if (lzo_init() != LZO_E_OK)
    {
        printf("internal error - lzo_init() failed !!!\n");
        printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
        return -1;
    }
	return 0;
}
unsigned long compress(unsigned long*src_data,unsigned long src_len)
{
	unsigned long *bufWrkMem=malloc(src_len*16*sizeof(unsigned long)); //工作空间长度为源数据长度乘以16
	int ret;
	unsigned long *dst_data= malloc((src_len+ src_len/ 16 + 64 + 3)*sizeof(unsigned long));
	unsigned long dst_len;

    printf("bufWrkMem sizeof = %d\n", sizeof(bufWrkMem));

	printf("开始压缩！！！\n");

	ret = lzo1x_1_compress(src_data, src_len, dst_data, &dst_len, bufWrkMem);
 
	if (ret == LZO_E_OK)
	{
		printf("将 %lu 字节压缩至 %lu字节,压缩率为%c %d ", src_len, dst_len,'%',dst_len*100/src_len);
	}
	else
	{
		/* this should NEVER happen */
		printf("internal error - compression failed: %d\n", ret);
	}
    if (dst_len >= src_len)
    {
        printf("This block contains incompressible data.\n");
    }
	printf("压缩完毕！！！\n");
	memcpy(src_data,dst_data,dst_len);
	// free(dst_data);
	return src_len;//返回数据一定是压缩后的数据地址+源数据的长度
}
int decompress(unsigned long *compressed_data,unsigned long src_len)//输入数据为压缩后数据地址和压缩前数据长度
{
	int ret;
	unsigned long*decompressed_data= malloc((src_len+ src_len/ 16 + 64 + 3)*sizeof(unsigned long));
	unsigned long decompressed_len;

	printf("开始解压！！！\n");
	ret = lzo1x_decompress(compressed_data, src_len,decompressed_data, &decompressed_len, NULL);
	if (ret == LZO_E_OK)
	{
		// fwrite(bufWrite, 1, writeLen, fileOut);
		printf("将 %lu 字节解压至 %lu 字节\n", src_len, decompressed_len);
		// readLen = param[paramCnt++];
	}
	else
	{
		/* this should NEVER happen */
		printf("internal error - decompression failed: %d\n", ret);
	}
	printf("解压完毕！！！\n");
	return 0;
}
int main()
{
	unsigned long dwRawDatBuf[256];
	unsigned long dwRnd,len;
	int i;
	unsigned long dwDecompressBuf[256];
	for(i = 0; i < 256; )
	{
		dwRnd = rand();
		//for(j = 0; j < 10; j++)
		{
			dwRawDatBuf[i] = dwRnd;
			i++;
		}
	}
	lzo_tool_init();
	len=compress(&dwRawDatBuf,256);
	decompress(&dwRawDatBuf,256);
}