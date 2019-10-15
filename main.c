/**********************************************************************
 *
 * Filename:    main.c
 * Author:      Axel Wachter
 * Date:        15.10.2019
 * Description: a program to generate 16bit CRC lookup tables
 *
 * Notes:       - to use different CRC standards, use defines:
 *                  POLYNOMIAL
 *                  INITIAL_REMAINDER
 *                  FINAL_XOR_VALUE
 *              - the table is written into the file "crcTable.txt"
 *              - for a list with all the different CRC types and their
 *              parameters, check:
 *              http://reveng.sourceforge.net/crc-catalogue/16.htm
 * 
 **********************************************************************/

#include <stdio.h>
#include <string.h>

/*
 * Select the CRC standard from the list that follows.
 */
#define CRC_CCITT_XMODEM


#if defined(CRC_CCITT_XMODEM)
typedef unsigned short  crc;
#define CRC_NAME                "CRC-CCITT(XMODEM)"
#define POLYNOMIAL		0x1021
#define INITIAL_REMAINDER	0x0000
#define FINAL_XOR_VALUE		0x0000

#elif defined(CRC_CCITT_KERMIT)
typedef unsigned short  crc;
#define CRC_NAME		"CRC-CCITT(KERMIT)"
#define POLYNOMIAL		0x1021
#define INITIAL_REMAINDER	0x0000
#define FINAL_XOR_VALUE		0x0000
#endif



#define WIDTH    (8 * sizeof(crc))
#define TOPBIT   (1 << (WIDTH - 1))

crc  crcTable[256];

void main(void)
{
    FILE *fp;
    crc			   remainder;
    int			   dividend;
    unsigned char  bit;
        
    fp = fopen("crcTable.txt", "w+");
    fprintf(fp, "CRC-16 table\n");
    fprintf(fp, "polynomial: 0x%04x\n", POLYNOMIAL);
    fprintf(fp, "init:       0x%04x\n", INITIAL_REMAINDER);
    fprintf(fp, "xorout:     0x%04x\n\n", FINAL_XOR_VALUE);
    
    for (dividend = 0; dividend < 256; ++dividend)
    {

        if(!(dividend%8))
            fprintf(fp, "\n");        
        
        /*
         * Start with the dividend followed by zeros.
         */
        remainder = dividend << (WIDTH - 8);

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */			
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }

        /*
         * Store the result into the table.
         */
        crcTable[dividend] = remainder;
        fprintf(fp, "0x%04X, ", crcTable[dividend]);
    }

    printf("CRC-16 table generated - check the \"crcTable.txt\" file.\n");
    printf("polynomial: 0x%04x\n", POLYNOMIAL);
    printf("init:       0x%04x\n", INITIAL_REMAINDER);
    printf("xorout:     0x%04x\n", FINAL_XOR_VALUE);
    
    fclose(fp);
} 
