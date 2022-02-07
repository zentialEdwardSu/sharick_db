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
//                     ���汣��             ����BUG
//            ��Ի:
//                     д��¥��д�ּ䣬д�ּ������Ա��
//                     ������Աд�������ó��򻻾�Ǯ��
//                     ����ֻ���������������������ߣ�
//                     ��������ո��գ����������긴�ꡣ
//                     ��Ը�������Լ䣬��Ը�Ϲ��ϰ�ǰ��
//                     ���۱������Ȥ���������г���Ա��
//                     ����Ц��߯��񲣬��Ц�Լ���̫����
//                     ��������Ư���ã��ĸ���ó���Ա��
}

//����˵������ָ���ĵ�ַ��ȡһ������
//
//        row_insider_cursor: ���ݵ�ַ
//        size: ����
//
//���ض�ȡ������Ƶ�ָ��
void *read_int(char *row_insider_cursor,int size)
{
    char *int_data = (char *)malloc(sizeof(int));
    memcpy(int_data,row_insider_cursor,sizeof(int));
    printf("%6d",*int_data);
    row_insider_cursor += 4;
    free(int_data);
    return row_insider_cursor;
}

//����˵������ָ���ĵ�ַ��ȡһ������
//
//        row_insider_cursor: ���ݵ�ַ
//        size: ����
//
//���ض�ȡ������Ƶ�ָ��
void *read_float(char *row_insider_cursor,int size)
{
    char *float_data = (char *)malloc(sizeof(float));
    memcpy(float_data,row_insider_cursor,sizeof(float));
    printf("%f",*float_data);
    row_insider_cursor += 4;
    free(float_data);
    return row_insider_cursor;
}

//����˵������ָ����ַ��ȡ�����ַ���
//
//        row_insider_cursor: ���ݵ�ַ
//        size: �ַ�������
//
//���ض�ȡ������Ƶ�ָ��
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

//����˵������ָ����ַ����д��һ������
//
//        row_insider_cursor: ���ݵ�ַ   
//        data��д�����ε�ָ��(��ת��Ϊchar *����)
//        size: ����
//
//����д�������Ƶ�ָ��
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

//����˵������ָ����ַ����д��һ������
//
//        row_insider_cursor: ���ݵ�ַ   
//        data��д�븡���ָ��(ת��char *����)
//        size: ����
//
//����д�������Ƶ�ָ��
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

//����˵������ָ����ַд�붨���ַ���
//
//        row_insider_cursor: ���ݵ�ַ
//        size���ַ�����С
//        data[]���ַ�������
//
//����д�������Ƶ�ָ��
void *write_char(char *row_insider_cursor,char *data,int size)
{
    memcpy(row_insider_cursor,data,size);
    printf("write cursor with char\n");
    row_insider_cursor += size*sizeof(char);
    return row_insider_cursor;
}

//����˵���������ݿ��ļ���ָ����ַ��ȡָ����С�����ݿ�
//
//        read_chunk_index�����ݿ������ݿ��ļ��еĵ�ַ
//        read_chunk_size�����ݿ��С
//
//����һ��ָ���ȡ���ݿ��ָ��
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

//����˵���������ݿ��ļ���hiding��ַд��ָ����С�����ݿ�
//        write_chunk_index�����ݿ������ݿ��ļ��еĵ�ַ
//        chunk_cursor��д������ݿ�ָ��
//
void write_chunk(uint32_t write_chunk_index,char *chunk_cursor)
{
    FILE *database_file_handle;
    database_file_handle = fopen64(database_file_path,"wb+");
    fwrite(chunk_cursor,chunk_size,1,database_file_handle);
    printf("write chunk %p\n",chunk_cursor);
    fclose(database_file_handle);
    
}

//������������ �������ڴ��������ɱ�ṹ
//�������::CREATE_TABLE User -encode utf8 INT[id] VARHCAR[name]255 VRABOOL[enable]
//  CREATE_TABLE ��ʾ����
//  User ��ʾ����
//  -encode ������ı����ʽ
//  INT VARCHAR VARBOOL ����֧�ֵ�����
//  (���� VARCHAR ֧���Զ��岻���� 255 ���� �ÿո��ʾ)
//  [] ��Ϊ����
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
    //���뽨�����
    gets(str);
    char *inputBuffer;
    inputBuffer = str;
    printf("%s\n",str);
    init_table(inputBuffer);
}