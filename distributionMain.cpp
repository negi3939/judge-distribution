#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"

#include "distributioncamera.h"

//全てのファイルを判定し，保存する
void run_judge(std::vector<std::string> imagename,Objectfeature ob,distributionCamera *cam){
    std::vector<point>  gop;//撒くべき場所の座標
    for(int ii=1;ii<imagename.size();ii++){
	    cam->read(imagename.at(0),imagename.at(ii),1);//差分入力
	    cam->filtering(ob);//二値化と平滑化
	    cam->judge(ob,gop);//エリア内の散布度判定
        cam->write();//保存
        std::cout << " In" << imagename.at(ii) << " image " << std::endl;
        for(int jj=0;jj<gop.size();jj++){
		    std::cout << jj << " : " << " x: "<< gop.at(jj).x  << " y: "<< gop.at(jj).y  << std::endl;//取得した散布すべき座標の表示
	    }
    }
}

//特定のファイルを判定し，表示する
void run_show(std::vector<std::string> imagename,int num,Objectfeature ob,distributionCamera *cam){
    std::vector<point>  gop;//撒くべき場所の座標
	cam->read(imagename.at(0),imagename.at(num),1);//差分入力
	cam->filtering(ob);//二値化と平滑化
	cam->judge(ob,gop);//エリア内の散布度判定
    getchar();
    while(1){
		cam->show();//表示
		if(cam->kbhit()){//キーボードを入力すると表示停止
			break;
		}
	}
}

int main(int argh, char* argv[]){
    distributionCamera *cam;
	cam = new distributionCamera(-1);//-1は画象読み込み，0以上でカメラ番号
	std::vector<std::string> onion(7);
	onion.at(0) = "image/pizza_0_0.jpg";//ファイル名で指定
	onion.at(1) = "image/pizza_0_1.jpg";
	onion.at(2) = "image/pizza_0_2.jpg";
    onion.at(3) = "image/pizza_0_3.jpg";
    onion.at(4) = "image/pizza_0_4.jpg";
    onion.at(5) = "image/pizza_0_5.jpg";
	onion.at(6) = "image/pizza_1_0.jpg";
	std::vector<std::string> cone(9);
	cone.at(0) = onion.at(6);
	cone.at(1) = "image/pizza_1_1.jpg";
	cone.at(2) = "image/pizza_1_2.jpg";
	cone.at(3) = "image/pizza_1_3.jpg";
    cone.at(4) = "image/pizza_1_4.jpg";
    cone.at(5) = "image/pizza_1_5.jpg";
    cone.at(6) = "image/pizza_1_6.jpg";
    cone.at(7) = "image/pizza_1_7.jpg";
	cone.at(8) = "image/pizza_2_0.jpg";
	std::vector<std::string> broccoli(11);
	broccoli.at(0) = cone.at(8);
	broccoli.at(1) = "image/pizza_2_1.jpg";
	broccoli.at(2) = "image/pizza_2_2.jpg";
	broccoli.at(3) = "image/pizza_2_3.jpg";
	broccoli.at(4) = "image/pizza_2_4.jpg";
    broccoli.at(5) = "image/pizza_2_5.jpg";
    broccoli.at(6) = "image/pizza_2_6.jpg";
    broccoli.at(7) = "image/pizza_2_7.jpg";
    broccoli.at(8) = "image/pizza_2_8.jpg";
    broccoli.at(9) = "image/pizza_2_9.jpg";
	broccoli.at(10) = "image/pizza_3_0.jpg";
	std::vector<std::string> cheese(7);
	cheese.at(0) = broccoli.at(10);
	cheese.at(1) = "image/pizza_3_1.jpg";
	cheese.at(2) = "image/pizza_3_2.jpg";
	cheese.at(3) = "image/pizza_3_3.jpg";
    cheese.at(4) = "image/pizza_3_4.jpg";
    cheese.at(5) = "image/pizza_3_5.jpg";
    cheese.at(6) = "image/pizza_3_6.jpg";
    
    cv::Scalar colonion(175,200,216);//玉ねぎの色
    cv::Scalar colcone(65,170,197);//トウモロコシの色
    cv::Scalar colbroccoli(71,160,147);//ブロッコリーの色
    cv::Scalar colcheese(161,220,229);//チーズの色

	Objectfeature obonion(21,121,20,colonion,40);//玉ねぎ用の平滑サイズ・判定サイズ・閾値・色・色の閾値
	Objectfeature obcone(21,61,20,colcone,10);//トウモコロシ用のサイズ・判定サイズ・閾値・色・色の閾値
	Objectfeature obbroccoli(21,121,20,colbroccoli,40);//ブロッコリー用のサイズ・判定サイズ・閾値・色・色の閾値
	Objectfeature obcheese(21,41,20,colcheese,20);//チーズ用のサイズ・判定サイズ・閾値・色・色の閾値

    int flag0 = 1,flag1 = 1,flag2 = 1,imnum;
    while(flag0){
        std::cout << std::endl;
        std::cout << "=============   select mode   ==================== " << std::endl;
        std::cout << "  a : all file run and save" << std::endl;
        std::cout << "  s : show judge" << std::endl;
        std::cout << "  q : quit" << std::endl;
        flag1 = 1;
        while(flag1){
            switch(getchar()){
                case 'a':
                    flag1 = 0;
                    run_judge(onion,obonion,cam);//玉ねぎの処理
                    run_judge(cone,obcone,cam);//トウモコロシの処理
                    run_judge(broccoli,obbroccoli,cam);//ブロッコリーの処理
                    run_judge(cheese,obcheese,cam);//チーズの処理
                    break;
                case 's':
                    flag1 = 0;
                    std::cout << "    which image?" << std::endl;
                    std::cout << "      o : onion" << std::endl;
                    std::cout << "      c : cone" << std::endl;
                    std::cout << "      b : broccoli" << std::endl;
                    std::cout << "      h : cheese" << std::endl;
                    std::cout << "      q : quit" << std::endl;
                    flag2 = 1;
                    while(flag2){
                        switch(getchar()){
                            case 'o':
                                flag2 = 0;
                                std::cout << "        input number 1-6" << std::endl;
                                scanf("%d",&imnum);
                                run_show(onion,imnum,obonion,cam);              
                                break;
                            case 'c':
                                flag2 = 0;
                                std::cout << "        input number 1-8" << std::endl;
                                scanf("%d",&imnum);
                                run_show(cone,imnum,obcone,cam);
                                break;
                            case 'b':
                                flag2 = 0;
                                std::cout << "        input number 1-10" << std::endl;
                                scanf("%d",&imnum);
                                run_show(broccoli,imnum,obbroccoli,cam);
                                break;
                            case 'h':
                                flag2 = 0;
                                std::cout << "        input number 1-6" << std::endl;
                                scanf("%d",&imnum);
                                run_show(cheese,imnum,obcheese,cam);
                                break;
                            case 'q':
                                flag2 = 0;
                                break;
                            default:
                                break;
                        }
                   }
                   break;
                case 'q':
                    flag0 = 0;
                    flag1 = 0;
                    break;
                default :
                    break;
            }
        }
    }

	delete cam;
	return 0;
}