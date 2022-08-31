#include "MazeLib.h"

//外からアクセスする関数. xyに制限が必要
_Bool setExistanceRawNode(maze_node *maze, uint8_t x, uint8_t y, wall_state et)
{
    if(x >= NUMBER_OF_SQUARES_X || y >= NUMBER_OF_SQUARES_Y || x < 0 || y < 0)
    {
        return false;
    }
    else
    {
        maze->RawNode[x][y].existence = et;
        maze->RawNode[x][y].flag = true;
        return true;
    }
}
_Bool setExistanceColumnNode(maze_node *maze, uint8_t x, uint8_t y, wall_state et)
{
    if(x >= NUMBER_OF_SQUARES_X || y >= NUMBER_OF_SQUARES_Y || x < 0 || y < 0)
    {
        return false;
    }
    else
    {
        maze->ColumnNode[x][y].existence = et;  
        maze->ColumnNode[x][y].flag = true;  
        return true;
    }
}

//wtが12ビットを超えるとまずい。後で考える
_Bool setWeightRawNode(maze_node *maze, uint8_t x, uint8_t y, uint16_t wt)
{
    if(x >= NUMBER_OF_SQUARES_X || y >= NUMBER_OF_SQUARES_Y || x < 0 || y < 0 || (wt < 0 || MAX_WEIGHT < wt) )
    {
        return false;
    }
    else
    {
        maze->RawNode[x][y].weight = wt;
        return true;
    }
}
_Bool setWeightColumnNode(maze_node *maze, uint8_t x, uint8_t y, uint16_t wt)
{
    if(x >= NUMBER_OF_SQUARES_X || y >= NUMBER_OF_SQUARES_Y || x < 0 || y < 0 || (wt < 0 || MAX_WEIGHT < wt) )
    {
        return false;
    }
    else
    {
        maze->ColumnNode[x][y].weight = wt;  
        return true;
    }
}
void initMaze(maze_node *maze)
{
    //まず未探索状態にする
    for(int i=0; i < NUMBER_OF_SQUARES_X; i++)
    {
        for(int j=1; j < NUMBER_OF_SQUARES_Y; j++)
        {
            maze->RawNode[i][j].existence = UNKNOWN;
            maze->RawNode[i][j].draw = false;//未知壁は描画のときに無いものとする？
            maze->RawNode[i][j].weight = 0;
            
        }
    }
    for(int i=1; i < NUMBER_OF_SQUARES_X; i++)
    {
        for(int j=0; j < NUMBER_OF_SQUARES_Y; j++)
        {
            maze->ColumnNode[i][j].existence = UNKNOWN;
            maze->ColumnNode[i][j].draw = false;
            maze->ColumnNode[i][j].weight = 0;
        }
    }
    
    // 壁の有無を初期化
    for(int i=0; i < NUMBER_OF_SQUARES_X; i++)
    {
        maze->RawNode[i][0].existence = WALL;                       //南壁すべて1
        maze->RawNode[i][NUMBER_OF_SQUARES_Y].existence = WALL;     //北壁すべて1
        maze->RawNode[i][0].weight = MAX_WEIGHT;                        //外壁すべてデフォルト値
        maze->RawNode[i][NUMBER_OF_SQUARES_Y].weight = MAX_WEIGHT;
        maze->RawNode[i][0].draw = true;                        
        maze->RawNode[i][NUMBER_OF_SQUARES_Y].draw = true;    
    }
    for(int j=0; j < NUMBER_OF_SQUARES_Y; j++)
    {
        maze->ColumnNode[0][j].existence = WALL;                    //西壁すべて1
        maze->ColumnNode[NUMBER_OF_SQUARES_X][j].existence = WALL;  //東壁すべて1
        maze->ColumnNode[0][j].weight = MAX_WEIGHT;                     
        maze->ColumnNode[NUMBER_OF_SQUARES_X][j].weight = MAX_WEIGHT;
        maze->ColumnNode[0][j].draw = true;                    
        maze->ColumnNode[NUMBER_OF_SQUARES_X][j].draw = true; 
    }
    maze->ColumnNode[1][0].existence = WALL;    //東1
    maze->RawNode[0][1].existence = NOWALL;     //北0

    maze->ColumnNode[1][0].weight = MAX_WEIGHT;    //東1
    maze->RawNode[0][1].weight = 0;     //北0

    maze->ColumnNode[1][0].draw = true;    //東1
    maze->RawNode[0][1].draw = false;     //北0
}
void printSingleNode(maze_node *mn, uint8_t x, uint8_t y)
{
    printf("行ノード %d, %d : 壁 %u, 重み %u, draw %u\r\n", x,y, mn->RawNode[x][y].existence,mn->RawNode[x][y].weight,mn->RawNode[x][y].draw);
    printf("列ノード %d, %d : 壁 %u, 重み %u, draw %u\r\n", x,y, mn->ColumnNode[x][y].existence,mn->ColumnNode[x][y].weight,mn->ColumnNode[x][y].draw);
}
//ノードの壁の有無はそのまま描画用データになる。外堀だけprintfしない
void printAllNode(maze_node *mn)
{
    printf("全ノード\r\n");
    //間違ってるかも
    //MATLABで保存するときと同じパターンで出力する
    //Raw
    //Column
    //Rawを1列出力し、改行せずColumnの1列出力。
    //行を増やして同じ処理
    for(int i=0; i < NUMBER_OF_SQUARES_X; i++)
    {
        for(int j=1; j < NUMBER_OF_SQUARES_Y+1; j++)
        {
            printf("%u,",mn->RawNode[i][j].draw);
        }
        for(int j=0; j < NUMBER_OF_SQUARES_Y; j++)
        {
            printf("%u",mn->ColumnNode[i+1][j].draw);
            if(j < NUMBER_OF_SQUARES_Y-1)
                printf(",");
        }
        printf("\r\n");
    }
    printf("\r\n");
}
static uint8_t convertNodeTo16Value(maze_node *maze, int x, int y)
{
    //xy座標を入力
    //各マスの16進数を出力。
    uint8_t val=0;
    val += 1 * maze->RawNode[x][y+1].draw;      //北
    val += 2 * maze->ColumnNode[x+1][y].draw;   //東
    val += 4 * maze->RawNode[x][y].draw;        //南
    val += 8 * maze->ColumnNode[x][y].draw;     //西    
    return val;
}

//機体からTeraTermで出力するための関数
void printMatrix16ValueFromNode(maze_node *maze)
{
    printf("機体からTeraTermへの出力用\r\n");
    for(int j=NUMBER_OF_SQUARES_Y-1; j >= 0; j--)
    {
        for(int i=0; i < NUMBER_OF_SQUARES_X; i++)
        {
            printf("%u",convertNodeTo16Value(maze, i,j));
            if(i < NUMBER_OF_SQUARES_X-1)
                printf(",");
        }
        printf("\r\n");
    }
    printf("\r\n");

}

//単ノードの更新
//壁の有無を更新。(drawはTeraTerm出力時でもいい）
//引数のwall_stが前右左
_Bool getWallNow(state *st, wall_state *wall_st)//wall_existence *wall[4])(
{
    switch (st->car)
    {
    case north:
        st->wall.north = wall_st[0];    //北
        st->wall.east = wall_st[1];     //東
        st->wall.south = wall_st[2];    //南
        st->wall.west = wall_st[3];     //西
        break;
    case east:
        st->wall.north = wall_st[3];    //北
        st->wall.east = wall_st[0];     //東
        st->wall.south = wall_st[1];    //南
        st->wall.west = wall_st[2];     //西
        break;
    case south:
        st->wall.north = wall_st[2];    //北
        st->wall.east = wall_st[3];     //東
        st->wall.south = wall_st[0];    //南
        st->wall.west = wall_st[1];     //西
        break;
    case west:
        st->wall.north = wall_st[1];    //北
        st->wall.east = wall_st[2];     //東
        st->wall.south = wall_st[3];    //南
        st->wall.west = wall_st[0];     //西
        break;
    default:
        //万が一斜めの方角を向いているときに呼び出してしまったら、
        return false;
        break;
    }
    return true;

}
//壁があれば重みはデフォルト値を代入する
//壁がなければそのままにしておく 前左右の情報の方角に合わせた変換は別のところで
void updateNodeThree(maze_node *maze, wall_state *wall, uint8_t x, uint8_t y)
{
    //壁の有無の更新。既知の壁には上書きしない。重みの更新は？既知かどうかは重複するから書かない
        //重みは？壁があればMAX値、なければそのまま   
    maze->RawNode[x][y+1].existence = (maze->RawNode[x][y+1].existence == UNKNOWN) ? wall[0] : maze->RawNode[x][y+1].existence;             //北
    maze->ColumnNode[x+1][y].existence = (maze->ColumnNode[x+1][y].existence == UNKNOWN) ? wall[1] : maze->ColumnNode[x+1][y].existence;    //東
    maze->RawNode[x][y].existence = (maze->RawNode[x][y].existence == UNKNOWN) ? wall[2] : maze->RawNode[x][y].existence;                   //南
    maze->ColumnNode[x][y].existence = (maze->ColumnNode[x][y].existence == UNKNOWN) ? wall[3] : maze->ColumnNode[x][y].existence;          //西

    maze->RawNode[x][y+1].flag = true;      //北
    maze->ColumnNode[x+1][y].flag = true;   //東
    maze->RawNode[x][y].flag = true;        //南
    maze->ColumnNode[x][y].flag = true;     //西

    maze->RawNode[x][y+1].weight = (maze->RawNode[x][y+1].weight == WALL) ? MAX_WEIGHT : maze->RawNode[x][y+1].weight;             //北
    maze->ColumnNode[x+1][y].weight = (maze->ColumnNode[x+1][y].weight == WALL) ? MAX_WEIGHT : maze->ColumnNode[x+1][y].weight;    //東
    maze->RawNode[x][y].weight = (maze->RawNode[x][y].weight == WALL) ? MAX_WEIGHT : maze->RawNode[x][y].weight;                   //南
    maze->ColumnNode[x][y].weight = (maze->ColumnNode[x][y].weight == WALL) ? MAX_WEIGHT : maze->ColumnNode[x][y].weight;          //西
}
//柱も使って壁の先取りをする
    //柱の周りのノードのうち3つに壁がなければ、残り一個を壁有にする

//全体の重みの更新（mazeの更新）
//指定座標を重み0とする
//外堀と、内側のデフォ値ノードを無視しながらノードの重みをゴールから加算していく
// void setGoalWeight(maze_node *maze, uint8_t x, uint8_t y)
// {
//     //ゴールエリアの外堀と中のノードは全て0
//     for(int i=0; i < GOAL_SIZE; i++)
//     {
//         for(int j=0; j < GOAL_SIZE; j++)
//         {
//             maze->RawNode[x+i][y+1+j].weight = 0;       //北
//             maze->ColumnNode[x+1+i][y+j].weight = 0;    //東
//             maze->RawNode[x+i][y+j].weight = 0;         //南
//             maze->ColumnNode[x+i][y+j].weight = 0;      //西
//         }
//     }
// }
void setTargetWeight(maze_node *maze, uint8_t x, uint8_t y, uint8_t target_size)
{
    //ゴールエリアの外堀と中のノードは全て0、壁があればMAX。
    for(int i=0; i < target_size; i++)
    {
        for(int j=0; j < target_size; j++)
        {
            maze->RawNode[x+i][y+1+j].weight = (maze->RawNode[x+i][y+1+j].weight == MAX_WEIGHT) ? MAX_WEIGHT : 0;       //北
            maze->ColumnNode[x+1+i][y+j].weight = (maze->ColumnNode[x+1+i][y+j].weight == MAX_WEIGHT) ? MAX_WEIGHT : 0; //東
            maze->RawNode[x+i][y+j].weight = (maze->RawNode[x+i][y+j].weight == MAX_WEIGHT) ? MAX_WEIGHT : 0;           //南
            maze->ColumnNode[x+i][y+j].weight = (maze->ColumnNode[x+i][y+j].weight == MAX_WEIGHT) ? MAX_WEIGHT : 0;     //西
        }
    }
}
void updateAllNodeWeight(maze_node *maze, uint8_t x, uint8_t y)
{
    //新しい区画に入ったときに、更新
    //初期化は要るのか
    //探索中は要らない
        //壁が無いもしくは未探索のノードだけで計算を行う
        //壁があるところは重みMAXで、ノードとして参照すらしなくていい
    //初期化を考える前に、ノードの重み計算を先に書いてしまおう
    
    //重みの計算開始はゴールエリアのノードかつ重みが0のもの
        //足立法でゴールしたらゴールエリアの重み0のノードが固定される. 
        //一回目は、重み0のノードをどうするか.(中は壁がないことが確定している)
    //ノードの座標の差を見て、斜めか直進かを決める
}

//向いている4方角とxy座標に合わせて壁の判定
//前左右の有無を書き込む

//補正に必要な壁情報
//向いている4方角とxy座標から、見るべきノードはわかるのでローカルに入れる(配列ポインタ？)




void initExistence(maze_node *maze)
{

}

/* ----- 探索者データ管理 ここから----- */
void setNextPosition(state *st)
{
    switch (st->car)
    {
    case north:
        st->pos.x += 0;
        st->pos.y += 1;
        /* code */
        break;
    case ne: //斜めのときの座標の変更条件をうまくやる
        if(1);
        st->pos.x += 1;
        st->pos.y += 0;
        
        st->pos.x += 0;
        st->pos.y += 1;
        break;
    case east:
        st->pos.x += 1;
        st->pos.y += 0; 
        break;
    case south:
        st->pos.x += 0;
        st->pos.y += -1;  
        /* code */
        break;
    case west:
        st->pos.x += -1;
        st->pos.y += 0; 
        break;
    
    default:
        printf("方角ミス\r\n");
        break;
    }
}
void setPosition(position *pos,uint8_t x, uint8_t y)
{
    pos->x = x;
    pos->y = y;
}
// void setTargetPosition()
// {

// }
void setGoal(profile *prof, uint8_t x, uint8_t y)
{
    //左下の座標と、ゴールサイズから、右上を求める
    setPosition( &(prof->goal_lesser) , x, y );
    setPosition( &(prof->goal_larger), x + GOAL_SIZE_X-1, y + GOAL_SIZE_Y-1 );
}
void setCardinal(state *st, cardinal car)
{
    st->car = car;
}

//斜め方角で壁情報どうするか.壁情報では斜め使わない.向きだけ
void setWallExistence(wall_existence *existence, wall_state *state)
{
    //単体で書き込めるモジュールは逆に面倒
    //自分の方角に合わせて書き込み先を変えるのは別のところで。
    existence->north = state[0];
    existence->east = state[1];
    existence->south = state[2];
    existence->west = state[3];
}
void initProfile(profile *prof)
{
    setGoal(prof, GOAL_X, GOAL_Y);

    setPosition(&(prof->now.pos), 0, 0);
    setCardinal(&(prof->now), north);

    setPosition(&(prof->next.pos), 0, 1);
    setCardinal(&(prof->next), north);

    //壁のセット
    wall_state w_st[4]={
        NOWALL,
        WALL,
        WALL,
        WALL
    };
    setWallExistence(&(prof->now.wall), &w_st[0]);
}
void shiftState(profile *prof)
{
    prof->now.car = prof->next.car;
    prof->now.pos.x = prof->next.pos.x;
    prof->now.pos.y = prof->next.pos.y;
}
void printState(state *st)
{
    printf("    座標    :   %u, %u\r\n", st->pos.x, st->pos.y);
    printf("    方角    :   %d\r\n", st->car);
    printf("    壁      :   %u, %u, %u, %u\r\n", st->wall.north, st->wall.east, st->wall.south, st->wall.west);
}
void printGoal(profile *prof)
{
    printf("左下 : (%u,%u), 右上 : (%u,%u)\r\n",prof->goal_lesser.x, prof->goal_lesser.y, prof->goal_larger.x, prof->goal_larger.y);
}
void printProfile(profile *prof)
{
    printGoal(prof);

    printf("現在\r\n");
    printState( &(prof->now) );

    printf("次\r\n");
    printState( &(prof->next) );

    // printf("目標\r\n");
    // printState( &(prof->target) );
    
}
/* アルゴリズムに関わる処理は別のファイルで*/