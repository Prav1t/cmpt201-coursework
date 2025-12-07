#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\e[9;31m"
#define GRN "\e[0;32m"
#define CRESET "\e[0m"

#define handle_error(msg)            
  do {                               
    perror(msg);                     
    exit(EXIT_FAILURE);              
  } while (0)

/* Helper: read entire file into buffer (binary). Exits on error. */
size_t read_all_bytes(const char *filename, void *buffer, size_t buffer_size) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    handle_error("Error opening file");
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    handle_error("fseek failed");
  }
  long file_size = ftell(file);
  if (file_size < 0) {
    handle_error("ftell failed");
  }
  if (fseek(file, 0, SEEK_SET) != 0) {
    handle_error("fseek failed");
  }

  if ((size_t)file_size > buffer_size) {
    handle_error("File size is too large");
  }

  if (fread(buffer, 1, file_size, file) != (size_t)file_size) {
    handle_error("Error reading file");
  }

  fclose(file);
  return (size_t)file_size;
}


void print_file(const char *filename, const char *color) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    handle_error("Error opening file");
  }

  printf("%s", color);
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    printf("%s", line);
  }
  fclose(file);
  printf(CRESET);
}


int verify(const char *message_path, const char *sign_path, EVP_PKEY *pubkey);

int main() {

  const char *message_files[] = {"message1.txt", "message2.txt", "message3.txt"};
  const char *signature_files[] = {"signature1.sig", "signature2.sig", "signature3.sig"};

 
  OpenSSL_add_all_algorithms();
  ERR_load_crypto_strings();

  //now to load the public key
  const char *key_candidates[] = {"pubkey.pem", "public.pem", "public_key.pem"};
  EVP_PKEY *pubkey = NULL;
  for (size_t i = 0; i < sizeof(key_candidates)/sizeof(key_candidates[0]); ++i) {
    FILE *kfile = fopen(key_candidates[i], "r");
    if (!kfile) continue;
    pubkey = PEM_read_PUBKEY(kfile, NULL, NULL, NULL);
    fclose(kfile);
    if (pubkey) {
      printf("Loaded public key from '%s'\n", key_candidates[i]);
      break;
    }
  }
  if (!pubkey) {
    fprintf(stderr, "Failed to load public key. Please provide pubkey.pem or public.pem or public_key.pem in the current directory.\n");
    ERR_print_errors_fp(stderr);
    /* Clean up and exit */
    EVP_cleanup();
    ERR_free_strings();
    return EXIT_FAILURE;
  }

  
  for (int i = 0; i < 3; i++) {
    printf("... Verifying message %d ...\n", i + 1);
    int result = verify(message_files[i], signature_files[i], pubkey);

    if (result < 0) {
      printf("Unknown authenticity of message %d\n", i + 1);
      print_file(message_files[i], CRESET);
    } else if (result == 0) {
      printf("Do not trust message %d!\n", i + 1);
      print_file(message_files[i], RED);
    } else {
      printf("Message %d is authentic!\n", i + 1);
      print_file(message_files[i], GRN);
    }
  }

  EVP_PKEY_free(pubkey);

  
  EVP_cleanup();
  ERR_free_strings();

  return 0;
}



int verify(const char *message_path, const char *sign_path, EVP_PKEY *pubkey) {
#define MAX_FILE_SIZE 8192
  unsigned char message[MAX_FILE_SIZE];
  unsigned char signature[MAX_FILE_SIZE];

 
  size_t message_len = read_all_bytes(message_path, message, sizeof(message));
  size_t signature_len = read_all_bytes(sign_path, signature, sizeof(signature));

 
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    fprintf(stderr, "EVP_MD_CTX_new failed\n");
    ERR_print_errors_fp(stderr);
    return -1;
  }

  
  if (EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, pubkey) <= 0) {
    fprintf(stderr, "EVP_DigestVerifyInit failed\n");
    ERR_print_errors_fp(stderr);
    EVP_MD_CTX_free(mdctx);
    return -1;
  }

 
  if (EVP_DigestVerifyUpdate(mdctx, message, message_len) <= 0) {
    fprintf(stderr, "EVP_DigestVerifyUpdate failed\n");
    ERR_print_errors_fp(stderr);
    EVP_MD_CTX_free(mdctx);
    return -1;
  }

 
  int verify_result = EVP_DigestVerifyFinal(mdctx, signature, signature_len);
  if (verify_result < 0) {
    
    fprintf(stderr, "EVP_DigestVerifyFinal error\n");
    ERR_print_errors_fp(stderr);
    EVP_MD_CTX_free(mdctx);
    return -1;
  }

  EVP_MD_CTX_free(mdctx);


  return verify_result;
}
