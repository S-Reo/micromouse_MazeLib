/* MATLABでゲットした壁データを、c言語で読み取る*/
/* 走行後にMATLABに迷路を表示してもらうための変換処理*/
/* 変換はここでまとめて書いたほうがいい */
// MazeLibとMazeSimulationのインターフェース
#include <stdio.h>
#include<stdint-gcc.h>
//一辺の区画数
//
#define NUMBER_OF_SQUARES 9

typedef struct{
    uint8_t north:2;
    uint8_t east:2;
    uint8_t south:2;
    uint8_t west:2;
    uint8_t hosu;
}t_wall;

t_wall Wall [NUMBER_OF_SQUARES][NUMBER_OF_SQUARES];

// void Maze_MATtoC(uint16_t *Maze)
// {
//     //壁データ配列を取得
//         //ファイルオープン
    

//     //座標ごとに16進数に直す
//     int Value16[N][N] = {0};
//     for(int x=0; x < N; x ++)
//     {
//         for(int y=0; y < N; y++)
//         {
//             Value16[x][y] = Maze[x][y].north + 2*Maze[x][y].east + 4*Maze[x][y].south + 8*Maze[x][y].west;
//         }
//     }
//     //順番に注意してファイルに出力する
//         //ファイル名は、
  
// }
#include <windows.h>
#include <Commdlg.h>
#include <stdio.h>

// 返回值: 成功 1, 失败 0
// 通过 path 返回获取的路径
int FileDialog(char *path)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); // 结构大小
    ofn.lpstrFile = path; // 路径
    ofn.nMaxFile = MAX_PATH; // 路径大小
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0"; // 文件类型
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	return GetOpenFileName(&ofn);
}

int main(char argc, char *argv[])
{
	char szFile[MAX_PATH] = {0};
	if(FileDialog(szFile))
	{
		puts(szFile);
	}
	getchar();
    return 0;
}



