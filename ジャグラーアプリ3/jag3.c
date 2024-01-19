#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// リールのシンボル（絵柄）
char symbols[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'B', '*'};

// ランダムなシンボルを生成
char generateRandomSymbol() {
    int randomIndex = rand() % 578; // 0から577の範囲の乱数を生成
    if (randomIndex < 100) { // 約5.78分の1の確率で'*'を返す
        return '*';
    } else {
        return symbols[rand() % 10]; // それ以外の場合は他のシンボルをランダムに返す
    }
}

// ユーザがリールを回す関数
char userSpinReel() {
    char input;
    printf("リールを回すには何かキーを押してください...\n");
    scanf(" %c", &input);
    if (input == 'q') { // 'q'を入力するとゲームを終了
        return 'q';
    }
    return generateRandomSymbol();
}


// 最大公約数を求める関数
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}

int main() {
    int coins = 0;
    int spins = 0;
    int regularBonuses = 0;
    int bigBonuses = 0;
    int reelCount = 3; // リールの数
    int initialAmount;
    int reels[3];
    char addMoney;
    int addAmount;
    int j;
    int totalBonuses;
    int year, month, day;
    FILE *file;

    time_t timer;        /* 時刻を取り出すための型（実際はunsigned long型） */
    struct tm *local;    /* tm構造体（時刻を扱う */

    /* 年月日と時分秒保存用 */

    timer = time(NULL);        /* 現在時刻を取得 */
    local = localtime(&timer);    /* 地方時に変換 */

    /* 年月日と時分秒をtm構造体の各パラメタから変数に代入 */
    year = local->tm_year + 1900;        /* 1900年からの年数が取得されるため */
    month = local->tm_mon + 1;        /* 0を1月としているため */
    day = local->tm_mday;

    file = fopen("jaglog.txt", "a");
    if (file == NULL) {
        printf("ファイルを開けませんでした。\n");
        return 1;
    }

    // 乱数生成器のシードを設定
    srand(time(NULL));

    printf("いくらお金を投入しますか？ (1000円か10000円のみ): ");
    scanf("%d", &initialAmount);

    if (initialAmount == 1000) {
        coins = 50; // 1000円で50コイン提供
    } else if (initialAmount == 10000) {
        coins = 500; // 10000円で500コイン提供
    } else {
        printf("1000円か10000円のいずれかを入力してください。\n");
        return 1;
    }

    while (1) {
        printf("現在のコイン: %d\n", coins);

        if (coins < 3) {
            printf("コインが足りなくなりました。\n");
            while (1) {
                printf("お金を追加しますか？ (y/n): ");
                scanf(" %c", &addMoney);
                if (addMoney == 'y' || addMoney == 'Y') {
                    printf("いくら追加しますか？ (1000円か10000円のみ): ");
                    scanf("%d", &addAmount);

                    if (addAmount == 1000) {
                        coins += 50; // 1000円で50コイン追加
                        break;
                    } else if (addAmount == 10000) {
                        coins += 500; // 10000円で500コイン追加
                        break;
                    } else {
                        printf("1000円か10000円のいずれかを入力してください。\n");
                    }
                } else if (addMoney == 'n' || addMoney == 'N') {
                    printf("ゲームを終了します。\n");
                    /* 現在の日時を表示 */
                    fprintf(file,"%d年%d月%d日\n", year, month, day);

                    fprintf(file,"BIG %d\t", bigBonuses);
                    fprintf(file,"REG %d\t", regularBonuses);
                    fprintf(file,"総回転数 %d\n", spins);
                    fprintf(file,"合算: %d/%d\n", totalBonuses, spins);

                    // ファイルを閉じる
                    fclose(file);
                    return 0;
                } else {
                    printf("無効な入力です。再度入力してください。\n");
                }
            }
        }

        // 3コインを使ってリールを3つ回す
        coins -= 3;
        printf("3コインを使ってリールを回します。\n");

        // リールをユーザが手動で操作
        for (j = 0; j < 3; j++) {
            reels[j] = userSpinReel();
            if (reels[j] == 'q') {
                printf("ゲームを終了します。\n");
                /* 現在の日時を表示 */
                fprintf(file,"%d年%d月%d日\n", year, month, day);

                fprintf(file,"BIG %d\t", bigBonuses);
                fprintf(file,"REG %d\t", regularBonuses);
                fprintf(file,"総回転数 %d\n", spins);
                fprintf(file,"合算: %d/%d\n", totalBonuses, spins);

                // ファイルを閉じる
                fclose(file);
                return 0;
            }
            printf("リール%d: %c\n", j + 1, reels[j]);
        }

        spins++;

        // BIGボーナスの確率に応じて判定
        if (reels[0] == '7' && reels[1] == '7' && reels[2] == '7') {
            printf("BIGボーナス！\n");
            coins += 250; // BIGボーナスで250枚増加
            bigBonuses++;
        }
            // レギュラーボーナスの確率に応じて判定
        else if ((reels[0] == '7' && reels[1] == '7' && reels[2] == 'B') ||
                 (reels[0] == '7' && reels[1] == 'B' && reels[2] == '7') ||
                 (reels[0] == 'B' && reels[1] == '7' && reels[2] == '7')) {
            printf("揃いました！レギュラーボーナス！\n");
            coins += 100; // レギュラーボーナスで100枚増加
            regularBonuses++;
        }
            // ブドウボーナスの確率に応じて判定
        else if (reels[0] == '*' && reels[1] == '*' && reels[2] == '*') {
            printf("ブドウが揃いました！\n");
            coins += 8; // ブドウボーナスで8枚増加
        }
        else {
            printf("ハズレ！\n");
        }

        //BIG,REG,,総回転数を表示
        printf("BIG %d\t", bigBonuses);
        printf("REG %d\t", regularBonuses);
        printf("総回転数 %d\n", spins);

        // 合算を表示（ビッグボーナス合計+レギュラーボーナス合計 / 回転数）
        totalBonuses = bigBonuses + regularBonuses;
        if (totalBonuses == 0) {
            printf("合算: 0/%d\n", spins);
        } else {
            int divisor = gcd(totalBonuses, spins); // 最大公約数を求める
            printf("合算: %d/%d\n", totalBonuses / divisor, spins / divisor);
        }
    }

    return 0;
}