#命令行实现进度条:命令行实现进度条.c
		#gcc 命令行实现进度条.c -g -o 命令行实现进度条

#clean:
	#rm 命令行实现进度条
	
#out:test.o a.o
	#gcc $^ -o $@ #$^:所有依赖对象

#%.o:%.c
	#gcc -c $< -o $@

bin=out#bin存储目标文件
src=$(wildcard *.cpp)#将.c文件存储在src中
obj=$(patsubst %.cpp,%.o,$(src))#将src中的.c文件转化为.o文件
G=-g

$(bin):$(obj)
	gcc $^ -o $@ 

%.o:%.cpp
	gcc -c $< -o $@ $G
