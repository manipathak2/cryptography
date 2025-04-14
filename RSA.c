#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------- Caesar Cipher ----------
void encrypt(char* text, int shift) {
    for (int i = 0; text[i] != '\0'; ++i) {
        char ch = text[i];
        if (ch >= 'A' && ch <= 'Z') text[i] = (ch + shift - 'A') % 26 + 'A';
        else if (ch >= 'a' && ch <= 'z') text[i] = (ch + shift - 'a') % 26 + 'a';
    }
}
void decrypt(char* text, int shift) {
    for (int i = 0; text[i] != '\0'; ++i) {
        char ch = text[i];
        if (ch >= 'A' && ch <= 'Z') text[i] = (ch - shift - 'A' + 26) % 26 + 'A';
        else if (ch >= 'a' && ch <= 'z') text[i] = (ch - shift - 'a' + 26) % 26 + 'a';
    }
}

// ---------- RSA ----------
void generateRSAKeys() {
    RSA *rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    FILE *priv = fopen("private.pem", "w");
    FILE *pub = fopen("public.pem", "w");
    PEM_write_RSAPrivateKey(priv, rsa, NULL, NULL, 0, NULL, NULL);
    PEM_write_RSA_PUBKEY(pub, rsa);
    fclose(priv); fclose(pub);
    RSA_free(rsa);
}
RSA* loadPublicKey(const char *file) {
    FILE *f = fopen(file, "r");
    if (!f) { perror("Public key load failed"); exit(EXIT_FAILURE); }
    RSA *rsa = PEM_read_RSA_PUBKEY(f, NULL, NULL, NULL);
    fclose(f);
    return rsa;
}
RSA* loadPrivateKey(const char *file) {
    FILE *f = fopen(file, "r");
    if (!f) { perror("Private key load failed"); exit(EXIT_FAILURE); }
    RSA *rsa = PEM_read_RSAPrivateKey(f, NULL, NULL, NULL);
    fclose(f);
    return rsa;
}
void rsaEncrypt(const char *input, unsigned char *encrypted, RSA *key) {
    int r = RSA_public_encrypt(strlen(input), (unsigned char*)input, encrypted, key, RSA_PKCS1_PADDING);
    if (r == -1) { fprintf(stderr, "RSA Encrypt error: %s\n", ERR_error_string(ERR_get_error(), NULL)); exit(EXIT_FAILURE); }
}
void rsaDecrypt(unsigned char *encrypted, unsigned char *decrypted, RSA *key) {
    int r = RSA_private_decrypt(256, encrypted, decrypted, key, RSA_PKCS1_PADDING);
    if (r == -1) { fprintf(stderr, "RSA Decrypt error: %s\n", ERR_error_string(ERR_get_error(), NULL)); exit(EXIT_FAILURE); }
    decrypted[r] = '\0';
}

// ---------- Main Program ----------
int main() {
    char answer = 'Y';
    char text[256];
    int shift, job, algo;

    printf("Welcome to the Integrated Encryption Tool!\n");

    // Check and generate RSA keys
    FILE *test = fopen("public.pem", "r");
    if (!test) {
        printf("RSA Keys not found. Generating...\n");
        generateRSAKeys();
    } else fclose(test);

    while (answer == 'Y' || answer == 'y') {
        printf("\nChoose what you want to perform:\n");
        printf("1. Message Encryption/Decryption\n");
        printf("2. File Encryption/Decryption (text only)\n");
        scanf("%d", &job);
        getchar();

        printf("\nChoose encryption method:\n1. Caesar Cipher\n2. RSA\n");
        scanf("%d", &algo);
        getchar();

        if (job == 1) {
            if (algo == 1) {
                printf("Enter message: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = '\0';

                printf("1. Encrypt\n2. Decrypt\n");
                scanf("%d", &job); getchar();
                printf("Enter key: "); scanf("%d", &shift);

                if (job == 1) { encrypt(text, shift); printf("Encrypted: %s\n", text); }
                else { decrypt(text, shift); printf("Decrypted: %s\n", text); }

            } else {
                printf("Enter message: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = '\0';

                unsigned char encrypted[256], decrypted[256];

                RSA *pub = loadPublicKey("public.pem");
                rsaEncrypt(text, encrypted, pub); RSA_free(pub);
                printf("Encrypted (base64-like): ");
                for (int i = 0; i < 256; ++i) printf("%02X", encrypted[i]);
                printf("\n");

                RSA *priv = loadPrivateKey("private.pem");
                rsaDecrypt(encrypted, decrypted, priv); RSA_free(priv);
                printf("Decrypted: %s\n", decrypted);
            }
        }

        else if (job == 2) {
            char inFile[100], outFile[100];
            printf("Enter input file name: "); scanf("%s", inFile);
            printf("Enter output file name: "); scanf("%s", outFile);

            printf("1. Encrypt\n2. Decrypt\n");
            scanf("%d", &job); getchar();

            if (algo == 1) {
                printf("Enter Caesar key: "); scanf("%d", &shift);

                FILE *in = fopen(inFile, "r");
                FILE *out = fopen(outFile, "w");
                if (!in || !out) { perror("File error"); exit(EXIT_FAILURE); }

                char ch;
                while ((ch = fgetc(in)) != EOF) {
                    if (job == 1) ch = (ch >= 'A' && ch <= 'Z') ? (ch + shift - 'A') % 26 + 'A' :
                                    (ch >= 'a' && ch <= 'z') ? (ch + shift - 'a') % 26 + 'a' : ch;
                    else ch = (ch >= 'A' && ch <= 'Z') ? (ch - shift - 'A' + 26) % 26 + 'A' :
                               (ch >= 'a' && ch <= 'z') ? (ch - shift - 'a' + 26) % 26 + 'a' : ch;
                    fputc(ch, out);
                }
                fclose(in); fclose(out);
                printf("File operation done!\n");
            } else {
                FILE *in = fopen(inFile, "r");
                FILE *out = fopen(outFile, "w");
                if (!in || !out) { perror("File error"); exit(EXIT_FAILURE); }

                char buffer[200];
                fgets(buffer, sizeof(buffer), in);
                buffer[strcspn(buffer, "\n")] = '\0';

                unsigned char encrypted[256], decrypted[256];

                if (job == 1) {
                    RSA *pub = loadPublicKey("public.pem");
                    rsaEncrypt(buffer, encrypted, pub); RSA_free(pub);
                    fwrite(encrypted, 1, 256, out);
                } else {
                    fread(encrypted, 1, 256, in);
                    RSA *priv = loadPrivateKey("private.pem");
                    rsaDecrypt(encrypted, decrypted, priv); RSA_free(priv);
                    fprintf(out, "%s", decrypted);
                }
                fclose(in); fclose(out);
                printf("RSA File operation complete.\n");
            }
        }

        printf("\nDo you want to perform another operation? (Y/N): ");
        scanf(" %c", &answer); getchar();
    }

    printf("\nThank you for using the encryption tool! Stay secure!\n");
    return 0;
}
