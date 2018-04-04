#include "predict.h"
#include "lib_io.h"
#include "lib_time.h"
#include "stdio.h"

int main(int argc, char *argv[])
{
    print_time("Begin");
	char *data[MAX_DATA_NUM];//最大数据
    char *info[MAX_INFO_NUM];//最大信息
	int data_line_num;
    int info_line_num;

#ifdef _DEBUG
    char *input_file = "../data/input1.txt";//输入文件
    char *data_file = "../data/TrainData_2015.1.1_2015.2.19.txt";//数据文件
    char *output_file = "../data/output.txt";//输出文件
#else
    char *data_file = argv[1];//数据文件
    char *input_file = argv[2];//输入文件
    char *output_file = argv[3];//输出文件
#endif

    //char *data_file = "../data/data_2016_1.txt";//数据文件
    data_line_num = read_file(data, MAX_DATA_NUM, data_file);//读取数据

    printf("data file line num is :%d \n", data_line_num);
    if (data_line_num == 0)
    {
        printf("Please input valid data file.\n");
        return -1;
    }

    info_line_num = read_file(info, MAX_INFO_NUM, input_file);//读取输入文件

    printf("input file line num is :%d \n", info_line_num);
    if (info_line_num == 0)
    {
        printf("Please input valid info file.\n");
        return -1;
    }

    predict_server(info, data, data_line_num, output_file);//预测

    release_buff(info, info_line_num);
	release_buff(data, data_line_num);

    print_time("End");

	return 0;
}

