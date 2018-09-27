---
title: openssl evp
date: 2018-09-27 21:54:43
tags: openssl, evp, aes
---

最近使用了openssl的AES解密函数，遇到些问题。

## 代码示例
以下代码结构摘录自[man page](https://linux.die.net/man/3/evp_decryptfinal)
```c
int do_crypt(FILE *in, FILE *out, int do_encrypt)
{
    /* Allow enough space in output buffer for additional block */
    inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int inlen, outlen;
    
    /* Bogus key and IV: we'd normally set these from
    * another source.
    */
    unsigned char key[] = "0123456789";
    unsigned char iv[] = "12345678";
    /* Don't set key or IV because we will modify the parameters */
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CipherInit_ex(&ctx, EVP_rc2(), NULL, NULL, NULL, do_encrypt);
    EVP_CIPHER_CTX_set_key_length(&ctx, 10);
    /* We finished modifying parameters so now we can set key and IV */
    EVP_CipherInit_ex(&ctx, NULL, NULL, key, iv, do_encrypt);

    for(;;)
    {
        inlen = fread(inbuf, 1, 1024, in);
        if(inlen <= 0) break;
        if(!EVP_CipherUpdate(&ctx, outbuf, &outlen, inbuf, inlen))
        {
            /* Error */
            EVP_CIPHER_CTX_cleanup(&ctx);
            return 0;
        }

        fwrite(outbuf, 1, outlen, out);
    }
       
    if(!EVP_CipherFinal_ex(&ctx, outbuf, &outlen))
    {
        /* Error */
        EVP_CIPHER_CTX_cleanup(&ctx);
        return 0;
    }
    
    fwrite(outbuf, 1, outlen, out);

    EVP_CIPHER_CTX_cleanup(&ctx);
    return 1;
}
```

## 问题记录
参考[Openssl Evp接口以及EVP_DecryptFinal使用细节](https://blog.csdn.net/weixiao2015/article/details/79487775)
1. `aes_128_ecb` `aes_256_ecb`中 128 和 256 指 key的长度，128对应16字节、256对应32字节
2. 分块加密，即不断调用Update。加密以16字节为一块，进行加密。如果末尾不足16字节，需要填充。
3. `EVP_CIPHER_CTX_set_padding` 指定填充模式
    ```c
    #define EVP_PADDING_PKCS7       1
    #define EVP_PADDING_ISO7816_4   2
    #define EVP_PADDING_ANSI923     3
    #define EVP_PADDING_ISO10126    4
    #define EVP_PADDING_ZERO        5
    ```
4. 加密时
   1. 如果padding开启, 最后update会留下16字节待处理，Final函数处理剩余字节
      * 明文不是16的整数倍，会填充至16字节
      * 明文是16的整数倍，保留最后16字节
     
   2. 如果padding没有开启padding
      * 明文是16的整数倍，update字节处理完，final不处理
      * 不是16的整数倍，final函数返回错误

5. `EVP_DecryptFinal` **Bad Decrypt**问题
   1. 没有设置padding，密文长度不是16的整数倍
   2. 加解密设置的padding不对应

## 其他
1. AES有几种扩展算法，其中ecb和cbc需要填充，即加密后长度可能会不一样，cfb和ofb不需要填充，密文长度与明文长度一样 
2. 针对**Bad Decrypt**问题，本人实际解密过程中，对16整数倍字节的数据未调用final函数，长度直接返回密文长度。


