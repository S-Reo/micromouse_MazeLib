/*
 * WallDetect.c
 *
 *  Created on: 2022/08/18
 *      Author: leopi
 */
//#include "MicroMouse.h"

//フォトセンサをどう持ってくるか
    //従来のMicroMouse.hは使いたくない
#include "WallDetect.h"
static float photo_data[4] = {0};
static _Bool flag=false;
//バックグラウンドでセットを呼ぶ
void setWall()
{
	//処理後のPhoto[]を配列で渡す
	photo_data[0] = Photo[FL];
	photo_data[1] = Photo[FR];
	photo_data[2] = Photo[SL];
	photo_data[3] = Photo[SR];
	flag = true;
}
//getしたらresetする
_Bool getWall(float* wall_data)//配列のアドレスを渡す
{
	if(flag == true)
	{
		for(int i=0; i < 4; i++)
		{
			*wall_data = photo_data[i];
			wall_data += sizeof(*wall_data);
		}
		return true;
	}
	else
	{
		return false;
	}

}
//フォアグラウンドでリセットを呼ぶ
void resetWall()
{
	flag = false;
	//配列をリセット
	photo_data[0] = 0;
	photo_data[1] = 0;
	photo_data[2] = 0;
	photo_data[3] = 0;
}


