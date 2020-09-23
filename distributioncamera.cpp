#include <iostream>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"

#include "camera.h"

typedef struct{
	int avsize;
	int matsize;
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
		int centerx,centery,ranger;
    public:
        distributionCamera();
        distributionCamera(int c_n);
		void init();
		void filtering(objectfeature ob);
		void judge(objectfeature ob);
		void show() override;//表示
		void write() override;//保存
};

distributionCamera::distributionCamera() : Camera(){init();}
distributionCamera::distributionCamera(int c_n) : Camera(c_n){init();}

void distributionCamera::init(){
	gpnum = 0;
	centerx = 580;
	centery = 500;
	ranger = 370;
}


void distributionCamera::filtering(objectfeature ob){
	cv::Mat bufm;
	cvtColor(diff,editimag,CV_BGR2GRAY);
	cv::threshold(editimag, editimag, 40, 255, cv::THRESH_BINARY);//二値化
	retimag = cv::Mat::zeros(editimag.rows,editimag.cols,cv::THRESH_BINARY);//黒背景
	retimag.copyTo(bufm);
	for(int ii = 0; ii<bufm.rows;ii++){
		for(int jj = 0;jj<bufm.cols;jj++){
			if(((ii-centery)*(ii-centery)+(jj-centerx)*(jj-centerx))<ranger*ranger){
				bufm.data[ii * bufm.step + jj] = editimag.data[ii * bufm.step + jj];
			}else{
				bufm.data[ii * bufm.step + jj] = 0;
			}
		}
	}
	int avsize = 21;
	frame.copyTo(retimag);
	int val;
	for(int ii = avsize/2; ii<editimag.rows-avsize/2+1;ii++){
		for(int jj = avsize/2;jj<editimag.cols-avsize/2+1;jj++){
			val = 0;
			for(int iii = ii-avsize/2;iii< ii + avsize/2+1;iii++){
				for(int jjj = jj-avsize/2;jjj< jj + avsize/2+1;jjj++){
					val += bufm.data[iii * bufm.step + jjj];
				}				
			}
			val = val/avsize/avsize;//avsize*avsizeの領域の平均
			if(val>40){
				val = 255;
			}else{
				val = 0;
			}
			if(((ii-centery)*(ii-centery)+(jj-centerx)*(jj-centerx))<ranger*ranger){
				editimag.data[ii * editimag.step + jj] = val;
				retimag.data[ii * retimag.step + jj*retimag.channels() + 2] = val;
			}else{
				editimag.data[ii * editimag.step + jj] = 0;
			}
		}
	}
}

void distributionCamera::judge(objectfeature ob){
	double val;
	gpnum = 0;
	int avsize = 81;
	for(int ii = avsize/2; ii<editimag.rows-avsize/2+1;ii=ii + avsize){
		for(int jj = avsize/2;jj<editimag.cols-avsize/2+1;jj = jj + avsize){
			val = 0;
			for(int iii = ii-avsize/2;iii< ii + avsize/2+1;iii++){
				for(int jjj = jj-avsize/2;jjj< jj + avsize/2+1;jjj++){
					val += editimag.data[iii * editimag.step + jjj];
				}			
			}
			val = val/avsize/avsize;//avsize*avsizeの領域の平均
			if(val<40){
				val = 255;
				if(((ii-centery)*(ii-centery)+(jj-centerx)*(jj-centerx))<ranger*ranger){
					gpnum++;
				}
			}else{
				val = 0;
			}
			for(int iii = ii-avsize/2;iii< ii + avsize/2+1;iii++){
				for(int jjj = jj-avsize/2;jjj< jj + avsize/2+1;jjj++){
					if(((iii-centery)*(iii-centery)+(jjj-centerx)*(jjj-centerx))<ranger*ranger){
						editimag.data[iii * editimag.step + jjj] = (int)val;
						if(val>=255){
							retimag.data[iii * retimag.step + jjj*retimag.channels() + 0] = 0;
							retimag.data[iii * retimag.step + jjj*retimag.channels() + 1] = val;
							retimag.data[iii * retimag.step + jjj*retimag.channels() + 2] = 0;
						}
					}else{
						editimag.data[iii * editimag.step + jjj] = 0;
					}
				}
			}
		}
	}
	//editimag.copyTo(retimag);
	gp = new point[gpnum];

}

void distributionCamera::show(){
	//cv::imshow("result",editimag);//表示
	cv::imshow("result",retimag);//表示
	cv::waitKey(1);	
}

void distributionCamera::write(){

}

#if 1
int main(int argh, char* argv[]){
	distributionCamera *cam;
	cam = new distributionCamera(-1);//-1は画象読み込み，0以上でカメラ番号
	//Camera *cam;
	//cam = new Camera(-1);//-1は画象読み込み，0以上でカメラ番号
	std::string imname1 = "image/pizza_0_0.jpg";
	std::string imname2 = "image/pizza_0_1.jpg";
	objectfeature obf;
	obf.avsize = 21;
	obf.matsize = 61;
	cam->read(imname1,imname2);//差分入力
	cam->filtering(obf);
	cam->judge(obf);
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