#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"

#include "distributioncamera.h"

objectfeature::objectfeature(){}
objectfeature::objectfeature(int av,int mat,double th){avsize = av;matsize = mat;thrval = th;}//コンストラクタで値を設定する

distributionCamera::distributionCamera() : Camera(){init();}
distributionCamera::distributionCamera(int c_n) : Camera(c_n){init();}//カメラ番号設定．画像なら-1を入れる
distributionCamera::distributionCamera(int c_n,int c_x,int c_y,int r_r){centerx = c_x;centery = c_y;ranger = r_r;}//コンストラクタで範囲設定

void distributionCamera::init(){centerx = 580;centery = 500;ranger = 370;}//コンストラクタで指定しない場合に呼び出し　範囲を設定

//フィルター．平滑処理
void distributionCamera::filtering(objectfeature ob){
	cv::Mat bufm;
	cvtColor(diff,editimag,CV_BGR2GRAY);//差分情報を白黒化
	cv::threshold(editimag, editimag, 40, 255, cv::THRESH_BINARY);//二値化
	retimag = cv::Mat::zeros(editimag.rows,editimag.cols,cv::THRESH_BINARY);//黒背景
	retimag.copyTo(bufm);//bufmも黒背景に
	for(int ii = 0; ii<bufm.rows;ii++){
		for(int jj = 0;jj<bufm.cols;jj++){
			if(((ii-centery)*(ii-centery)+(jj-centerx)*(jj-centerx))<ranger*ranger){
				bufm.data[ii * bufm.step + jj] = editimag.data[ii * bufm.step + jj];//範囲内の情報コピー
			}else{
				bufm.data[ii * bufm.step + jj] = 0;//範囲外は黒に
			}
		}
	}
	int avsize = ob.avsize;//平滑化の範囲設定
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
				val = 255;//認識物は赤
			}else{
				val = 0;//赤抜き
			}
			if(((ii-centery)*(ii-centery)+(jj-centerx)*(jj-centerx))<ranger*ranger){
				editimag.data[ii * editimag.step + jj] = val;//出力シロ
				retimag.data[ii * retimag.step + jj*retimag.channels() + 2] = val;//領域内の認識物は赤．それ以外は赤抜けに
			}else{
				editimag.data[ii * editimag.step + jj] = 0;//出力クロ
			}
		}
	}
}

//散布すべき領域かどうかの判定．散布すべき座標はgopintに返ってくる
void distributionCamera::judge(objectfeature ob, std::vector<point> &gopoint){
	double val;
	int avsize = ob.matsize;//判定領域のマスの設定
	point xyp;//座標バッファ
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
				val = 0;//範囲外は黒に
			}
			for(int iii = ii-avsize/2;iii< ii + avsize/2+1;iii++){
				for(int jjj = jj-avsize/2;jjj< jj + avsize/2+1;jjj++){
					if(((iii-centery)*(iii-centery)+(jjj-centerx)*(jjj-centerx))<ranger*ranger){
						editimag.data[iii * editimag.step + jjj] = (int)val;
						if(val>=255){
							retimag.data[iii * retimag.step + jjj*retimag.channels() + 0] = 0;
							retimag.data[iii * retimag.step + jjj*retimag.channels() + 1] = val;//散布すべき領域を緑色に
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

//表示
void distributionCamera::show(){
	//cv::imshow("result",editimag);//白黒で結果表示
	//cv::waitKey(1);
	cv::imshow("with image",retimag);//画像に色追加で表示
	cv::waitKey(1);	
}

//保存
void distributionCamera::write(){
	cv::imwrite(outpname,retimag);//画像の保存
}
