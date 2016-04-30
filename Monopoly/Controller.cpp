//
//  Controller.cpp
//  Monopoly
//
//  Created by Dragonly on 4/27/16.
//  Copyright © 2016 Dragonly. All rights reserved.
//

#include "Controller.hpp"

using std::to_string;
using std::cout;
using std::endl;

namespace monopoly {
    Controller::Controller(GameContext& gc): gc(gc), gs(gc.gs) {
    }
    
    void Controller::nextTurn() {
        // vector 超出下标不会报错而是 undefined behavior = =, 异常坑
        // 翻转flag, 当所有人flag相同的时候, 表示过了一天
        gs.currentPlayer().done = true;
        gs.playerIndex = (gs.playerIndex + 1) % gs.players.size();
        nextDay();
    }
    
    void Controller::eval(char* cmd) {
        gs.error = false;
        switch (gs.state){
                // return makes a good one, break makes a bad one
            case GS::normal:
            {
                if (strcmp(cmd, "t") == 0) {
                    gs.message += "请选择工具编号:";
                    gs.state = GS::tool;
                }
                else if (strcmp(cmd, "s") == 0) {
                    gs.message += "请输入步数(-9 ~ 9), 负号表示反向于前进方向: (输入 " + RED + "x" + NC + " 返回)";
                    gs.state = GS::step;
                }
                else if (strcmp(cmd, "r") == 0) {
                    gs.lastRoll = static_cast<int>(rand() % 3) + 1;
                    gs.lastRoll = 3; // TODO: remove
                    gs.message += "玩家" + GREEN + gs.currentPlayer().name + NC + "刚刚掷出了 " + LCYAN + to_string(gs.lastRoll) + NC + "\n";
                    movePlayerWithAnimation(gs.lastRoll);
                    handleEvents();
                }
                else if (strcmp(cmd, "gg") == 0) {
                    popCurrentPlayer();
                }
                else if (strcmp(cmd, "init") == 0) {
                    gs.message += RED+"欢"+PURPLE+"迎"+BROWN+"来"+GREEN+"到"+BLUE+"大"+LRED+"富"+CYAN+"翁"+ NC;
                }
                else {
                    gs.errMsg = RED + "什么鬼 _(:з」∠)_" + NC;
                    break; // error
                }
                return;
            }
            case GS::tool:
            {
                Player& player = gs.currentPlayer();
                size_t count = player.tools.size();
                if (player.usingMagicDice) {
                    int step = atoi(cmd);
                    if (step > 0 && step < 7) {
                        player.usingMagicDice = false;
                        gs.message += "前进 " + to_string(step) + " 步";
                        gs.state = GS::normal;
                        movePlayerWithAnimation(step);
                        handleEvents();
                        return;
                    }
                    else {
                        gs.errMsg = RED + "无效的步数" + NC;
                        gs.message += "请输入前进步数 (1 ~ 6)";
                        break;
                    }
                }
                else if (player.usingRoadblock) {
                    int step = atoi(cmd);
                    if (step > -9 && step < 9) {
                        gs.message += "在位置 " + to_string(step) + " 处放置了一个路障";
                        int x = gs.road[player.curPos + step].pos.first;
                        int y = gs.road[player.curPos + step].pos.second;
                        gs.board[x][y].name = "roadblock";
                        gs.board[x][y].roadblock = true;
                        gs.state = GS::normal;
                        player.usingRoadblock = false;
                        return;
                    }
                    else {
                        gs.errMsg = RED + "无效的位置" + NC;
                        gs.message += "\n请输入放置路障的位置 (-8 ~ 8)";
                        break;
                    }
                }
                else if (strlen(cmd) == 1 && cmd[0] >= 48/*0*/ && cmd[0] < 48 + count) {
                    const Tool &tool = player.tools[atoi(cmd)];
                    gs.message = string("你使用了道具: ") + tool.name;
                    useTool(atoi(cmd));
                    return;
                }
                else if (strcmp(cmd, "x") == 0) {
                    gs.message += "取消操作";
                    gs.state = GS::normal;
                }
                else {
                    gs.errMsg = RED + "没有这个道具 :(" + NC;
                    break; // error
                }
            }
            case GS::toolStore:
            {
                Player& player = gs.currentPlayer();
                int n = atoi(cmd);
                if (n > -1 && n < 7) {
                    if (player.coupon == 0) {
                        gs.errMsg = "点券不足";
                        gs.state = GS::normal;
                        nextTurn();
                        break;
                    }
                    else {
                        player.addTool(static_cast<ToolType>(n));
                        player.coupon--;
                        gs.message += "成功购买道具: " + GREEN + player.toolMap[static_cast<ToolType>(n)] + NC;
                        gs.state = GS::normal;
                        nextTurn();
                        return;
                    }
                }
                else {
                    gs.errMsg = "没有这个道具";
                    gs.message += "请选择你想要购买的道具(花费"+BROWN+"1"+NC+"点券)\n";
                    gs.message += LBLUE + "0.路障, 1.遥控骰子, 2.均富卡, 3.转向卡, 4.滞留卡, 5.拆迁卡, 6.怪兽卡" + NC;
                    break; // error
                }
            }
            case GS::step:
            {
                if (strcmp(cmd, "x") == 0) {
                    gs.state = GS::normal;
                    return;
                }
                int s = atoi(cmd);
                if (s > -10 && s < 10){
                    int pos = (gs.currentPlayer().curPos + s) % gs.road.size();
                    gs.message += "玩家" + LBLUE;
                    if (s > 0) gs.message += "前";
                    else       gs.message += "后";
                    gs.message += NC + to_string(abs(s)) + "步是";
                    int x = gs.road[pos].pos.first;
                    int y = gs.road[pos].pos.second;
                    gs.message += gc.posSymbolMap[gs.board[x][y].name];
                    gs.state = GS::normal;
                    return;
                }
                else {
                    gs.errMsg = RED + "无效的步数" + NC;
                    break; // error
                }
            }
            case GS::buy:
            {
                if (strcmp(cmd, "y") == 0) {
                    Player &player = gs.currentPlayer();
                    int x = gs.road[player.curPos].pos.first;
                    int y = gs.road[player.curPos].pos.second;
                    Land &land = gs.board[x][y];
                    
                    // 只能在 land 上进行下列行为
                    if (land.landType == LandType::land) {
                        nextTurn();
                        gs.state = GS::normal;
                        if (land.owner == "none") { // 无主之地
                            if (player.cash < 200) {
                                gs.errMsg = "现金不足";
                                break;
                            }
                            else {
                                land.owner = player.name;
                                land.name = player.name + "Land" + to_string(land.level);
                                player.cash -= 200;
                                player.vPos.push_back(land.pos);
                            }
                        }
                        else if (land.owner == player.name) { // 升级房屋
                            if (player.cash < 200) {
                                gs.errMsg = "现金不足";
                                break;
                            }
                            else if (land.level == land.maxLevel) {
                                gs.errMsg = "房屋达到最高等级";
                                break;
                            }
                            else {
                                player.cash -= 200;
                                land.level += 1;
                            }
                        }
                    }
                    return;
                }
                else if (strcmp(cmd, "n") == 0) {
                    gs.message += "取消操作\n";
                    nextTurn();
                    gs.state = GS::normal;
                    return;
                }
                else {
                    gs.errMsg = RED + "什么鬼 _(:з」∠)_" + NC;
                    
                    Player &player = gs.currentPlayer();
                    int x = gs.road[player.curPos].pos.first;
                    int y = gs.road[player.curPos].pos.second;
                    Land &land = gs.board[x][y];
                    if (land.owner == "none") {
                        gs.message += "当前土地闲置, 是否花费 ￥200 购买?(y/n)";
                    }
                    else {
                        gs.message += "是否花费 ￥200 升级?(y/n)";
                    }
                    break;
                }
            }
            case GS::bank:
            {
                int save = atoi(cmd);
                Player& player = gs.currentPlayer();
                if (save > 0 && save > player.cash) {
                    gs.errMsg = "现金不足";
                }
                else if (save < 0 && -save > player.deposit) {
                    gs.errMsg = "存款不足";
                }
                else {
                    player.cash -= save;
                    player.deposit += save;
                    if (player.stepsLeft != 0) {
                        gs.state = GS::normal;
                        movePlayerWithAnimation(player.stepsLeft);
                        player.stepsLeft = 0;
                        handleEvents();
                    }
                    else {
                        gs.state = GS::normal;
                        nextTurn();
                        return;
                    }
                }
                break;
            }
        }
        gs.error = true;
    }
    
    void Controller::movePlayerWithAnimation(int delta) {
        for (int i = 0; i < delta; i++) {
            movePlayer(1);
            gc.drawGame();
            Player &player = gs.currentPlayer();
            int x = gs.road[player.curPos].pos.first;
            int y = gs.road[player.curPos].pos.second;
            if (gs.board[x][y].roadblock) {
                gs.board[x][y].roadblock = false;
                gs.board[x][y].name = player.name;
                return;
            }
            else if (gs.board[x][y].landType == LandType::bank) {
                player.stepsLeft = delta - i - 1;
                return;
            }
            if (i != delta - 1) usleep(80000);
        }
    }
    
    void Controller::movePlayer(int delta) {
        int curX, curY, preX, preY;
        Player &player = gs.currentPlayer();
        player.prePos = player.curPos;
        if (player.direction) {
            player.curPos = (player.curPos + delta) % gs.road.size();
        }
        else {
            player.curPos = (player.curPos - delta + static_cast<int>(gs.road.size())) % gs.road.size();
        }
        curX = gs.road[player.curPos].pos.first;
        curY = gs.road[player.curPos].pos.second;
        preX = gs.road[player.prePos].pos.first;
        preY = gs.road[player.prePos].pos.second;
        gs.board[curX][curY].name = player.name;
        player.x = curX;
        player.y = curY;
        displayLandCorrectly(preX, preY);
    }
    
    void Controller::popCurrentPlayer() {
        vector<Player>::iterator it = gs.players.begin();
        for (int i = 0; i < gs.playerIndex; i++) {
            it++;
        }
        // erase a player will make the next player the currentPlayer
        // if last player quit, we should make the 0 indexed player the currentPlayer
        if (gs.playerIndex == gs.players.size() - 1) {
            gs.playerIndex = 0;
        }
        gs.players.erase(it);
        if (gs.players.size() == 1) {
            gs.gameover = true;
        }
    }
    
    void Controller::displayLandCorrectly(int x, int y) {
        Land &land = gs.board[x][y];
        // restore init display name
        if (land.owner == "none") {
            land.name = gc.IlandMap[gs.board[x][y].landType];
        }
        else {
            land.name = land.owner + string("Land") + to_string(land.level);
        }
        for (int i = 0; i < gs.players.size(); i++) { // if player on land then display player
            if (gs.players[i].x == x && gs.players[i].y == y) {
                //            gs.board[x][y].name = gs.players[i].name;
                land.name = gs.players[i].name;
            }
        }
    }
    
    void Controller::displayPlayers() {
        int x, y;
        for (int i = 0; i < gs.players.size(); i++) {
            x = gs.players[i].x;
            y = gs.players[i].y;
            gs.board[x][y].name = gs.players[i].name;
        }
        // force current player on top
        x = gs.currentPlayer().x;
        y = gs.currentPlayer().y;
        gs.board[x][y].name = gs.currentPlayer().name;
    }
    
    void Controller::handleEvents() {
        Player &player = gs.currentPlayer();
        int x = gs.road[player.curPos].pos.first;
        int y = gs.road[player.curPos].pos.second;
        Land &land = gs.board[x][y];
        // TODO: 完善所有土地类型事件
        switch (land.landType) {
            case LandType::land:
            {
                if (land.owner == "none") { // 无主之地
                    gs.message += "\n当前土地闲置, 是否花费 ￥200 购买?(y/n)";
                    gs.state = GS::buy;
                }
                else if (land.owner == player.name) { // 升级房屋
                    gs.message += "房屋当前等级" + RED + to_string(land.level) + NC;
                    gs.message += "\n是否花费 ￥200 升级?(y/n)";
                    gs.state = GS::buy;
                }
                else {
                    // 罚款 = 0.3*房屋购买价格 + 0.1*同一条街道其余该玩家拥有街道购买价格之和
                    int penalty = land.basePrice * land.level * 0.2 + gs.streetPenalty(land.street, land.owner);
                    player.cash -= penalty;
                    gs.getPlayerByName(land.owner).cash += penalty;
                    if (player.cash < 0) { // 玩家现金不够, 用银行存款抵扣
                        player.deposit += player.cash;
                        player.cash = 0;
                    }
                    gs.message += RED + "玩家" + GREEN + player.name + RED + "交过路费 ￥" + to_string(penalty) + NC;
                    if (player.deposit < 0) {
                        gs.message =  RED + "玩家" + GREEN + player.name + RED + "破产\n" + NC;
                        popCurrentPlayer();
                    }
                    nextTurn();
                }
            } break;
            case LandType::gift:
            {
                int r = static_cast<int>(rand() % 7);
                player.addTool(static_cast<ToolType>(r));
                gs.message += "获得随机道具: " + GREEN + player.toolMap[static_cast<ToolType>(r)] + NC;
                nextTurn();
            } break;
            case LandType::bank:
            {
                gs.message += "请输入存款金额, 取款请输入负数";
                gs.state = GS::bank;
            } break;
            case LandType::news:
            {
                // TODO: test
                gs.message += "新闻事件:\n";
                int r0 = rand() % 5;
                vector<Player>::iterator it, itMax, itMin;
                switch (r0)
                {
                    // 地主称号按照土地数量计算
                    case 0:
                    {
                        for (itMax = it = gs.players.begin(); it != gs.players.end(); it++) {
                            if (it->vPos.size() > itMax->vPos.size()) {
                                itMax = it;
                            }
                        }
                        int r1 = ((rand() % 50) + 50) * static_cast<int>(itMax->vPos.size());
                        itMax->cash += r1;
                        gs.message += "公开表扬第一地主 " + GREEN + itMax->name + NC + ", 奖励" + BROWN + "￥" + to_string(r1) + NC;
                    } break;
                    case 1:
                    {
                        for (itMin = it = gs.players.begin(); it != gs.players.end(); it++) {
                            if (it->vPos.size() > itMin->vPos.size()) {
                                itMin = it;
                            }
                        }
                        int r2 = ((rand() % 50) + 50) * static_cast<int>(itMin->vPos.size());
                        itMin->cash += r2;
                        gs.message += "公开补助土地最少者 " + GREEN + itMin->name + NC + ", 补助" + BROWN + "￥" + to_string(r2) + NC;
                    } break;
                    case 2:
                    {
                        for (it = gs.players.begin(); it != gs.players.end(); it++) {
                            it->deposit *= 1.1;
                        }
                        gs.message += "银行加发储蓄红利, 每人存款增加" + RED + "10%" + NC;
                    } break;
                    case 3:
                    {
                        for (it = gs.players.begin(); it != gs.players.end(); it++) {
                            it->deposit *= 0.9;
                        }
                        gs.message += "所有人缴纳财产税" + RED + "10%" + NC;
                    } break;
                    case 4:
                    {
                        int r;
                        for (it = gs.players.begin(); it != gs.players.end(); it++) {
                            r = static_cast<int>(rand() % 7);
                            it->addTool(static_cast<ToolType>(r));
                            gs.message += GREEN + it->name + NC + "获得随机道具: " + BROWN + player.toolMap[static_cast<ToolType>(r)] + NC + "\n";
                        }
                    } break;
                }
                nextTurn();
            } break;
            case LandType::toolStore:
            {
                gs.message += "进入道具店\n";
                gs.message += "请选择你想要购买的道具(花费"+BROWN+"1"+NC+"点券)\n";
                gs.message += LBLUE + "0.路障, 1.遥控骰子, 2.均富卡, 3.转向卡, 4.滞留卡, 5.拆迁卡, 6.怪兽卡" + NC;
                gs.state = GS::toolStore;
            } break;
            case LandType::blank:
            {
                gs.message += "这里什么也没有";
                nextTurn();
            } break;
            case LandType::lottery:
            {
                gs.message += "彩票";
                nextTurn();
            } break;
            case LandType::coupon:
            {
                int coupon = ((rand() % 15) + 1) / 5; // 获得0~2点券
                player.coupon += coupon;
                gs.message += "获得点券" + YELLOW + to_string(coupon) + NC;
                nextTurn();
            } break;
            case LandType::VOID:
                break;
        }
    }
    
    void Controller::nextDay() {
        bool newDay = true;
        for (auto p : gs.players) {
            if (!p.done) {
                newDay = false;
                break;
            }
        }
        if (newDay) {
            for (auto& p : gs.players) {
                p.done = false;
            }
            gs.today.nextDay();
        }
        if (gs.today.d[2] == 1) { // 月初发钱
            vector<Player>::iterator it;
            for (it = gs.players.begin(); it != gs.players.end(); it++) {
                it->deposit *= 1.1;
            }
        }
    }
    
    void Controller::useTool(int i) {
        Player &player = gs.currentPlayer();
        const Tool &tool = player.tools[i];
        switch(tool.type) {
            case ToolType::MAGIC_DICE:
            {
                gs.message += "\n请输入前进步数 (1 ~ 6)";
                gs.currentPlayer().usingMagicDice = true;
            } break;
            case ToolType::ROADBLOCK:
            {
                gs.message += "\n请输入放置路障的位置 (-8 ~ 8)";
                gs.currentPlayer().usingRoadblock = true;
            } break;
            case ToolType::TURNING_CARD:
            {
                gs.message += "\n使用了转向卡";
                player.direction = !player.direction;
                gs.state = GS::normal;
            } break;
            case ToolType::AVERAGE_CARD:
            {
                int sum = 0;
                int average;
                vector<Player>::iterator it;
                for (it = gs.players.begin(); it != gs.players.end(); it++) {
                    sum += it->cash;
                }
                average = sum / gs.players.size();
                for (it = gs.players.begin(); it != gs.players.end(); it++) {
                    it->cash = average;
                }
                gs.message += "\n所有人的现金被平均分了";
                gs.state = GS::normal;
            } break;
            case ToolType::STAY_CARD:
            {
                gs.message += "\n原地停留一回合";
                gs.state = GS::normal;
                handleEvents();
            } break;
            case ToolType::REMOVE_CARD:
            {
                int x, y;
                int streetNum = gs.board[player.x][player.y].street;
                for (auto i : gs.streets[streetNum]) {
                    x = gs.road[i].pos.first;
                    y = gs.road[i].pos.second;
                    if (gs.board[x][y].owner != "none") {
                        // 返还 150% 当前房价
                        for (int j = 0; j < gs.players.size(); j++) {
                            if (gs.players[j].name == gs.board[x][y].owner) {
                                gs.players[j].cash += gs.board[x][y].level * gs.board[x][y].basePrice * 1.5;
                                gs.players[j].dropLand(i);
                                break;
                            }
                        }
                        gs.board[x][y].level = 1;
                    }
                }
                gs.state = GS::normal;
            } break;
            case ToolType::MONSTER_CARD:
            {
                int x, y;
                int streetNum = gs.board[player.x][player.y].street;
                for (auto i : gs.streets[streetNum]) {
                    x = gs.road[i].pos.first;
                    y = gs.road[i].pos.second;
                    if (gs.board[x][y].owner != "none") {
                        gs.board[x][y].name = gc.IlandMap[gs.board[x][y].landType];
                        gs.board[x][y].owner = "none";
                        gs.board[x][y].level = 1;
                    }
                }
                gs.state = GS::normal;
            } break;
        }
        player.dropTool(i);
    }
}