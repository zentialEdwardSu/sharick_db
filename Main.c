#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint-gcc.h>

int chunk_size = 9;
char database_file_path[] = "base.daaaaaa";

typedef struct{
    char* data;
    uint8_t size;
}db_string;

typedef struct{
    // char* tablename;
    // uint8_t name_size;
    // char** col_names;
    // uint8_t col_names_size;
    // char* datatype_encode;
    // uint8_t datatype_encode_size;
    db_string tablename;
    db_string *col_names;
    db_string datatype_encode;
    uint32_t current_index;
}table_head;

void BuddhismBless(void)
{
//                              _ooOoo_
//                             o8888888o
//                             88" . "88
//                             (| -_- |)
//                              O\ = /O
//                           ____/`---'\____
//                        .   ' \\| |// `.
//                         / \\||| : |||// \
//                        / _||||| -:- |||||- \
//                         | | \\\ - /// | |
//                       | \_| ''\---/'' | |
//                        \ .-\__ `-` ___/-. /
//                    ___`. .' /--.--\ `. . __
//                  ."" '< `.___\_<|>_/___.' >'"".
//                 | | : `- \`.;`\ _ /`;.`/ - ` : | |
//                    \ \ `-. \_ __\ /__ _/ .-` / /
//           ======`-.____`-.___\_____/___.-`____.-'======
//                              `=---='
//
//           .............................................
//                     佛祖保佑             永无BUG
//            佛曰:
//                     写字楼里写字间，写字间里程序员；
//                     程序人员写程序，又拿程序换酒钱。
//                     酒醒只在网上坐，酒醉还来网下眠；
//                     酒醉酒醒日复日，网上网下年复年。
//                     但愿老死电脑间，不愿鞠躬老板前；
//                     奔驰宝马贵者趣，公交自行程序员。
//                     别人笑我忒疯癫，我笑自己命太贱；
//                     不见满街漂亮妹，哪个归得程序员？
}

//函数说明：从指定的地址读取一个整形
//
//        row_insider_cursor: 数据地址
//        size: 忽略
//
//返回读取后向后移的指针
void *read_int(char *row_insider_cursor,int size)
{
    char *int_data = (char *)malloc(sizeof(int));
    memcpy(int_data,row_insider_cursor,sizeof(int));
    printf("%6d",*int_data);
    row_insider_cursor += 4;
    free(int_data);
    return row_insider_cursor;
}

//函数说明：从指定的地址读取一个浮点
//
//        row_insider_cursor: 数据地址
//        size: 忽略
//
//返回读取后向后移的指针
void *read_float(char *row_insider_cursor,int size)
{
    char *float_data = (char *)malloc(sizeof(float));
    memcpy(float_data,row_insider_cursor,sizeof(float));
    printf("%f",*float_data);
    row_insider_cursor += 4;
    free(float_data);
    return row_insider_cursor;
}

//函数说明：从指定地址读取定长字符串
//
//        row_insider_cursor: 数据地址
//        size: 字符串长度
//
//返回读取后向后移的指针
void *read_char(char *row_insider_cursor,int size)
{
    char *char_data = (char *)malloc(size*sizeof(char));
    memcpy(char_data,row_insider_cursor,sizeof(int));
    for(int i=0;i<size;i++)
    {
        printf("%c",*(char_data+i));
        // char_data++;
    }
    printf("\n");
    // printf("%s\n",*char_data);
    row_insider_cursor += size;
    free(char_data);
    return row_insider_cursor;
}

//函数说明：在指定地址往后写入一个整形
//
//        row_insider_cursor: 数据地址   
//        data：写入整形的指针(需转换为char *传入)
//        size: 忽略
//
//返回写入后向后移的指针
void *write_int(char *row_insider_cursor,char *data,int size)
{   

    int *temp_cursor = data;
    memcpy((int *)row_insider_cursor,temp_cursor,4);
    (char *)row_insider_cursor;
    row_insider_cursor+=sizeof(int);
    // printf("Now row_insider_sursor is %p\n",row_insider_cursor);
    printf("write cursor with int\n");
    return row_insider_cursor;
}

//函数说明：在指定地址往后写入一个浮点
//
//        row_insider_cursor: 数据地址   
//        data：写入浮点的指针(转换char *传入)
//        size: 忽略
//
//返回写入后向后移的指针
void *write_float(char *row_insider_cursor,char *data,int size) 
{
    float *temp_cursor = data;
    memcpy((float *)row_insider_cursor,temp_cursor,4);
    (char *)row_insider_cursor;
    row_insider_cursor+=sizeof(float);
    // printf("Now row_insider_sursor is %p\n",row_insider_cursor);
    printf("write cursor with float\n");
    return row_insider_cursor;
}

//函数说明：在指定地址写入定长字符串
//
//        row_insider_cursor: 数据地址
//        size：字符串大小
//        data[]：字符串内容
//
//返回写入后向后移的指针
void *write_char(char *row_insider_cursor,char *data,int size)
{
    memcpy(row_insider_cursor,data,size);
    printf("write cursor with char\n");
    row_insider_cursor += size*sizeof(char);
    return row_insider_cursor;
}

//函数说明：在数据库文件的指定地址读取指定大小的数据块
//
//        read_chunk_index：数据块在数据库文件中的地址
//        read_chunk_size：数据块大小
//
//返回一个指向读取数据块的指针
void *read_chunk(uint32_t read_chunk_index,int read_chunk_size)
{
    FILE *database_file_handle;
    database_file_handle = fopen64(database_file_path,"rb");
    char *read_chunk_handle;
    fread(read_chunk_handle,chunk_size,1,database_file_handle);
    fclose(database_file_handle);
    printf("read chunk\n");
    return read_chunk_handle;

}

//函数说明：在数据库文件的hiding地址写入指定大小的数据块
//        write_chunk_index：数据块在数据库文件中的地址
//        chunk_cursor：写入的数据块指针
//
void write_chunk(uint32_t write_chunk_index,char *chunk_cursor)
{
    FILE *database_file_handle;
    database_file_handle = fopen64(database_file_path,"wb+");
    fwrite(chunk_cursor,chunk_size,1,database_file_handle);
    printf("write chunk %p\n",chunk_cursor);
    fclose(database_file_handle);
    
}

//解析读入的语句 解析并在磁盘上生成表结构
//语句样例::CREATE_TABLE User -encode utf8 INT[id] VARHCAR[name]255 VRABOOL[enable]
//  CREATE_TABLE 表示建表
//  User 表示表名
//  -encode 声明表的编码格式
//  INT VARCHAR VARBOOL 三种支持的类型
//  (其中 VARCHAR 支持自定义不超过 255 长度 用空格表示)
//  [] 中为列名
void init_table(char statement[])
{
    char *p;
    table_head th;
//    p = (char *)malloc(sizeof(char));
    p = strtok(statement," ");
    while(p)
    {
        p = strtok(NULL, " ");
        printf("%s\n", p);
    }

}
// int main()
// {
//     char *row_cursor,*row_handle;
//     row_handle = (char *)malloc(9);
//     row_cursor = row_handle;
//     // printf("%p\n%p\n",row_handle,row_cursor);
//     printf("malloc done\n");
//     int in_int = 5;
//     int *t;
//     t = &in_int;
//     printf("%d\n",*t);
//     row_cursor = (char *)write_int(row_cursor,(char *)t,1);
//     printf("write cursor with 5\n");
//     write_char(row_cursor,"ABCD",5); 
//     printf("%p\n%p\n",row_handle,row_cursor);
//     row_cursor = (char *)read_int(row_handle,1);
//     row_cursor = (char *)read_char(row_cursor,5);
//     // printf("%d\n",*row_handle);
//     // printf("%c",*row_cursor);
//     // printf("%c",*row_cursor+1);
//     // printf("%c",*row_cursor+2);
//     // printf("%c\n",*row_cursor+3);
//     // int *temp_int = (int *)(read_int(row_handle));
//     // printf("%d\n",*temp_int);
//     write_chunk(0,row_handle);
//     free(row_handle);
//     row_handle = read_chunk(0,chunk_size);
//     row_cursor = (char *)read_int(row_handle,1);
//     row_cursor = (char *)read_char(row_cursor,5);
//     free(row_handle);

//     // free(row_cursor);

// }
int main()
{
    char str[255];
    //读入建表语句
    gets(str);
    char *inputBuffer;
    inputBuffer = str;
    printf("%s\n",str);
    init_table(inputBuffer);
}