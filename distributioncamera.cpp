#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"

#include "camera.h"

typedef struct{
	cv::Scalar c_min;
	cv::Scalar c_max;
	int size;
} objectfeature;

typedef struct{
	int x;
	int y;
} point;


class distributionCamera : public Camera{
    protected:
		cv::Mat editimag,retimag;
		point *gp;
		int gpnum;
		int centerx,centery,ramger;
    public:
        distributionCamera();
        distributionCamera(int c_n);
		void init();
		void filtering(objectfeature ob);
		void judge(objectfeature ob,point *p,int &n);
		void show() override;
		void write();
};

distributionCamera::distributionCamera() : Camera(){init();}
distributionCamera::distributionCamera(int c_n) : Camera(c_n){init();}

void distributionCamera::init(){
	gpnum = 0;
	centerx = 540;
	centery = 500;
	ramger = 400;
}


void distributionCamera::filtering(objectfeature ob){
	cv::Mat gray;
	gpnum = 0;
	frame.copyTo(retimag);
	int size = ob.size/10;
	if(size%2==0){size+=1;}
	inRange(frame, ob.c_min, ob.c_max, gray);
	editimag = cv::Mat::zeros(gray.rows, gray.rows, CV_8UC3);;
	for (int y = size/2; y < editimag.rows - size/2;y = y + size) {
		for (int x = size/2; x < editimag.cols - size/2 ;x = x + size) {
			int val = 0;
			for (int yy = y - size/2; yy <= y + size/2; yy++) {
				for (int xx = x - size/2; xx <= x + size/2; xx++) {
					for (int ii = 0;ii < editimag.channels();ii++) { //B,G,R画像ごとに処理する
						val += gray.data[yy * gray.step + xx * gray.channels() + ii];
					}
				}
			}
			val = val/size/size/editimag.channels();
			if(val>35){
					val  = 0;
			}else{
					val = 255;
					gpnum++;
			}
			for (int ii = 0;ii < editimag.channels();ii++)
				for (int yy = y - size/2; yy <= y + size/2; yy++) {
					for (int xx = x - size/2; xx <= x + size/2; xx++) {
						if(((yy-centery)*(yy-centery) + (xx-centerx)*(xx-centerx))>ramger*ramger){
							editimag.data[yy * editimag.step + xx * editimag.channels() + ii] = 0;	
						}else{
							editimag.data[yy * editimag.step + xx * editimag.channels() + ii] = val;
							if(val==255){
								if(ii==2){
									retimag.data[yy * retimag.step + xx * editimag.channels() + ii] = val;
								}else{
									retimag.data[yy * retimag.step + xx * editimag.channels() + ii] = 0;
								}
							}
						}
					}
				}
			}
		}
	std::cout <<" imag row is " << editimag.rows << std::endl;
	std::cout <<" imag col is " << editimag.cols << std::endl;
}

void distributionCamera::judge(objectfeature ob,point *p,int &n){
	gp = new point[gpnum];

}

void distributionCamera::show(){
	//cv::imshow("result",editimag);//表示
	cv::imshow("result",retimag);//表示
	cv::waitKey(1);	
}

void write(){

}

#if 1
int main(int argh, char* argv[]){
	distributionCamera *cam;
	cam = new distributionCamera(-1);//-1は画象読み込み，0以上でカメラ番号
	//Camera *cam;
	//cam = new Camera(-1);//-1は画象読み込み，0以上でカメラ番号
	std::string imname = "image/pizza_1.jpeg";
	objectfeature obf;
	obf.c_min = cv::Scalar(100,100,100);//B, G, R
	obf.c_max = cv::Scalar(255,255,255);//B, G, R
	obf.size = 1000;
	cam->read(imname);
	cam->filtering(obf);
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