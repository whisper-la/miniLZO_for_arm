
#include "lzo_tool.h"
#include <sys/time.h>
struct timeval start, end;

int lzo_tool_init()
{
	if (lzo_init() != LZO_E_OK)
    {
        printf("internal error - lzo_init() failed !!!\n");
        printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
        return -1;
    }
	// printf("lzo压缩工具初始化成功\n");
	return 0;
}
int  compress(lzo_data *src_data)//传入待压缩数据和待压缩数据长度,成功压缩返回值0,否则为其他
{
	unsigned long *bufWrkMem=malloc(src_data->len*16*sizeof(unsigned long));
	int ret;
	lzo_data dst_data;
	dst_data.addr=malloc((src_data->len + src_data->len/ 16 + 64 + 3)*sizeof(unsigned long));;

    // printf("压缩所需要的工作空间大小约为 %dKB\n", src_data->len*16*sizeof(unsigned long)/8/1024);

	// printf("开始压缩！！！\n");

	ret = lzo1x_1_compress(src_data->addr, src_data->len, dst_data.addr, &dst_data.len, bufWrkMem);
 
	if (ret == LZO_E_OK)
	{
		printf("将 %lu 字节压缩至 %lu字节,压缩率约为 %c%d\n", src_data->len, dst_data.len,'%',dst_data.len*100/src_data->len);
	}
	else
	{
		/* this should NEVER happen */
		printf("internal error - compression failed: %d\n", ret);
		return -1;
	}
    if (dst_data.len >= src_data->len)
    {
        printf("This block contains incompressible data.\n");
		return -1;
    }
	memcpy(src_data->addr,dst_data.addr,dst_data.len);
	src_data->len=dst_data.len;
	// printf("压缩完毕！！！\n");
	free(bufWrkMem);
	free(dst_data.addr);
	return 0;//返回数据一定是压缩后的数据地址+源数据的长度
}
int decompress(lzo_data *src_data)//传入为压缩后数据地址和压缩后数据长度，成功返回值0,否则其他
{
	int ret;
	lzo_data dst_data;
	dst_data.addr=malloc(src_data->len*sizeof(unsigned long));

	// printf("开始解压！！！\n");
	ret = lzo1x_decompress(src_data->addr,src_data->len,dst_data.addr, &dst_data.len, NULL);
	if (ret == LZO_E_OK)
	{
		printf("将 %lu 字节解压至 %lu 字节\n", src_data->len, dst_data.len);
	}
	else
	{
		/* this should NEVER happen */
		printf("internal error - decompression failed: %d\n", ret);
		return -1;
	}
	// printf("解压完毕！！！\n");
	memcpy(src_data->addr,dst_data.addr,dst_data.len);
	src_data->len=dst_data.len;
	free(dst_data.addr);

	return 0;
}
void test(const char *src_path,const char *dst_path)
{
	long long total_time;
	FILE *fileRead;
	FILE *fileOut;
	lzo_data test_data,new_data,compressed_data;
	unsigned long dwRawDatBuf[1024*1*34];
	unsigned long dwRnd,len;
	int i;

	fileRead = fopen(src_path, "rb");
	if (NULL == fileRead)
	{
		printf("Opren Read File Fail!!!\n");
	}
    fileOut = fopen(dst_path, "wb");
	len = fread(dwRawDatBuf, 1, sizeof(dwRawDatBuf), fileRead);
	test_data.addr=dwRawDatBuf;
	test_data.len=len;
	gettimeofday(&start, NULL);
	compress(&test_data);
	

	gettimeofday(&end, NULL);
    total_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec); // get the run time by microsecond
    printf("total time is %lld us,%lld ms\n", total_time,total_time /1000);

	gettimeofday(&start, NULL);
	decompress(&test_data);

	gettimeofday(&end, NULL);
    total_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec); // get the run time by microsecond
    printf("total time is %lld us,%lld ms\n", total_time,total_time /1000);

	printf("解压后长度是%d\n",test_data.len);
	if(test_data.addr==NULL) 
	{
		printf("数据");
	}

	fwrite(test_data.addr,1,test_data.len, fileOut);

	fclose(fileRead);
	fclose(fileOut);
}
int main()
{
	lzo_tool_init();
	test("8.jpg","new_8.jpg");

}