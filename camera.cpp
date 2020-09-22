#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"

#include "camera.h"

Camera::Camera(){
	cap = new cv::VideoCapture(0);//camera open
	if(!cap->isOpened()){
		std::cout << "open fail" << std::endl;
		exit(0);
	}
}

Camera::Camera(int c_n){
	if(c_n>=0){//負の番号を指定するとcapをnewしない
		cap = new cv::VideoCapture(c_n);//camera open
		if(!cap->isOpened()){
			std::cout << "open fail" << std::endl;
			exit(0);
		}
	}
}

int Camera::read(){
	return cap->read(frame);
}

int Camera::read(std::string &imname){
	int ret = 1;
	frame = cv::imread(imname,ret);
	return ret;
}

void Camera::show(){
	cv::imshow("camera",frame);//表示
	cv::waitKey(1);
}

int Camera::kbhit(){
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}



Camera::~Camera(){
	cv::destroyAllWindows();
}

#if 0
int main(int argh, char* argv[]){
	Camera *cam;
	cam = new Camera(-1);//-1は画象読み込み，0以上でカメラ番号
	std::string imname = "image/pizza_0.jpeg";
	cam->read(imname);
	while(1){
		cam->show();
		if(cam->kbhit()){
			break;
		}
	}
	delete cam;
	return 0;
}
#endif
