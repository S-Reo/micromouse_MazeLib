/*
 * dataQueue.c
 *
 *  Created on: 2022/08/18
 *      Author: leopi
 */
#include "CommandQueue.h"
//
//#include<string.h>

void initQueue(Queue *myq)
{
    myq->max_num = DATA_NUM+1;
	myq->head = 0;
	myq->tail = DATA_NUM;
//	memset(myq->queue,'0',(myq->max_num));
}
_Bool setQueue(Queue *myq, Command data)
{
    if( (((myq->tail)+2) % myq->max_num) == myq->head)
    {
        //先頭の一個後ろにはデータを入れない
    	return false;
    }
    else
    {
        myq->queue[((myq->tail)+1)%myq->max_num] = data;//0が空き、1から。
        myq->tail = ((myq->tail)+1)%myq->max_num;
        return true;
    }
}
_Bool getQueue(Queue *myq, Command* data)
{
    if( ( ((myq->tail)+1) % (myq->max_num)) == (myq->head)) //キューが空であれば
    {
    	myq->head = myq->head;
    	return false;
    }
    else
    {
        *data = myq->queue[myq->head];
        myq->head = ((myq->head)+1)%(myq->max_num);
        return true;
    }
}

void ResetCommand(Command* command)
{
	command->FinishPhase = 1;
	command->Acceleration = 0;
	command->AngAccel = 0;
	command->Mileage = 0;
	command->Velocity = 0;
}
