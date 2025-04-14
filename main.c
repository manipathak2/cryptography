#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//function to encrypt a string
void encrypt(char* text, int shift) {
    for (int i = 0; text[i] != '\0'; ++i) {
        char ch = text[i];

        // Encrypt uppercase letters
        if (ch >= 'A' && ch <= 'Z') {
            text[i] = (ch + shift - 'A') % 26 + 'A';
        }
        // Encrypt lowercase letters
        else if (ch >= 'a' && ch <= 'z') {
            text[i] = (ch + shift - 'a') % 26 + 'a';
        }
    }
}
//function to decrypt a string
void decrypt(char* text, int shift) {
    for (int i = 0; text[i] != '\0'; ++i) {
        char ch = text[i];

        // Decrypt uppercase letters
        if (ch >= 'A' && ch <= 'Z') {
            text[i] = (ch - shift - 'A' + 26) % 26 + 'A';
        }
        // Decrypt lowercase letters
        else if (ch >= 'a' && ch <= 'z') {
            text[i] = (ch - shift - 'a' + 26) % 26 + 'a';
        }
    }
}


// Function to encrypt a character
char encryptChar(char ch, int shift) {
    if (ch >= 'A' && ch <= 'Z') {
        return (ch + shift - 'A') % 26 + 'A';
    } else if (ch >= 'a' && ch <= 'z') {
        return (ch + shift - 'a') % 26 + 'a';
    } else {
        return ch; // Non-alphabetic characters are unchanged
    }
}
char decryptChar(char ch, int shift){
        if (ch >= 'A' && ch <= 'Z') {
            return (ch - shift - 'A' + 26) % 26 + 'A';
        }
        // Decrypt lowercase letters
        else if (ch >= 'a' && ch <= 'z') {
            return (ch - shift - 'a' + 26) % 26 + 'a';
        }
        else {
            return ch; // Non-alphabetic characters are unchanged
        }
}

// Function to encrypt the contents of a file
void encryptFile(const char* inputFile, const char* outputFile, int shift) {
        FILE *inFile = fopen(inputFile, "r");
        if (inFile == NULL) {
            // If file doesn't exist, create it and write a user message
            inFile = fopen(inputFile, "w");
            if (inFile == NULL) {
                perror("Error creating input file");
                exit(EXIT_FAILURE);
            }
        char userMessage[256];
        printf("The file does not exist. Enter a message to write into the new file: ");
        getchar();
            // Consume newline character left by previous input
        fgets(userMessage, sizeof(userMessage),stdin); // Remove newline character from the input string
        userMessage[strcspn(userMessage, "\n")] = '\0';
        fprintf(inFile, "%s", userMessage);
        fclose(inFile); // Re-open in read mode
        inFile = fopen(inputFile, "r");
        }

        FILE *outFile = fopen(outputFile, "w");
        if (outFile == NULL){
            perror("Error opening output file");
            fclose(inFile);
            exit(EXIT_FAILURE);
        }
        char ch;
        while ((ch = fgetc(inFile)) != EOF) {
            char encryptedChar = encryptChar(ch, shift);
            fputc(encryptedChar, outFile);
        }
        fclose(inFile);
        fclose(outFile);
}

void decryptFile(const char* inputFile, const char* outputFile, int shift){
    FILE *inFile = fopen(inputFile, "r");
    if (inFile == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    FILE *outFile = fopen(outputFile, "w");
    if (outFile == NULL) {
            perror("Error opening output file");
            fclose(inFile); exit(EXIT_FAILURE);
            }
    char ch;
    while ((ch = fgetc(inFile)) != EOF) {
            char decryptedChar = decryptChar(ch, shift);
            fputc(decryptedChar, outFile);
            }
            fclose(inFile);
            fclose(outFile);
}

int main() {
    char inputFile[100];
    char outputFile[100];
    int shift,task;
    char text[100];
    char answer;
    printf("-------Welcome to the Encryption & Decryption Tool!-------\n");
    printf("\nReady to test your cryptographic skills?");
    printf("\nEnter Y to begin or N to exit: ");
    scanf("%c",&answer);
    getchar();
                // To consume the newline character left by scanf
     while(answer=='y'||answer=='Y'){
        printf("Choose what do you want to perform: \n");
        printf("1. File Encryption & Decryption\n");
        printf("2. Message Encryption & Decryption\n");
        int job;
        scanf("%d", &job);
        getchar();
        switch(job){
                //File cryptography technique
    case 1:
        printf("Enter the name of the input file: ");
        scanf("%s", inputFile);

        printf("Enter the name of the output file: ");
        scanf("%s", outputFile);

        printf("Choose the following options :\n");
        printf("1. File Encryption\n");
        printf("2. File Decryption\n");
        int fjob;
        scanf("%d",&fjob);
        getchar();
        switch(fjob)
        {
        case 1:
            printf("Enter the Encryption key: ");
            scanf("%d", &shift);
            encryptFile(inputFile, outputFile, shift);
            printf("File encryption complete. Encrypted data written to %s\n", outputFile);
            break;

        case 2:
            printf("Enter the Decryption key: ");
            scanf("%d", &shift);
            decryptFile(inputFile, outputFile, shift);
            printf("File Decryption complete. Decrypted data written to %s\n", outputFile);
            break;

        default:
            printf("Invalid Input\n");
            break;
        }
        printf("\nWant to Encrypt or decrypt more file? Enter Y or N: ");
        scanf(" %c", &answer);
        getchar();
    break;
                // message cryptography technique
    case 2:
        printf("Enter a message : ");
        fgets(text, sizeof(text), stdin);
                // Remove newline character from the input string
        text[strcspn(text, "\n")] = '\0';
        printf("Choose the following options :\n");
        printf("1. Encryption\n");
        printf("2. Decryption\n");
        scanf("%d", &task);

        switch(task)
            {
                case 1:
                    printf("Enter Encryption Key: ");
                    scanf("%d", &shift);
                    encrypt(text, shift);
                    printf("Encrypted message: %s\n", text);
                    printf("Successfully encrypted !!\n");
                    break;

                case 2:
                    printf("Enter Decryption Key: ");
                    scanf("%d", &shift);
                    decrypt(text, shift);
                    printf("Decrypted message: %s\n", text);
                    printf("Successfully Decrypted !!\n");
                    break;

                default :
                    printf("Invalid Input!!");
                    break;
                    }
                    printf("\nWant to Encrypt and decrypt more? Enter Y or N: ");
                    scanf(" %c", &answer);
                    getchar();
        break;
    default:
        printf("Invalid Input!!");
        break;
        }
        printf("\nDo you want to perform another task? Enter Y or N: ");
        scanf(" %c", &answer);
        getchar();
    }
    printf("\n-----Thank you for using the encryption/decryption tool!-----\n");
    printf("\n****Remember: In the digital world, your security is paramount.****\n");
    printf("                      Stay safe and encrypted!\n");
    printf("              Goodbye, and have a fantastic day ahead!\n");
return 0;
}
