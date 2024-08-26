
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
lzo_data compress(lzo_data src_data)
{
	unsigned long bufWrkMem[src_data.len*16]; //工作空间长度为源数据长度乘以16
	int ret;
	lzo_data dst_data;
	unsigned long temp[src_data.len + src_data.len/ 16 + 64 + 3];
	dst_data.addr=temp;

    printf("bufWrkMem sizeof = %d\n", sizeof(bufWrkMem));

	printf("开始压缩！！！\n");

	ret = lzo1x_1_compress(src_data.addr, src_data.len, dst_data.addr, &dst_data.len, bufWrkMem);
 
	if (ret == LZO_E_OK)
	{
		printf("将 %lu 字节压缩至 %lu字节,压缩率为\%%d ", src_data.len, dst_data.len,src_data.len*100/dst_data.len);
	}
	else
	{
		/* this should NEVER happen */
		printf("internal error - compression failed: %d\n", ret);
	}
    if (dst_data.len >= src_data.len)
    {
        printf("This block contains incompressible data.\n");
    }
	printf("压缩完毕！！！\n");

	dst_data.len=src_data.len;

	return dst_data;//返回数据一定是压缩后的数据地址+源数据的长度
}
lzo_data decompress(lzo_data src_data)//输入数据为压缩后数据地址和压缩前数据长度
{
	// unsigned long bufWrkMem[src_data.len*16]; //工作长度为源数据长度乘以16
	int ret;
	lzo_data dst_data;
	printf("无符号长整形为%d",sizeof(unsigned long));
	unsigned long temp[src_data.len];
	dst_data.addr=temp;

	printf("开始解压！！！\n");
	ret = lzo1x_decompress(src_data.addr, src_data.len, dst_data.addr, &dst_data.len, NULL);
	if (ret == LZO_E_OK)
	{
		// fwrite(bufWrite, 1, writeLen, fileOut);
		printf("将 %lu 字节解压至 %lu 字节\n", src_data.len, dst_data.len);
		// readLen = param[paramCnt++];
	}
	else
	{
		/* this should NEVER happen */
		printf("internal error - decompression failed: %d\n", ret);
	}
	printf("解压完毕！！！\n");
	return dst_data;
}
