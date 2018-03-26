#encoding:utf-8
import sys
import os

#使用matplotlib绘制简单的折线图
import matplotlib.pyplot as plt

#绘制简单的折线图

def main():
	
	file_path='../data/cpp_py.txt'

	arry=read_lines(file_path);

	list=[]
	if arry == None:
		return None

	for a in arry:
		x=a.split(" ")
		x = [ int( x ) for x in x if x ]
		list.append(x)
	
	paint(list)

def paint(list):
	for arry in list:

		plt.plot(arry[2:])   #plot函数根据数字绘制出有意义的图形e
		plt.title("Flavor Num", fontsize = 24)      #给图表指定标题
		plt.xlabel("Date", fontsize = 14)              #给X轴设置标题
		plt.ylabel("Num", fontsize = 14)    #给Y轴设置标题
		#plt.tick_params(axis = 'both', labelsize = 14)  

	plt.show()          #show函数打开matplotlib查看器,并显示绘制的图形



def read_lines(file_path):
    if os.path.exists(file_path):
        array = []
        with open(file_path, 'r') as lines:
            for line in lines:
                array.append(line[:-2])
        return array
    else:
        print 'file not exist: ' + file_path
        return None

if __name__=="__main__":
	main()

