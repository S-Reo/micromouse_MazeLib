/*
 * CommandQueue.h
 *
 *  Created on: 2022/08/19
 *      Author: leopi
 */

#ifndef INC_COMMANDQUEUE_H_
#define INC_COMMANDQUEUE_H_

#include <stdbool.h>
typedef struct Command
{
	_Bool FinishPhase;						//フェイズ終了判定 終了していればtrue
	float Velocity;							//目標速度（mm/s）
	float Acceleration;						//加速度
	float Mileage;								//直線走行距離（mm）左右の走行距離の総和で計算。この距離到達時に目標速度となるように加速度を計算する。
	float Angle;								//目標角度（rad）
	float AngVelo;							//
	float AngAccel;
	float WallDetect_Start;
	float WallCorrectPos_Start;
	float WallCorrectPos_End;

}Command;

#define DATA_NUM 10
typedef struct Queue
{
	int head;
	int tail;
	int max_num;
	Command queue[DATA_NUM];
}Queue;
Queue cq;
Command com;
void initQueue(Queue *myq);
_Bool setQueue(Queue *myq, Command data);
_Bool getQueue(Queue *myq, Command* data);

void ResetCommand(Command* command);

#endif /* INC_COMMANDQUEUE_H_ */
