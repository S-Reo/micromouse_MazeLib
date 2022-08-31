//各種ライブラリを用いて、情報の更新を含めた探索アルゴリズムを記述
//壁の有無を更新. 後ろ以外を更新

// void writeNodeInfo(maze_node *maze, wall_state et)
// {
    
// }
// void updateWallExistence(maze_node *maze, wall_existence *wet, cardinal car, uint8_t x, uint8_t y)
// {
//     switch (car)
//     {
//     case north:
//         //南以外の壁更新
//         maze->RawNode[x][y].existence = wet->north;
//         maze->ColumnNode[x][y].existence = wet->east;
//         maze->ColumnNode[x][y].existence = wet->west;
//         break;
    
//     default:
//         break;
//     }
// }
// void writeExistence(uint8_t x, uint8_t y)
// {
//     //xyに合わせて書き込む参照先を工夫する
// }