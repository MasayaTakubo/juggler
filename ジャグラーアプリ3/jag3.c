#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ���[���̃V���{���i�G���j
char symbols[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'B', '*'};

// �����_���ȃV���{���𐶐�
char generateRandomSymbol() {
    int randomIndex = rand() % 578; // 0����577�͈̗̔͂����𐶐�
    if (randomIndex < 100) { // ��5.78����1�̊m����'*'��Ԃ�
        return '*';
    } else {
        return symbols[rand() % 10]; // ����ȊO�̏ꍇ�͑��̃V���{���������_���ɕԂ�
    }
}

// ���[�U�����[�����񂷊֐�
char userSpinReel() {
    char input;
    printf("���[�����񂷂ɂ͉����L�[�������Ă�������...\n");
    scanf(" %c", &input);
    if (input == 'q') { // 'q'����͂���ƃQ�[�����I��
        return 'q';
    }
    return generateRandomSymbol();
}


// �ő���񐔂����߂�֐�
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
    int reelCount = 3; // ���[���̐�
    int initialAmount;
    int reels[3];
    char addMoney;
    int addAmount;
    int j;
    int totalBonuses;
    int year, month, day;
    FILE *file;

    time_t timer;        /* ���������o�����߂̌^�i���ۂ�unsigned long�^�j */
    struct tm *local;    /* tm�\���́i���������� */

    /* �N�����Ǝ����b�ۑ��p */

    timer = time(NULL);        /* ���ݎ������擾 */
    local = localtime(&timer);    /* �n�����ɕϊ� */

    /* �N�����Ǝ����b��tm�\���̂̊e�p�����^����ϐ��ɑ�� */
    year = local->tm_year + 1900;        /* 1900�N����̔N�����擾����邽�� */
    month = local->tm_mon + 1;        /* 0��1���Ƃ��Ă��邽�� */
    day = local->tm_mday;

    file = fopen("jaglog.txt", "a");
    if (file == NULL) {
        printf("�t�@�C�����J���܂���ł����B\n");
        return 1;
    }

    // ����������̃V�[�h��ݒ�
    srand(time(NULL));

    printf("�����炨���𓊓����܂����H (1000�~��10000�~�̂�): ");
    scanf("%d", &initialAmount);

    if (initialAmount == 1000) {
        coins = 50; // 1000�~��50�R�C����
    } else if (initialAmount == 10000) {
        coins = 500; // 10000�~��500�R�C����
    } else {
        printf("1000�~��10000�~�̂����ꂩ����͂��Ă��������B\n");
        return 1;
    }

    while (1) {
        printf("���݂̃R�C��: %d\n", coins);

        if (coins < 3) {
            printf("�R�C��������Ȃ��Ȃ�܂����B\n");
            while (1) {
                printf("������ǉ����܂����H (y/n): ");
                scanf(" %c", &addMoney);
                if (addMoney == 'y' || addMoney == 'Y') {
                    printf("������ǉ����܂����H (1000�~��10000�~�̂�): ");
                    scanf("%d", &addAmount);

                    if (addAmount == 1000) {
                        coins += 50; // 1000�~��50�R�C���ǉ�
                        break;
                    } else if (addAmount == 10000) {
                        coins += 500; // 10000�~��500�R�C���ǉ�
                        break;
                    } else {
                        printf("1000�~��10000�~�̂����ꂩ����͂��Ă��������B\n");
                    }
                } else if (addMoney == 'n' || addMoney == 'N') {
                    printf("�Q�[�����I�����܂��B\n");
                    /* ���݂̓�����\�� */
                    fprintf(file,"%d�N%d��%d��\n", year, month, day);

                    fprintf(file,"BIG %d\t", bigBonuses);
                    fprintf(file,"REG %d\t", regularBonuses);
                    fprintf(file,"����]�� %d\n", spins);
                    fprintf(file,"���Z: %d/%d\n", totalBonuses, spins);

                    // �t�@�C�������
                    fclose(file);
                    return 0;
                } else {
                    printf("�����ȓ��͂ł��B�ēx���͂��Ă��������B\n");
                }
            }
        }

        // 3�R�C�����g���ă��[����3��
        coins -= 3;
        printf("3�R�C�����g���ă��[�����񂵂܂��B\n");

        // ���[�������[�U���蓮�ő���
        for (j = 0; j < 3; j++) {
            reels[j] = userSpinReel();
            if (reels[j] == 'q') {
                printf("�Q�[�����I�����܂��B\n");
                /* ���݂̓�����\�� */
                fprintf(file,"%d�N%d��%d��\n", year, month, day);

                fprintf(file,"BIG %d\t", bigBonuses);
                fprintf(file,"REG %d\t", regularBonuses);
                fprintf(file,"����]�� %d\n", spins);
                fprintf(file,"���Z: %d/%d\n", totalBonuses, spins);

                // �t�@�C�������
                fclose(file);
                return 0;
            }
            printf("���[��%d: %c\n", j + 1, reels[j]);
        }

        spins++;

        // BIG�{�[�i�X�̊m���ɉ����Ĕ���
        if (reels[0] == '7' && reels[1] == '7' && reels[2] == '7') {
            printf("BIG�{�[�i�X�I\n");
            coins += 250; // BIG�{�[�i�X��250������
            bigBonuses++;
        }
            // ���M�����[�{�[�i�X�̊m���ɉ����Ĕ���
        else if ((reels[0] == '7' && reels[1] == '7' && reels[2] == 'B') ||
                 (reels[0] == '7' && reels[1] == 'B' && reels[2] == '7') ||
                 (reels[0] == 'B' && reels[1] == '7' && reels[2] == '7')) {
            printf("�����܂����I���M�����[�{�[�i�X�I\n");
            coins += 100; // ���M�����[�{�[�i�X��100������
            regularBonuses++;
        }
            // �u�h�E�{�[�i�X�̊m���ɉ����Ĕ���
        else if (reels[0] == '*' && reels[1] == '*' && reels[2] == '*') {
            printf("�u�h�E�������܂����I\n");
            coins += 8; // �u�h�E�{�[�i�X��8������
        }
        else {
            printf("�n�Y���I\n");
        }

        //BIG,REG,,����]����\��
        printf("BIG %d\t", bigBonuses);
        printf("REG %d\t", regularBonuses);
        printf("����]�� %d\n", spins);

        // ���Z��\���i�r�b�O�{�[�i�X���v+���M�����[�{�[�i�X���v / ��]���j
        totalBonuses = bigBonuses + regularBonuses;
        if (totalBonuses == 0) {
            printf("���Z: 0/%d\n", spins);
        } else {
            int divisor = gcd(totalBonuses, spins); // �ő���񐔂����߂�
            printf("���Z: %d/%d\n", totalBonuses / divisor, spins / divisor);
        }
    }

    return 0;
}