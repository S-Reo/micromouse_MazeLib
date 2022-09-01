#include <stdio.h>
#include <stdbool.h>
#include <stdint-gcc.h>


#include "MazeLib.h"

#define DEBUG_ON    0
#define SIMULATION  1

#if SIMULATION
    #include "MazeSimulation.h"
#endif

//モード切替 : シミュレーションの関数を埋め込み、コメントアウト切り替えで実機の処理と入れ替え可能にする。（実機にSimulationライブラリを入れたくないため）
#define LEFTHAND_SEARCH 1

simulation test;

void Search()
{   
    //迷路の初期化
    printf("0\r\n");
    maze_node my_maze;
    initMaze(&my_maze);
        printf("自分の迷路の初期化の確認");
        printMatrix16ValueFromNode(&my_maze);
        printf("%ld\r\n",sizeof(my_maze));
        printAllNode(&my_maze);
    printf("1\r\n");
    //状態の初期化
    profile Mouse;
    initProfile(&Mouse);
        //printProfile(&Mouse);
    printf("2\r\n");
    //走らせる
    #if SIMULATION 
        //最初の加速
    #else 
        //実環境走行 : //最初の61.5mmの加速コマンドを発行
    #endif

    shiftState(&Mouse);
    //printf("3\r\n");
        //test用の迷路を参照 test.virtual_maze
        //斜め走行シミュレーションのための位置取得はどうするか
            //MATLABはdouble型なので、位置を小数点付きで指定すればいい
            //こちらのポジションを変換する
            //滑らかに描画したいので、matplotlib? とりあえず細かく移動量を作って、limitrateを付けたらどうか.setで小数点レベルで位置を動かしてやる
        //物理データはどうするかROSで通信したらいいかも?とりあえず保留

        //既に書いたやつ使う
        //1. 壁の更新タイミングが来るまで待機
        //2. 来たら、方角とか向きとか現在座標とか更新してから、壁を更新
        //3. 迷路の計算
        //3. 次の座標と方角をセット
        //3. コマンドを発行
        //4. プラスアルファの計算して、1に戻る
        //5. 探索が終わったら、フラッシュ、合図、迷路の出力、デバッグデータなど。
        printProfile(&Mouse);
        while( ! ((Mouse.goal_lesser.x <= Mouse.now.pos.x && Mouse.now.pos.x <= Mouse.goal_larger.x) && (Mouse.goal_lesser.y <= Mouse.now.pos.y && Mouse.now.pos.y <= Mouse.goal_larger.y)) )
        {
            static int count=0;
            #if LEFTHAND_SEARCH || RIGHTHAND_SEARCH
                if(Mouse.next.pos.x == 0 && Mouse.next.pos.y == 0)
                {
                    printf("break; 一周にかかった歩数 : %d\r\n",count);
                    break;
                }
                    
                    //return false;
            #endif

            //シミュレータではアニメーションをとりあえず置いておき、最短経路がどうなるかとかを確認する
            //2. 方角、座標の更新
                shiftState(&Mouse);
                //printf("4\r\n");
            //2. 壁の更新
                //今向いている方向に応じて、前右左をとる（後ろはかならず無し.）かならず013前右左, 3後ろ. 値は01のどちらかしかない
                wall_state wall[4]={0};
                /*（シミュレーションと実環境走行を切り替え）*/
                #if SIMULATION 
                    //配列と現在情報から、東西南北の配列を求めて渡す
                    
                    #if DEBUG_ON
                        printf("壁の状態0 %d, %d, %d, %d\r\n", wall[0], wall[1], wall[2], wall[3]);
                        printf("チェック: %d\r\n",convert16ValueToWallDirection_Simulation(&test, &(Mouse.now), &wall[0]));
                        printf("壁の状態1 %d, %d, %d, %d\r\n", wall[0], wall[1], wall[2], wall[3]);
                    #else
                        convert16ValueToWallDirection_Simulation(&test, &(Mouse.now), &wall[0]);
                        
                    #endif
                #else 
                    //実環境走行 : センサデータを持ってきて、閾値で判断したものをwallに代入
                    //Photo[]を得るためのWallDetectライブラリを使う
                #endif
                //前右左の情報を配列に入れて持ってくる
                getWallNow(&(Mouse.now), &wall[0]);    //前後左右のデータを自分の現在壁情報に反映
                #if DEBUG_ON
                    printf("壁の状態2 %d, %d, %d, %d\r\n", wall[0], wall[1], wall[2], wall[3]);
                #endif
                //2. 現在壁情報を、Mazeに反映
                updateNodeThree(&my_maze, &(Mouse.now), Mouse.now.pos.x, Mouse.now.pos.y);
            #if SIMULATION 
                    //機体から出力するためにデータをセットする処理を呼ぶ
                    //flagじゃなくて、drawに入れる
                    updateNodeDraw(&my_maze, Mouse.now.pos.x, Mouse.now.pos.y);
            #endif

            #if DEBUG_ON
                printf("壁の状態3 %d, %d, %d, %d\r\n", wall[0], wall[1], wall[2], wall[3]);
            #endif
            //ここから肝心のアルゴリズム
            #if LEFTHAND_SEARCH
                //単純な左手法
                #if DEBUG_ON
                    printf("壁の状態4 %d, %d, %d, %d\r\n", wall[0], wall[1], wall[2], wall[3]);
                #endif
                //スタート座標に戻ってきてしまったら停止。左手法で解けない迷路。
                if(wall[left] == NOWALL)
                {
                    //現在の方角に合わせてxyを更新する
                    switch (Mouse.now.car)
                    {
                    case north:
                        Mouse.next.car = west;
                        break;
                    case east:
                        Mouse.next.car = north;
                        break;
                    case south:
                        Mouse.next.car = east;
                        break;
                    case west:
                        Mouse.next.car = south;
                        break;
                    default:
                        break;
                    }
                }
                else if(wall[front] == NOWALL)
                {
                    switch (Mouse.now.car)
                    {
                    case north:
                        Mouse.next.car = north;
                        break;
                    case east:
                        Mouse.next.car = east;
                        break;
                    case south:
                        Mouse.next.car = south;
                        break;
                    case west:
                        Mouse.next.car = west;
                        break;
                    default:
                        break;
                    }
                }
                else if(wall[right] == NOWALL)
                {
                    switch (Mouse.now.car)
                    {
                    case north:
                        Mouse.next.car = east;
                        break;
                    case east:
                        Mouse.next.car = south;
                        break;
                    case south:
                        Mouse.next.car = west;
                        break;
                    case west:
                        Mouse.next.car = north;
                        break;
                    default:
                        break;
                    }
                }
                else //back
                {
                    //Uターン
                    switch (Mouse.now.car)
                    {
                    case north:
                        Mouse.next.car = south;
                        break;
                    case east:
                        Mouse.next.car = west;
                        break;
                    case south:
                        Mouse.next.car = north;
                        break;
                    case west:
                        Mouse.next.car = east;
                        break;
                    default:
                        break;
                    }
                }
                setNextPosition(&(Mouse.next));

            #elif RIGHTHAND_SEARCH
                if(wall[right] == NOWALL)
                {
                    //現在の方角に合わせてxyを更新する
                    switch (Mouse.now.car)
                    {
                    case north:
                        Mouse.next.car = east;
                        break;
                    case east:
                        Mouse.next.car = south;
                        break;
                    case south:
                        Mouse.next.car = west;
                        break;
                    case west:
                        Mouse.next.car = north;
                        break;
                    default:
                        break;
                    }
                }
                else if(wall[front] == NOWALL)
                {
                    switch (Mouse.now.car)
                    {
                    case north:
                        Mouse.next.car = north;
                        break;
                    case east:
                        Mouse.next.car = east;
                        break;
                    case south:
                        Mouse.next.car = south;
                        break;
                    case west:
                        Mouse.next.car = west;
                        break;
                    default:
                        break;
                    }
                }
                else if(wall[left] == NOWALL)
                {
                    switch (Mouse.now.car)
                    {
                    case north:
                        Mouse.next.car = west;
                        break;
                    case east:
                        Mouse.next.car = north;
                        break;
                    case south:
                        Mouse.next.car = east;
                        break;
                    case west:
                        Mouse.next.car = south;
                        break;
                    default:
                        break;
                    }

                }
                else //back
                {
                    //Uターン
                    switch (Mouse.now.car)
                    {
                    case north:
                        Mouse.next.car = south;
                        break;
                    case east:
                        Mouse.next.car = west;
                        break;
                    case south:
                        Mouse.next.car = north;
                        break;
                    case west:
                        Mouse.next.car = east;
                        break;
                    default:
                        break;
                    }
                }
                setNextPosition(&(Mouse.next));
            #elif ADACHI_SEARCH
                //最短経路を求めながら走る
                //求め方にも流派がある.
                //全ノードの重みの計算

                //次のノードを決定
                //最大3つのノードを比較。行けるところがなければUターン

                //現ノードと次ノードの情報から、
            #elif DEPTH_SEARCH
                //深さ優先探索
            #elif WIDTH_SEARCH
                //幅優先探索 
            #endif
        #if DEBUG_ON
            printf("8\r\n");
            
        #endif
        #if SIMULATION
            printProfile(&Mouse);
        #endif
        //途中でアニメーション用の軌跡ログの関数を入れておく
        
        //軌跡ログをMATLABに出力する
            //ある座標において斜め走行か否か
            //ある座標において加速したか否か
            //軌道と速度がアニメーションされればいい
            //あとは目標座標までを色付けしたり
            
        
        //MATLABかpythonかで描画する
        //matplotlibがアニメーション細かくて速そう
        //python連携ならROS? 軌跡ログ取らないで計算しながら描画
        //各種情報を表示する
            //探索にかかった歩数、時間、
            //最短経路の候補とその歩数、時間、
            //評価値マップの可視化
            //電流消費量の見積もりは？（センサのオンオフとオン時の消費量。モータの....エンコーダの...etc)
        // printMatrix16Value(&maze);
        // printf("%ld\r\n",sizeof(maze));
        count ++;
        }

    #if SIMULATION 
        //出来上がった迷路を出力する
        printf("得られた迷路\r\n");
        printAllNode(&(my_maze));
        //printf("9\r\n");
        printMatrix16ValueFromNode(&(my_maze)); //自分の迷路を更新していなかった
        //printf("10\r\n");
    #else 
        //実環境走行 : //最初の61.5mmの加速コマンドを発行
        //フラッシュ
        //合図

        //待機
        //迷路出力
        printMatrix16ValueFromNode(&my_maze);
    #endif
    printf("終了\r\n");
    //break;
    //return true;
}

_Bool Simulation()
{
    //外部から迷路をインポートして走らせる

    //MATLABもしくは実機走行により作った迷路テキストをインポート
    
    initMaze(&(test.virtual_maze));
    #if DEBUG_ON
    printMatrix16ValueFromNode(&(test.virtual_maze));//OK
    #endif

    if(getFileData(&test) == true) //OK
    {
        printf("ファイル読み込みに成功しました\r\n");        
    }
    else
    {
        printf("ファイル読み込みに失敗しました\r\n");
        return false;
    }
    //参照用の仮想迷路データに変換
    getNodeFrom16Value_Simulation(&test);

    //確認 : フォーマットOK
    #if DEBUG_ON
        printf("仮想迷路の");
        printAllNode(&(test.virtual_maze));
        printf("仮想迷路の");
        printMatrix16ValueFromNode(&(test.virtual_maze));
    #endif

    /* ここでアルゴリズムを試し書きする */
    printf("探索関数\r\n");
    Search();
    return true;
    
}
int main()
{
    //外部から迷路をインポートして走らせる
    if(Simulation() == true)
    {
        printf("完了\r\n");
    }
    else
    {
        printf("失敗\r\n");
    }

    return 0;
}