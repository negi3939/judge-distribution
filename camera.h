#ifndef CAMERA_H_
#define CAMERA_H_
#include "opencv2/opencv.hpp"

class Camera{
	private:
	protected:
		cv::VideoCapture *cap;
		cv::Mat frame;
		int hogenum;	
	public:
		Camera();
		Camera(int c_n);
		~Camera();
		int read();
		int read(std::string &imname);
		int read(std::string &imname1,std::string &imname2);
		virtual void show();
		int kbhit();
};

#endif