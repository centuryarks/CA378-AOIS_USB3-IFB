// RAW2RGB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#define RAW10_MAX   (1023)          // RAW10 max value

BITMAPFILEHEADER bitmapFileHeader;  // bitmap file header
BITMAPINFOHEADER bitmapInfoHeader;  // bitmap info header
unsigned char *RAW_Image = NULL;    // RAW image data
unsigned short*RAW10_Image = NULL;  // RAW 10bit image data
unsigned char *RGB_Image = NULL;    // RGB image data
int RAW_width;                      // RAW width
int RAW_height;                     // RAW height
int RGB_width;                      // RGB width
int RGB_height;                     // RGB height

unsigned char g_gammaTable[1024];   // 10-bit resolution
double g_gamma = 0.45;              // gamma value
int g_offset = 16;                  // offset value
int g_rgbGain[3];                   // gain value
int g_interpolation = 1;            // bayer interpolation
int g_flip = 0;                     // flip images vertically
char g_inputFile[_MAX_FNAME];       // input filename
char g_outputFile[_MAX_FNAME];      // output filename


// Update gamma table
static void UpdateGammaTable(BYTE* pTable, int tableSize, double gamma)
{
    int i, v;
    for (i = 0; i < tableSize; i++)
    {
        v = (int)(pow(i / (double)tableSize, gamma) * 255 + 0.5);
        if (v > 255) v = 255;
        pTable[i] = v;
    }
}

// Load bitmap
int Load_bitmap(char *filename)
{
    errno_t err;        // error no
    FILE *fp = NULL;    // file pointer
    size_t size = 0;    // read size

    // open filename in read binary mode
    err = fopen_s(&fp, filename,"rb");
    if (err)
    {
        perror("File open error!");
        return 1;
    }

    // read the bitmap file header
    size = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    if (size == 0)
    {
        perror("Read bitmapt file header error!");
        fclose(fp);
        return 1;
    }

    // verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType != 0x4D42)
    {
        perror("File not bitmap!");
        fclose(fp);
        return 1;
    }

    // read the bitmap info header
    size = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp); 
    if (size == 0)
    {
        perror("Read bitmapt info header error!");
        fclose(fp);
        return 1;
    }

    // move file point to the begging of bitmap data
    fseek(fp, bitmapFileHeader.bfOffBits, SEEK_SET);

    // calculate image size
    bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth
                                 * bitmapInfoHeader.biHeight
                                 * bitmapInfoHeader.biBitCount / 8;

    // allocate memory for RAW image data
    RAW_Image = (unsigned char*)malloc(bitmapInfoHeader.biSizeImage);
    if (!RAW_Image)
    {
        perror("RAW image allocate error");
        fclose(fp);
        return 1;
    }

    // read in the bitmap image data
    if (g_flip)
    {
        // read in vertical reversal every line
        for (int height = 0; height < bitmapInfoHeader.biHeight; height++)
        {
            // Read size per line
            int lineBytes = bitmapInfoHeader.biWidth * bitmapInfoHeader.biBitCount / 8;
            size = fread(&RAW_Image[(bitmapInfoHeader.biHeight - height - 1) * lineBytes], lineBytes, 1, fp);
            if (size == 0)
            {
                perror("Read bitmapt image error!");
                fclose(fp);
                return 1;
            }
        }
    }
    else
    {
        size = fread(RAW_Image, bitmapInfoHeader.biSizeImage, 1, fp);
        if (size == 0)
        {
            perror("Read bitmapt image error!");
            fclose(fp);
            return 1;
        }
    }

    // close file
    fclose(fp);

    return 0;
}

// Save bitmap
int Save_bitmap(char *filename)
{
    errno_t err;        // error no
    FILE *fp = NULL;    // file pointer
    size_t size = 0;    // write size

    // open filename in write binary mode
    err = fopen_s(&fp, filename, "wb");
    if (err)
    {
        perror("File open error!");
        return 1;
    }

    // write the bitmap file header
    size = fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    if (size == 0)
    {
        perror("Write bitmapt file header error!");
        fclose(fp);
        return 1;
    }

    // write the bitmap info header
    size = fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp); 
    if (size == 0)
    {
        perror("Write bitmapt info header error!");
        fclose(fp);
        return 1;
    }

    // write in the bitmap image data
    size = fwrite(RGB_Image, bitmapInfoHeader.biSizeImage, 1, fp);
    if (size == 0)
    {
        perror("Write bitmapt image error!");
        fclose(fp);
        return 1;
    }

    // close file
    fclose(fp);

    return 0;
}

// Unpack RAW10 image
void Unpack_RAW10()
{
    int w, h;
    int P0, P1, P2, P3, P4;
    int index;

    int DATA_LINE = RAW_height - RGB_height;

    for (h = 0; h < RGB_height; h++)
    {
        for (w = 0; w < RGB_width; w += 4)
        {
            // 10bit RAW format
            //          bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
            // Byte 0:  P0[9]   P0[8]   P0[7]   P0[6]   P0[5]   P0[4]   P0[3]   P0[2]
            // Byte 1:  P1[9]   P1[8]   P1[7]   P1[6]   P1[5]   P1[4]   P1[3]   P1[2]
            // Byte 2:  P2[9]   P2[8]   P2[7]   P2[6]   P2[5]   P2[4]   P2[3]   P2[2]
            // Byte 3:  P3[9]   P3[8]   P3[7]   P3[6]   P3[5]   P3[4]   P3[3]   P3[2]
            // Byte 4:  P3[1]   P3[0]   P2[1]   P2[0]   P1[1]   P1[0]   P0[1]   P0[0]
            index = DATA_LINE * RAW_width * 3 + w * 5 / 4;
            P4 =  RAW_Image[h * RAW_width * 3 + (index + 4)];
            P3 = (RAW_Image[h * RAW_width * 3 + (index + 3)] << 2) + ((P4 >> 6) & 0x3);
            P2 = (RAW_Image[h * RAW_width * 3 + (index + 2)] << 2) + ((P4 >> 4) & 0x3);
            P1 = (RAW_Image[h * RAW_width * 3 + (index + 1)] << 2) + ((P4 >> 2) & 0x3);
            P0 = (RAW_Image[h * RAW_width * 3 + (index + 0)] << 2) + ((P4 >> 0) & 0x3);
            RAW10_Image[h * RGB_width + (w + 0)] = P0;
            RAW10_Image[h * RGB_width + (w + 1)] = P1;
            RAW10_Image[h * RGB_width + (w + 2)] = P2;
            RAW10_Image[h * RGB_width + (w + 3)] = P3;
        }
    }
}

// Nearest Neighbor method Processing
static void NearestNeighborProcessing(int w, int h, int &R, int &G, int &B)
{
    // Copy the near pixel at the edge of the image
    if (w == RGB_width  - 1) w -= 1;
    if (h == RGB_height - 1) h -= 1;

    switch (((h & 0x1) << 1) + (w & 0x1))
    {
    case 0:     // GB center
        G = RAW10_Image[(h + 0) * RGB_width + (w + 0)];
        B = RAW10_Image[(h + 0) * RGB_width + (w + 1)];
        R = RAW10_Image[(h + 1) * RGB_width + (w + 0)];
        break;
    case 1:     // B center
        B = RAW10_Image[(h + 0) * RGB_width + (w + 0)];
        G = RAW10_Image[(h + 0) * RGB_width + (w + 1)];
        R = RAW10_Image[(h + 1) * RGB_width + (w + 1)];
        break;
    case 2:     // R center
        R = RAW10_Image[(h + 0) * RGB_width + (w + 0)];
        G = RAW10_Image[(h + 0) * RGB_width + (w + 1)];
        B = RAW10_Image[(h + 1) * RGB_width + (w + 1)];
        break;
    case 3:     // GR center
        G = RAW10_Image[(h + 0) * RGB_width + (w + 0)];
        R = RAW10_Image[(h + 0) * RGB_width + (w + 1)];
        B = RAW10_Image[(h + 1) * RGB_width + (w + 0)];
        break;
    default:
        break;
    }
}

// Bilinear interpolation Processing
static void BilinearProcessing(int w, int h, int &R, int &G, int &B)
{
    // Copy the near pixel at the edge of the image
    if (w == 0) w += 1;
    if (h == 0) h += 1;
    if (w == RGB_width  - 1) w -= 1;
    if (h == RGB_height - 1) h -= 1;

    switch (((h & 0x1) << 1) + (w & 0x1))
    {
    case 0:     // GB center
        G =  RAW10_Image[(h + 0) * RGB_width + (w + 0)];
        B = (RAW10_Image[(h + 0) * RGB_width + (w - 1)]
          +  RAW10_Image[(h + 0) * RGB_width + (w + 1)]) >> 1;
        R = (RAW10_Image[(h - 1) * RGB_width + (w + 0)]
          +  RAW10_Image[(h + 1) * RGB_width + (w + 0)]) >> 1;
        break;
    case 1:     // B center
        B =  RAW10_Image[(h + 0) * RGB_width + (w + 0)];
        G = (RAW10_Image[(h + 0) * RGB_width + (w - 1)]
          +  RAW10_Image[(h + 0) * RGB_width + (w + 1)]
          +  RAW10_Image[(h - 1) * RGB_width + (w + 0)]
          +  RAW10_Image[(h + 1) * RGB_width + (w + 0)]) >> 2;
        R = (RAW10_Image[(h - 1) * RGB_width + (w - 1)]
          +  RAW10_Image[(h - 1) * RGB_width + (w + 1)]
          +  RAW10_Image[(h + 1) * RGB_width + (w - 1)]
          +  RAW10_Image[(h + 1) * RGB_width + (w + 1)]) >> 2;
        break;
    case 2:     // R center
        R =  RAW10_Image[(h + 0) * RGB_width + (w + 0)];
        G = (RAW10_Image[(h + 0) * RGB_width + (w - 1)]
          +  RAW10_Image[(h + 0) * RGB_width + (w + 1)]
          +  RAW10_Image[(h - 1) * RGB_width + (w + 0)]
          +  RAW10_Image[(h + 1) * RGB_width + (w + 0)]) >> 2;
        B = (RAW10_Image[(h - 1) * RGB_width + (w - 1)]
          +  RAW10_Image[(h - 1) * RGB_width + (w + 1)]
          +  RAW10_Image[(h + 1) * RGB_width + (w - 1)]
          +  RAW10_Image[(h + 1) * RGB_width + (w + 1)]) >> 2;
        break;
    case 3:     // GR center
        G =  RAW10_Image[(h + 0) * RGB_width + (w + 0)];
        R = (RAW10_Image[(h + 0) * RGB_width + (w - 1)]
          +  RAW10_Image[(h + 0) * RGB_width + (w + 1)]) >> 1;
        B = (RAW10_Image[(h - 1) * RGB_width + (w + 0)]
          +  RAW10_Image[(h + 1) * RGB_width + (w + 0)]) >> 1;
        break;
    default:
        break;
    }
}

// Nearest Neighbor method
void NearestNeighbor()
{
    int w, h;
    int R, G, B;

    for (h = 0; h < RGB_height; h++)
    {
        for (w = 0; w < RGB_width; w++)
        {
            // Nearest Neighbor method Processing
            NearestNeighborProcessing(w, h, R, G, B);

            // Offset subtraction
            if(R > g_offset) R -= g_offset; else R = 0;
            if(G > g_offset) G -= g_offset; else G = 0;
            if(B > g_offset) B -= g_offset; else B = 0;

            // Gain adjustment
            R = (g_rgbGain[0] * R) >> 8;
            G = (g_rgbGain[1] * G) >> 8;
            B = (g_rgbGain[2] * B) >> 8;

            // Gamma correction
            if (g_gamma != 1.0f)
            {
                if (R > RAW10_MAX) R = RAW10_MAX;
                if (G > RAW10_MAX) G = RAW10_MAX;
                if (B > RAW10_MAX) B = RAW10_MAX;

                R = g_gammaTable[R];
                G = g_gammaTable[G];
                B = g_gammaTable[B];
            }
            else
            {
                R >>= 2;
                G >>= 2;
                B >>= 2;
            }

            // clip within 8bit 
            if (R > 255) R = 255;
            if (G > 255) G = 255;
            if (B > 255) B = 255;

            // store RGB image
            RGB_Image[(h * RGB_width + w) * 3 + 0] = B;
            RGB_Image[(h * RGB_width + w) * 3 + 1] = G;
            RGB_Image[(h * RGB_width + w) * 3 + 2] = R;
        }
    }
}

// Bilinear interpolation
void Bilinear()
{
    int w, h;
    int R, G, B;

    for (h = 0; h < RGB_height; h++)
    {
        for (w = 0; w < RGB_width; w++)
        {
            // Bilinear interpolation Processing
            BilinearProcessing(w, h, R, G, B);

            // Offset subtraction
            if(R > g_offset) R -= g_offset; else R = 0;
            if(G > g_offset) G -= g_offset; else G = 0;
            if(B > g_offset) B -= g_offset; else B = 0;

            // Gain adjustment
            R = (g_rgbGain[0] * R) >> 8;
            G = (g_rgbGain[1] * G) >> 8;
            B = (g_rgbGain[2] * B) >> 8;

            // Gamma correction
            if (g_gamma != 1.0f)
            {
                if (R > RAW10_MAX) R = RAW10_MAX;
                if (G > RAW10_MAX) G = RAW10_MAX;
                if (B > RAW10_MAX) B = RAW10_MAX;

                R = g_gammaTable[R];
                G = g_gammaTable[G];
                B = g_gammaTable[B];
            }
            else
            {
                R >>= 2;
                G >>= 2;
                B >>= 2;
            }

            // clip within 8bit 
            if (R > 255) R = 255;
            if (G > 255) G = 255;
            if (B > 255) B = 255;

            // store RGB image
            RGB_Image[(h * RGB_width + w) * 3 + 0] = B;
            RGB_Image[(h * RGB_width + w) * 3 + 1] = G;
            RGB_Image[(h * RGB_width + w) * 3 + 2] = R;
        }
    }
}

// Parse parameters
int parse_parameters(int argc, char *argv[])
{
    int i;
    int argchk = 0;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            if (i+1 < argc) {
                strncpy_s(g_inputFile, sizeof(g_inputFile), argv[i+1], sizeof(g_inputFile));
                printf("Input:%s\n", argv[i+1]);
                argchk++;
            }
        }
        if (strcmp(argv[i], "-o") == 0) {
            if (i+1 < argc) {
                strncpy_s(g_outputFile, sizeof(g_outputFile), argv[i+1], sizeof(g_outputFile));
                printf("Output:%s\n", argv[i+1]);
                argchk++;
            }
        }
        if (strcmp(argv[i], "-offset") == 0) {
            if (i+1 < argc) {
                g_offset = atoi(argv[i+1]);
                printf("Offset:%s\n", argv[i+1]);
            }
        }
        if (strcmp(argv[i], "-gain") == 0) {
            if (i+3 < argc) {
                g_rgbGain[0] = (int)(atof(argv[i+1]) * 0x100 + 0.5);
                g_rgbGain[1] = (int)(atof(argv[i+2]) * 0x100 + 0.5);
                g_rgbGain[2] = (int)(atof(argv[i+3]) * 0x100 + 0.5);
                printf("Gain:%s %s %s\n", argv[i+1], argv[i+2], argv[i+3]);
            }
        }
        if (strcmp(argv[i], "-gamma") == 0) {
            if (i+1 < argc) {
                g_gamma = atof(argv[i+1]);
                printf("Gamma:%s\n", argv[i+1]);
            }
        }
        if (strcmp(argv[i], "-interp") == 0) {
            if (i+1 < argc) {
                g_interpolation = atoi(argv[i+1]);
                printf("Interpolation:%s\n", argv[i+1]);
            }
        }
        if (strcmp(argv[i], "-flip") == 0) {
            if (i+1 < argc) {
                g_flip = atoi(argv[i+1]);
                printf("Flip:%s\n", argv[i+1]);
            }
        }
    }

    if (argchk < 1) {
        printf("Usage: %s -i RAW10_sample.bmp -o RGB24_sample.bmp -offset 16 -gain 2.0 2.0 2.0 -gamma 0.45\n", argv[0]);
        return -1;
    }

    return 0;
}

// Main procedure
int main(int argc, char* argv[])
{
    // parse parameter
    int ret = parse_parameters(argc, argv);
    if (ret < 0) {
        perror("Parameter error!");
        goto exit;
    }

    // load bitmap
    if (Load_bitmap(g_inputFile))
    {
        perror("Load bitmap error!");
        goto exit;
    }

    // convert width for RAW10 to RGB24
    RAW_width  = bitmapInfoHeader.biWidth;
    RAW_height = bitmapInfoHeader.biHeight;

    // Decompression size from RAW to RGB
    switch (RAW_width)
    {
    case 1700:  // 4056x3040
        RGB_width  = 4056;
        RGB_height = 3040;
        break;
    case 860:   // 2028x1520
        RGB_width  = 2028;
        RGB_height = 1520;
        break;
    case 440:   // 1012x760
        RGB_width  = 1012;
        RGB_height = 760;
        break;
    case 1600:  // 3840x2160
        RGB_width  = 3840;
        RGB_height = 2160;
        break;
    case 800:   // 1920x1080
        RGB_width  = 1920;
        RGB_height = 1080;
        break;
    case 340:   // 640x480
        RGB_width  = 640;
        RGB_height = 480;
        break;
    default:
        RGB_width  = RAW_width * 24 / 10;
        RGB_height = RAW_height - 2;
        break;
    }

    // allocate memory for RAW 10bit image data
    RAW10_Image = (unsigned short*)malloc(RGB_width * RGB_height * sizeof(unsigned short));
    if (!RAW10_Image)
    {
        perror("RAW10 image allocate error");
        goto exit;
    }

    // set new image size
    bitmapInfoHeader.biWidth     = RGB_width;
    bitmapInfoHeader.biHeight    = RGB_height;
    bitmapInfoHeader.biSizeImage = RGB_width * RGB_height * bitmapInfoHeader.biBitCount / 8;

    // allocate memory for RGB image data
    RGB_Image = (unsigned char*)malloc(bitmapInfoHeader.biSizeImage);
    if (!RGB_Image)
    {
        perror("RGB image allocate error");
        goto exit;
    }

    // update gamma table
    UpdateGammaTable(g_gammaTable, sizeof(g_gammaTable), g_gamma);

    // unpack RAW10 image
    Unpack_RAW10();

    // bayer conversion interpolation
    switch (g_interpolation)
    {
    case 0:     // Nearest Neighbor method
        NearestNeighbor();
        break;
    case 1:     // Bilinear interpolation
        Bilinear();
        break;
    default:
        break;
    }

    // save bitmap
    if (Save_bitmap(g_outputFile))
    {
        perror("Save bitmap error!");
    }

exit:
    // free RAW image data
    if (RAW_Image)
    {
        free(RAW_Image);
    }

    // free RAW10 image data
    if (RAW10_Image)
    {
        free(RAW10_Image);
    }

    // free RGB image data
    if (RGB_Image)
    {
        free(RGB_Image);
    }

    return 0;
}
