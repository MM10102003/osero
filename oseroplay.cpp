#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int player[10][10];

// 盤面表示
void show(int s, int t) {
    if (player[s][t] == 1) cout << " ● ";
    else if (player[s][t] == 2) cout << " ○ ";
    else if (player[s][t] == 0) cout << " = ";
}

// 相手の石を数える（プレイヤー用）
int count_rival1(int s, int t, int dir_s, int dir_t) {
    int count = 1;
    while (player[s + dir_s * count][t + dir_t * count] == 2) {
        count++;
    }
    if (player[s + dir_s * count][t + dir_t * count] == 1) return count - 1;
    return 0;
}

// 相手の石を数える（コンピュータ用）
int count_rival2(int s, int t, int dir_s, int dir_t) {
    int count = 1;
    while (player[s + dir_s * count][t + dir_t * count] == 1) {
        count++;
    }
    if (player[s + dir_s * count][t + dir_t * count] == 2) return count - 1;
    return 0;
}

// 置けるかチェック
bool can_place(int p, int s, int t) {
    if (player[s][t] != 0) return false;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            if (p == 1 && count_rival1(s, t, i, j) > 0) return true;
            if (p == 2 && count_rival2(s, t, i, j) > 0) return true;
        }
    }
    return false;
}

// 石をひっくり返す
void turn1(int s, int t, int dir_s, int dir_t) {
    int j = count_rival1(s, t, dir_s, dir_t);
    for (int i = 1; i <= j; i++) player[s + i * dir_s][t + i * dir_t] = 1;
}

void turn2(int s, int t, int dir_s, int dir_t) {
    int j = count_rival2(s, t, dir_s, dir_t);
    for (int i = 1; i <= j; i++) player[s + i * dir_s][t + i * dir_t] = 2;
}

int main(void) {
    srand(time(NULL));
    int s, t, i, j, u, v;

    cout << "\n=== GAME START ===" << endl;

    // 盤面初期化
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (i == 0 || i == 9 || j == 0 || j == 9) player[i][j] = -1;
            else player[i][j] = 0;
        }
    }
    //真ん中のマス目に置く
    player[4][4] = 1; player[5][5] = 1;
    player[4][5] = 2; player[5][4] = 2;

    for (i = 1; i < 9; i++) {
        cout << "\n" << endl;
        for (j = 1; j < 9; j++) {
            show(i, j);
        }
    }

    int zero_count = 1;//0(なにもおかれていないマス目)が1つもなくなったらゲーム終了
    while (zero_count != 0) {

        //プレイヤーの番
        bool your_can = false;
        for (i = 1; i <= 8; i++) {
            for (j = 1; j <= 8; j++) if (can_place(1, i, j)) { your_can = true; break; }
            if (your_can) break;
        }

        if (your_can) {
            cout << "\nYour Turn (●):" << endl;
            cout << "\nplease type in vertical\n" << endl;
            cin >> s;
            cout << "please type in beside\n" << endl;
            cin >> t;
            while (can_place(1, s, t) == false) {
                cout << "you can't put here..." << endl;
                cout << "please type in vertical" << endl;
                cin >> s;
                cout << "please type in beside" << endl;
                cin >> t;
            }

            for (i = -1; i < 2; i++) {
                for (j = -1; j < 2; j++) {
                    turn1(s, t, i, j);
                }
            }
            player[s][t] = 1;
            for (i = 1; i < 9; i++) {
                cout << "\n" << endl;
                for (j = 1; j < 9; j++) {
                    show(i, j);
                }
            }
        }
        else {
            cout << "\nYour Turn: PASS" << endl;
        }

        // --- コンピュータの番 ---
        bool comp_can = false;
        for (i = 1; i <= 8; i++) {
            for (j = 1; j <= 8; j++) if (can_place(2, i, j)) { comp_can = true; break; }
            if (comp_can) break;
        }

        if (comp_can) {
            while (true) {
                u = rand() % 8 + 1; v = rand() % 8 + 1;
                if (can_place(2, u, v)) {
                    player[u][v] = 2;
                    for (int di = -1; di <= 1; di++) {
                        for (int dj = -1; dj <= 1; dj++) turn2(u, v, di, dj);
                    }
                    break;
                }
            }
            cout << "\nComputer Turn (○):" << endl;
            for (i = 1; i <= 8; i++) {
                for (j = 1; j <= 8; j++) show(i, j);
                cout << endl;
            }
        }
        else {
            cout << "\nComputer Turn: PASS" << endl;
        }

        // 終了判定
        for (i = 1; i <= 8; i++) {
            for (j = 1; j <= 8; j++) if (player[i][j] == 0) zero_count++;
        }
        if (!your_can && !comp_can) break;
    }

    // 結果集計
    int p1 = 0, p2 = 0;
    for (i = 1; i <= 8; i++) {
        for (j = 1; j <= 8; j++) {
            if (player[i][j] == 1) p1++;
            if (player[i][j] == 2) p2++;
        }
    }
    //結果の表示
    cout << "\nResult: ● " << p1 << " - " << p2 << " ○" << endl;
    if (p1 > p2) cout << "WIN!" << endl;
    else if (p1 < p2) cout << "LOSE..." << endl;
    else cout << "DRAW" << endl;

    return 0;
}