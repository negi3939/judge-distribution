#ifndef DISTRCAMERA_H_
#define DISTRCAMERA_H_

#include "camera.h"

class Objectfeature{
	public:
		Objectfeature();
		Objectfeature(int av,int mat,double th,cv::Scalar co,double coth);//コンストラクタで設定
		int avsize;//平滑化処理用の範囲
		int matsize;//散布すべき領域のマスの大きさ
		double thrval;//画像差分のしきい値
		double colthval;//基準食との色のしきい値
		cv::Scalar col;//基準の色
};

typedef struct{
	double x;
	double y;
} point;//2D座標用


class distributionCamera : public Camera{
    protected:
		cv::Mat editimag,retimag;//editimagは白黒で散布すべき領域表示．retimagはカラーで認識物と散布すべき領域を表示
		std::vector<point> gp;//散布すべき領域の座標郡
		int centerx,centery,ranger;//範囲を円で決めるx,y,r　コンストラクタで設定
    public:
        distributionCamera();
        distributionCamera(int c_n);//c_nはカメラ番号．カメラじゃない場合は-1を入れる
        distributionCamera(int c_n,int c_x,int c_y,int r_r);//範囲を円で決めるx,y,r　コンストラクタで設定
		void init();//コンストラクタで指定しない場合に呼び出し
		void filtering(Objectfeature ob);//フィルター．平滑処理
		void judge(Objectfeature ob,std::vector<point> &gopoint);//散布すべき領域かどうかの判定．散布すべき座標はgopintに返ってくる
		void removenoize(Objectfeature ob);//色が合わないものを除去
		void show() override;//表示
		void write() override;//保存
};

#endif