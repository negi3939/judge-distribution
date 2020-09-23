#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"

#include "distributioncamera.h"

distributionCamera::distributionCamera() : Camera(){init();}
distributionCamera::distributionCamera(int c_n) : Camera(c_n){init();}

objectfeature::objectfeature(){}
objectfeature::objectfeature(int av,int mat,double th){
	avsize = av;
	matsize = mat;
	thrval = th;
}

void distributionCamera::init(){
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
	int avsize = ob.avsize;
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
			if(val>ob.thrval){
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

void distributionCamera::judge(objectfeature ob, std::vector<point> &gopoint){
	double val;
	int avsize = ob.matsize;
	point xyp;
	for(int ii = avsize/2; ii<editimag.rows-avsize/2+1;ii=ii + avsize){
		for(int jj = avsize/2;jj<editimag.cols-avsize/2+1;jj = jj + avsize){
			val = 0;
			for(int iii = ii-avsize/2;iii< ii + avsize/2+1;iii++){
				for(int jjj = jj-avsize/2;jjj< jj + avsize/2+1;jjj++){
					val += editimag.data[iii * editimag.step + jjj];
				}			
			}
			val = val/avsize/avsize;//avsize*avsizeの領域の平均
			if(val<ob.thrval){
				val = 255;
				if(((ii-centery)*(ii-centery)+(jj-centerx)*(jj-centerx))<ranger*ranger){
					xyp.x = jj;//x座標の取得
					xyp.y = ii;//y座標の取得
					gp.push_back(xyp);//gpのvectorデータに追加
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
	gopoint = gp;
}

void distributionCamera::show(){
	//cv::imshow("result",editimag);//表示
	cv::imshow("result",retimag);//表示
	cv::waitKey(1);	
}

void distributionCamera::write(){
	cv::imwrite(outpname,retimag);
}


#if 0
int main(int argh, char* argv[]){
	distributionCamera *cam;
	cam = new distributionCamera(-1);//-1は画象読み込み，0以上でカメラ番号
	std::string onion[4];
	onion[0] = "image/pizza_0_0.jpg";//ファイル名で指定
	onion[1] = "image/pizza_0_1.jpg";
	onion[2] = "image/pizza_0_2.jpg";
	onion[3] = "image/pizza_1_0.jpg";
	std::string cone[5];
	cone[0] = onion[3];
	cone[1] = "image/pizza_1_1.jpg";
	cone[2] = "image/pizza_1_2.jpg";
	cone[3] = "image/pizza_1_3.jpg";
	cone[4] = "image/pizza_2_0.jpg";
	std::string broccoli[6];
	broccoli[0] = cone[4];
	broccoli[1] = "image/pizza_2_1.jpg";
	broccoli[2] = "image/pizza_2_2.jpg";
	broccoli[3] = "image/pizza_2_3.jpg";
	broccoli[4] = "image/pizza_2_4.jpg";
	broccoli[5] = "image/pizza_3_0.jpg";
	std::string cheese[4];
	cheese[0] = broccoli[5];
	cheese[1] = "image/pizza_3_1.jpg";
	cheese[2] = "image/pizza_3_2.jpg";
	cheese[3] = "image/pizza_3_3.jpg";
	cheese[4] = "image/pizza_3_4.jpg";
	objectfeature obonion(21,121,20);//玉ねぎ用のサイズ
	objectfeature obcone(21,61,20);//トウモコロシ用のサイズ
	objectfeature obbroccoli(21,121,20);//ブロッコリー用のサイズ
	objectfeature obcheese(21,41,20);//チーズ用のサイズ
	std::vector<point>  gop;//撒くべき場所の座標
	cam->read(onion[0],onion[1]);//差分入力
	cam->filtering(obonion);//二値化と平滑化
	cam->judge(obonion,gop);//エリア内の散布度判定
	std::cout << onion << "Number of gopoint is " << gop.size() << std::endl;//取得した散布すべき座標点の数
	for(int ii=0;ii<gop.size();ii++){
		std::cout << ii << " : " << " x: "<< gop.at(ii).x  << " y: "<< gop.at(ii).y  << std::endl;//取得した散布すべき座標の表示
	}
	cam->write();

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