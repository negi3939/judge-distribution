#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"

#include "distributioncamera.h"

void run_judge(std::vector<std::string> imagename,objectfeature ob){
    distributionCamera *cam;
	cam = new distributionCamera(-1);//-1は画象読み込み，0以上でカメラ番号

    std::vector<point>  gop;//撒くべき場所の座標
    for(int ii=1;ii<imagename.size();ii++){
	    cam->read(imagename.at(0),imagename.at(ii));//差分入力
	    cam->filtering(ob);//二値化と平滑化
	    cam->judge(ob,gop);//エリア内の散布度判定
        cam->write();//保存
        std::cout << " In" << imagename.at(ii) << " image " << std::endl;
        for(int jj=0;jj<gop.size();jj++){
		    std::cout << jj << " : " << " x: "<< gop.at(jj).x  << " y: "<< gop.at(jj).y  << std::endl;//取得した散布すべき座標の表示
	    }
    }
    delete cam;
}

#if 1
int main(int argh, char* argv[]){
    distributionCamera *cam;
	cam = new distributionCamera(-1);//-1は画象読み込み，0以上でカメラ番号
	std::vector<std::string> onion(4);
	onion.at(0) = "image/pizza_0_0.jpg";//ファイル名で指定
	onion.at(1) = "image/pizza_0_1.jpg";
	onion.at(2) = "image/pizza_0_2.jpg";
	onion.at(3) = "image/pizza_1_0.jpg";
	std::vector<std::string> cone(5);
	cone.at(0) = onion.at(3);
	cone.at(1) = "image/pizza_1_1.jpg";
	cone.at(2) = "image/pizza_1_2.jpg";
	cone.at(3) = "image/pizza_1_3.jpg";
	cone.at(4) = "image/pizza_2_0.jpg";
	std::vector<std::string> broccoli(6);
	broccoli.at(0) = cone.at(4);
	broccoli.at(1) = "image/pizza_2_1.jpg";
	broccoli.at(2) = "image/pizza_2_2.jpg";
	broccoli.at(3) = "image/pizza_2_3.jpg";
	broccoli.at(4) = "image/pizza_2_4.jpg";
	broccoli.at(5) = "image/pizza_3_0.jpg";
	std::vector<std::string> cheese(4);
	cheese.at(0) = broccoli.at(5);
	cheese.at(1) = "image/pizza_3_1.jpg";
	cheese.at(2) = "image/pizza_3_2.jpg";
	cheese.at(3) = "image/pizza_3_3.jpg";
	objectfeature obonion(21,121,20);//玉ねぎ用のサイズ
	objectfeature obcone(21,61,20);//トウモコロシ用のサイズ
	objectfeature obbroccoli(21,121,20);//ブロッコリー用のサイズ
	objectfeature obcheese(21,41,20);//チーズ用のサイズ

    run_judge(onion,obonion);//玉ねぎの処理
    run_judge(cone,obcone);//トウモコロシの処理
    run_judge(broccoli,obbroccoli);//ブロッコリーの処理
    run_judge(cheese,obcheese);//チーズの処理

    std::cout << "Would you like to show image?" << std::endl;
    if(getchar()=='y'){
        getchar();
        std::vector<point>  gop;//撒くべき場所の座標
	    cam->read(cone.at(0),cone.at(1));//差分入力
	    cam->filtering(obcone);//二値化と平滑化
	    cam->judge(obcone,gop);//エリア内の散布度判定
	    while(1){
		    cam->show();//表示
		    if(cam->kbhit()){//キーボードを入力すると表示停止
			    break;
		    }
	    }
    }
	delete cam;
	return 0;
}
#endif