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
	    cam->read(imagename.at(0),imagename.at(ii));//差分入力
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
	cam->read(imagename.at(0),imagename.at(num));//差分入力
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
    
	Objectfeature obonion(21,121,20);//玉ねぎ用の平滑サイズ・判定サイズ・閾値
	Objectfeature obcone(21,61,20);//トウモコロシ用のサイズ・判定サイズ・閾値
	Objectfeature obbroccoli(21,121,20);//ブロッコリー用のサイズ・判定サイズ・閾値
	Objectfeature obcheese(21,41,20);//チーズ用のサイズ・判定サイズ・閾値

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
                                std::cout << "        input number 1-3" << std::endl;
                                scanf("%d",&imnum);
                                run_show(onion,imnum,obonion,cam);              
                                break;
                            case 'c':
                                flag2 = 0;
                                std::cout << "        input number 1-4" << std::endl;
                                scanf("%d",&imnum);
                                run_show(cone,imnum,obcone,cam);
                                break;
                            case 'b':
                                flag2 = 0;
                                std::cout << "        input number 1-5" << std::endl;
                                scanf("%d",&imnum);
                                run_show(broccoli,imnum,obbroccoli,cam);
                                break;
                            case 'h':
                                flag2 = 0;
                                std::cout << "        input number 1-3" << std::endl;
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