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
	return cap->read(frame);//カメラから取得
	outpname = "imageout/camaraimage.jpg";
}

int Camera::read(std::string &imname){
	int ret = 1;
	frame = cv::imread(imname,ret);//画像の読み込み
	cv::resize(frame, frame, cv::Size(1200, 1200));//画像の読み込み
	return ret;
	setoutpname(imname);//出力ファイル名の設定
}

int Camera::read(std::string &imname1,std::string &imname2){
	int ret = 1;
	cv::Mat buf1,buf2;
	buf1 = cv::imread(imname1,ret);
	buf2 = cv::imread(imname2,ret);
	cv::resize(buf1, buf1, cv::Size(1200, 1200));//画像の大きさを統一
	cv::resize(buf2, buf2, cv::Size(1200, 1200));//画像の大きさを統一
	buf2.copyTo(frame);//imname2をorgとする
	cv::absdiff(buf1,buf2,diff);//差分を取得
	setoutpname(imname2);//出力ファイル名の設定
	return ret;
}

void Camera::setoutpname(std::string &imname){
	std::string bufst0,bufst1,bufst2;
	bufst0 =  "imageout/";
	bufst1 = imname;
	bufst2 = "_output.jpg";
	int countst=0,countend=0;
	for(int ii=0;ii<bufst1.size();ii++){
		if(bufst1[ii]=='/'){break;}//ディレクトリ名の削除
		countst++;
	}
	for(int ii=0;ii<countst+1;ii++){
		bufst1.erase(bufst1.begin());
	}
	for(int ii=bufst1.size()-1;ii>=0;ii--){
		if(bufst1[ii]=='.'){break;}//拡張子の削除
		countend++;
	}
	for(int ii=0;ii<countend+1;ii++){
		bufst1.erase(bufst1.end()-1);
	}
	outpname = bufst0;
	outpname += bufst1;
	outpname += bufst2;
	std::cout << outpname << std::endl;
}

void Camera::show(){
	cv::imshow("camera",frame);//表示
	cv::waitKey(1);//これがないと表示されない
}

int Camera::kbhit(){//キーボード割り込み入力判定
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

void Camera::write(){
	cv::imwrite(outpname,frame);
}

Camera::~Camera(){
	cv::destroyAllWindows();
}

#if 0
int main(int argh, char* argv[]){
	Camera *cam;
	cam = new Camera(-1);//-1は画象読み込み，0以上でカメラ番号
	std::string imname1 = "image/pizza_0_0.jpg";
	std::string imname2 = "image/pizza_0_2.jpg";
	cam->read(imname1,imname2);//差分画像
	while(1){
		cam->show();//表示
		if(cam->kbhit()){//キーボードを入力すると表示停止
			break;
		}
	}
	delete cam;
	return 0;
}
#endif
