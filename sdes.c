#include <stdio.h>
#include <stdlib.h>

// Initial Permutation (IP) Table
int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};

// Inverse Initial Permutation (IP^-1) Table
int IP_INV[] = {4, 1, 3, 5, 7, 2, 8, 6};

// Expansion Permutation (EP) Table
int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};

// Permutation Function (P4) Table
int swap[] = {5,6,7,8,1,2,3,4};

// S-Box Tables
int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};
int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

// Key Generation
void keyGeneration(int key[], int k1[], int k2[]) {
    int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
    int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};

    // Apply P10 permutation
    int temp1[10], temp2[10];
    for (int i = 0; i < 10; i++) {
        temp1[i] = key[P10[i] - 1];
    }

    // Left circular shift 1 bit
    for (int i = 0; i < 4; i++) {
        temp2[i] = temp1[(i + 1) % 5];
    }
    temp2[4] = temp1[0];
    for (int i = 5; i < 10; i++) {
        temp2[i] = temp1[((i + 1) % 5) + 5];
    }
    // Apply P8 permutation to get K1
    for (int i = 0; i < 8; i++) {
        k1[i] = temp2[P8[i] - 1];
    }
    // Left circular shift 2 bits
    for (int i = 0; i < 3; i++) {
        temp1[i] = temp2[(i + 2) % 5];
    }
    temp1[3] = temp2[0];
    temp1[4] = temp2[1];
    for (int i = 5; i < 10; i++) {
        temp1[i] = temp2[((i + 2) % 5) + 5];
    }
    // Apply P8 permutation to get K2
    for (int i = 0; i < 8; i++) {
        k2[i] = temp1[P8[i] - 1];
    }
}

// Initial Permutation (IP) Function
void initialPermutation(int plaintext[], int temp[]) {
    for (int i = 0; i < 8; i++) {
        temp[i] = plaintext[IP[i] - 1];
    }
}

// Inverse Initial Permutation (IP^-1) Function
void inverseInitialPermutation(int plaintext[], int temp[]) {
    for (int i = 0; i < 8; i++) {
        temp[i] = plaintext[IP_INV[i] - 1];
    }
}

// Expansion Permutation (EP) Function
void expansionPermutation(int text[], int temp[]) {
    for (int i = 0; i < 8; i++) {
        temp[i] = text[EP[i]-1];
    }
}

// XOR Operation
void xorOperation(int arr1[], int arr2[], int size) {
    for (int i = 0; i < size; i++) {
        arr1[i] = arr1[i] ^ arr2[i];
    }
}

// S-Box Substitution
void sboxSubstitution(int plaintext[],int text1[],int text2[], int temp[]) {
    int row, col, val, s0_0,s0_1,s1_0,s1_1;
    row = plaintext[0] * 2 + plaintext[3];
    col = plaintext[1] * 2 + plaintext[2];
    val = S0[row][col];
    s0_0 = val / 2;
    s0_1 = val % 2;

    row = plaintext[4] * 2 + plaintext[7];
    col = plaintext[5] * 2 + plaintext[6];
    val = S1[row][col];
    s1_0 = val / 2;
    s1_1 = val % 2;

    int t[4] = {s0_0,s0_1,s1_0,s1_1};

    for(int i=0;i<4;i++)
        temp[i]=t[i]^text1[i];
    for(int i=4;i<8;i++)
        temp[i]=text2[i-4];

}

// Permutation Function (P4) Function
void swapfunction(int plaintext[], int temp[]) {
    for (int i = 0; i < 8; i++) {
        temp[i] = plaintext[swap[i] - 1];
    }
}

void roundfunction(int text[],int key[],int temp[]){
     int text1[4],text2[4];
      for(int i=0;i<4;i++)
        text1[i]=text[i];
    for(int i=0 ;i<4;i++)
        text2[i]=text[i+4];
    expansionPermutation(text2, temp);
    xorOperation(temp, key, 8);
    sboxSubstitution(temp,text1,text2, temp);
}

// Encryption Function
void encryption(int plaintext[],int k1[],int k2[],int encrypt[]) {
    int temp[8],text1[4],text2[4];
    initialPermutation(plaintext, temp);
    // Round 1
    roundfunction(temp,k1,temp);
    // Swap
    swapfunction(temp,temp);
    // Round 2
    roundfunction(temp,k2,temp);
    inverseInitialPermutation(temp, encrypt);
    printf("Encrypted Text: ");
    for (int i = 0; i < 8; i++) {
        printf("%d", encrypt[i]);
    }
    printf("\n");
}

// Decryption Function
void decryption(int ciphertext[],int k1[],int k2[],int decrypt[]) {
    int temp[8],text1[8],text2[8];

    initialPermutation(ciphertext, temp);
    // Round 1
    roundfunction(temp,k2,temp);
    // Swap
    swapfunction(temp,temp);
    // Round 2
    roundfunction(temp,k1,temp);
    inverseInitialPermutation(temp, decrypt);
    printf("Decrypted Text: ");
    for (int i = 0; i < 8; i++) {
        printf("%d", decrypt[i]);
    }
    printf("\n");
}

void main() {
    int plaintext[8], key[10],encrypt[8],decrypt[8],k1[8],k2[8];
    printf("Enter 8-bit plaintext (0s and 1s only): ");
    for (int i = 0; i < 8; i++) {
        scanf("%1d", &plaintext[i]);
    }
    printf("Enter 10-bit key (0s and 1s only): ");
    for (int i = 0; i < 10; i++) {
        scanf("%1d", &key[i]);
    }
    keyGeneration(key, k1, k2);
    encryption(plaintext, k1,k2,encrypt);
    decryption(encrypt,k1,k2,decrypt);
}
