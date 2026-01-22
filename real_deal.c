#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

volatile unsigned int mask_1it = 0x439fc898; // (- 0x12365498 == 0x31697400 == 1it << 4) for check_1
volatile unsigned int mask_root = 0x81a5c898; // 'oot' for check_1
volatile bool lenght = 1;    // inverted because why not (so 0 is good)
volatile bool main_branch = 0;   // for good pwds
volatile bool branch_2 = 0;  // for so-so pwds
volatile bool branch_3 = 0;  // for bruh pwds
volatile bool fake_branch = 0;

volatile char key = 0x50;
// "What is this bruh? What the pwd is this bruh?\n"
const char message1[] = {
    0x07, 0x38, 0x31, 0x24, 0x70, 0x39, 0x23, 0x70,
    0x24, 0x38, 0x39, 0x23, 0x70, 0x32, 0x22, 0x25,
    0x38, 0x6F, 0x70, 0x07, 0x38, 0x31, 0x24, 0x70,
    0x24, 0x38, 0x35, 0x70, 0x20, 0x27, 0x34, 0x70,
    0x39, 0x23, 0x70, 0x24, 0x38, 0x39, 0x23, 0x70,
    0x32, 0x22, 0x25, 0x38, 0x6F, 0x00
};
// "Finally! A worthy opponent!\n"
const char message2[] = {
    0x16, 0x39, 0x3E, 0x31, 0x3C, 0x3C, 0x29, 0x71,
    0x70, 0x11, 0x70, 0x27, 0x3F, 0x22, 0x24, 0x38,
    0x29, 0x70, 0x3F, 0x20, 0x20, 0x3F, 0x3E, 0x35,
    0x3E, 0x24, 0x71, 0x5A, 0x36, 0x3C, 0x31, 0x37,
    0x2B, 0x23, 0x38, 0x38, 0x0F, 0x23, 0x25, 0x20,
    0x35, 0x22, 0x0F, 0x23, 0x35, 0x33, 0x22, 0x35,
    0x24, 0x0F, 0x23, 0x35, 0x33, 0x22, 0x35, 0x24,
    0x2D, 0x00
};
// "Nice try but you still have much to learn\n"
const char message3[] = {
    0x1E, 0x39, 0x33, 0x35, 0x70, 0x24, 0x22, 0x29,
    0x70, 0x32, 0x25, 0x24, 0x70, 0x29, 0x3F, 0x25,
    0x70, 0x23, 0x24, 0x39, 0x3C, 0x3C, 0x70, 0x38,
    0x31, 0x26, 0x35, 0x70, 0x3D, 0x25, 0x33, 0x38,
    0x70, 0x24, 0x3F, 0x70, 0x3C, 0x35, 0x31, 0x22,
    0x3E, 0x00
};

void print_message(const char* enc, char key, int wat) {
    char buffer[256];
    int i;
    for (i = 0; enc[i] != 0; i++) {
        buffer[i] = enc[i] ^ key;
    }
    buffer[i] = '\0';
    printf("%s\n", buffer);
}


void lenght_check(const char* pwd) {
    char len = strlen(pwd);
    
    if (len >= 8) {
        lenght = 0;
        branch_3 = 1;
        // lenght == 32
        volatile unsigned int cookie = 1337, cookie2 = 890, cookie3 = 1635;
        if (len == (cookie ^ cookie2 ^ cookie3)) {
            lenght ^= len ^ cookie ^ cookie2 ^ cookie3;
            branch_3 = !branch_3;
            return;
        }
    }
}

void calc_start_branch(const char* pwd) {
    volatile char hehe = 0x25;
    volatile char haha = hehe ^ pwd[0];
    
    if (lenght == 1) {
        branch_3 = 1;
    }
    
    switch(haha) {
        // E (37 ^ 69 = 96)
        case 96:
            main_branch = 1;
            break;
        // r for root (37 ^ 114 = 87)
        case 87:
            branch_2 = 1;
            break;
        // just for meme (37 ^ 67 = 102 = f)
        case 67:
            main_branch = 1;
            branch_2 = 1;
            branch_3 = rand() % 2;
            break;
        default:
            branch_3 = 1;
            break;
    }
}

// after start 'E' or 'r' check for "E1it" or "root"
bool check_1(const char* pwd) {
    
    // pwd[1..3] == "1it" or "oot"
    unsigned int some = 0x12365498 + (pwd[1] << 24 | pwd[2] << 16 | pwd[3] << 8);
    
    if (main_branch & !branch_2 & !branch_3) {
        if (some == mask_1it) {
            // fake computations
            mask_1it = (some * main_branch) % 1337;
            branch_2 = 1;
            fake_branch = 1;
        }
        else if (some == mask_root) {
            mask_root = (some * main_branch) % 1337;
            branch_2 = 1;
        }
    }
    else if (!main_branch & branch_2) {
        switch(branch_3) {
            case 0:
                mask_1it = (some * branch_2) % 1337;
                if (some == mask_root) {
                    mask_root = (some * main_branch) % 1337;
                    branch_2 = 1;
                }
                fake_branch = 1;
                break;
            case 1:
                mask_1it = (some * branch_3) % 1337;
                if (some == mask_root) {
                    mask_root = (some * main_branch) % 1337;
                    branch_2 = 1;
                }
                fake_branch = 1;
                break;
        }
    }
    else {
        mask_1it = (some * mask_1it) % 1337;
        fake_branch = 1;
        return 0;
    }

    return 1;
}

// pwd[4, 16, 27] == '_';
// pwd[9, 13, 24] == '$';
// pwd[19, 21] == '_'^'$' == '{'
bool check_3(const char* pwd) {
    return 
        (pwd[15] == pwd[0] ^ pwd[5]) &&
        (pwd[0] - pwd[9] < 61) && // 95 - 36 < 61 (=), else there can be (' and #) or (a and ")
        (pwd[17] == pwd[12] ^ pwd[9]) &&
        (pwd[0] == pwd[12]) && 
        (pwd[12] == pwd[23]) &&
        (pwd[5] == pwd[9]) &&
        (pwd[9] == pwd[20]) &&
        (pwd[0] - pwd[9] > 0x3a) && // 95 - 36 > 58 (:)
        ((pwd[0] + pwd[9]) % 127 == 4); // 95 + 36 == 131 % 127 = 4
}

// check for flags and '_', '$', '{' in check_3 + handle fake_branch
bool check_2(const char* pwd) {
    volatile bool sf = !fake_branch;

    const char* xaxa = pwd + 4;
    if (check_3(xaxa) && !sf) {
        // let's do fake_branch flag needed active
        sf = (!lenght & main_branch & branch_2 & !branch_3 & fake_branch);
    }
    // could be more complicated
    
    return sf;
}

char final_check(const char* pwd) {
    lenght_check(pwd);
    calc_start_branch(pwd);
    
    bool check_res_1 = check_1(pwd);
    bool check_res_2 = check_2(pwd);

    volatile char tmp1 = 0x41;
    volatile int tmp2 = 1337;
    
    volatile char f1 = lenght | (main_branch << 1) | (branch_2 << 3) | (branch_3 << 2);
    volatile char f2 = (lenght << 2) | (main_branch << 3) | (branch_2 << 1) | branch_3;

    // branch_2, branch_3, main_branch, lenght == 1010 == 10
    if (f1 == (tmp1 - 0x37) && check_res_1 && check_res_2) {
        return 1;
    }
    // main_branch, lenght, branch_2, branch_3 == 0010 == 2
    else if (f2 == (tmp2 % 3) && check_res_1) {
        return 0;
    }

    return 2;
}

int main() {
    srand(time(NULL));
    printf("Enter the password if you dare: ");
    
    char pwd[256];
    if (fgets(pwd, sizeof(pwd), stdin) == NULL) {
        return 1;
    }
    pwd[strlen(pwd) - 1] = '\0';    // for handling \n
    
    char res = final_check(pwd);
    switch(res) {
        case 0:
            // main_branch, lenght, branch_2, branch_3 == 0010 == 2
            print_message(message3, key, res);
            break;
        case 1:
            // branch_2, branch_3, main_branch, lenght == 1010 == 10
            print_message(message2, key, res);
            break;
        // case 2:
        default:
            print_message(message1, key, res);
            break;
    }
    
    return 0;
}