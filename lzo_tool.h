#ifndef LZO_TOOL_H
#define LZO_TOOL_H
#include <stdio.h>
#include <stdlib.h>

#include "minilzo.h"

typedef struct lzo_data_struct {
    unsigned long *addr;
    unsigned long len;
}lzo_data;

lzo_data compress(lzo_data src_data);//输入数据为待压缩数据地址和待压缩数据的长度
lzo_data decompress(lzo_data src_data);//输入数据为压缩后数据地址和压缩前数据长度
int lzo_tool_init();//初始化函数，默认返回值0,出错返回值-1


#endif 