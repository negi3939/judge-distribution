#ifndef DISTRCAMERA_H_
#define DISTRCAMERA_H_

#include "camera.h"

class objectfeature{
	public:
		objectfeature();
		objectfeature(int av,int mat,double th);
		int avsize;
		int matsize;
		double thrval;
};

typedef struct{
	double x;
	double y;
} point;


class distributionCamera : public Camera{
    protected:
		cv::Mat editimag,retimag;
		 std::vector<point> gp;
		int centerx,centery,ranger;
    public:
        distributionCamera();
        distributionCamera(int c_n);
		void init();
		void filtering(objectfeature ob);
		void judge(objectfeature ob,std::vector<point> &gopoint);
		void show() override;//表示
		void write() override;//保存
};

#endif