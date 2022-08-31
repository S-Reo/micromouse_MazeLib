/*
 * WallDetect.h
 *
 *  Created on: 2022/08/19
 *      Author: leopi
 */

#ifndef INC_WALLDETECT_H_
#define INC_WALLDETECT_H_
#include <stdbool.h>
void setWall();
//getしたらresetする
_Bool getWall(float *wall_data);//配列のアドレスを渡す
//フォアグラウンドでリセットを呼ぶ
void resetWall();

#endif /* INC_WALLDETECT_H_ */
