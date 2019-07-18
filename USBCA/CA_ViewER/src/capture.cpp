/*
Copyright (c) 2017-2019, CenturyArks
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

#include "stdafx.h"
#include "USBCAExtensionUnit.h"
#include "demo_control.h"
#include "capture.h"
#include "af_control.h"
#include "mode.h"


using namespace cv;
using namespace std;
using std::cout; using std::cerr; using std::endl;


/***************************************************************
 *  Global variable for Capture
 **************************************************************/
#define RAW10_MAX       (1023)          // RAW10 max value
#define BIT16_MAX       (65535)         // BIT16 max value
#define COMMAND_MAX     (256)           // command max
#define GAMMA_TABLE_NUM (1024)          // gamma table number
int nColorMatrix[3][3] = { 0 };         // color matrix for calculation
int RAW_WIDTH;                          // RAW10 pack width
int RAW_HEIGHT;                         // RAW10 pack height
int RGB_HEIGHT;                         // RGB width
int RGB_WIDTH;                          // RGB height
ushort g_gammaTable[GAMMA_TABLE_NUM];   // 16bit gamma correction table
double g_gamma_value     = 0.45;        // gamma value
bool g_bEnableGamma      = false;       // enable gamma
bool g_bSRGBGamma        = false;       // sRGB gamma
bool g_bEnableCCM        = false;       // enable CCM(Color Correction Matrix)
bool g_bRunCapture       = false;       // run captrue flag
bool g_bStillCapture     = false;       // still capture flag
bool g_bMovieRecord      = false;       // movie record flag
bool g_bMouseDown        = false;       // mouse down flag
bool g_bDispHistogram    = false;       // display histogram
bool g_bEnableContrast   = false;       // enable contrast
int g_iSelectMode        = 0;           // select mode
int g_iRecordNumber      = 0;           // movie record frame number
int g_iRecordCounter     = 0;           // movie record frame counter
int g_iConvertCounter    = 0;           // movie convert frame counter
HANDLE  g_hCaptureThread = NULL;        // capture thread handle
unsigned int g_uThreadID = 0;           // captrue thread ID
int g_mouse_x1           = 0;           // mouse down x
int g_mouse_y1           = 0;           // mouse donw y
int g_mouse_x2           = 0;           // mouse up x
int g_mouse_y2           = 0;           // mouse up y
int g_contrastValue      = 32;          // contrast value
double g_dAverageFps     = 0;           // average fps
size_t g_nCapturedFrames = 0;           // captured frames


/*******************************************************************************
 * @brief   Set select mode
 *
 * @param   mode        Select mode
 *
 * @return  void
 ******************************************************************************/
void SetSelectMode(int mode)
{
    g_iSelectMode = mode;
}

/*******************************************************************************
 * @brief   Set gamma value
 *
 * @param   value       Gamma value
 *
 * @return  void
 ******************************************************************************/
void SetGammaValue(double value)
{
    g_gamma_value = value;
}

/*******************************************************************************
 * @brief   Enable gamma
 *
 * @param   enable      Disable/Enable
 *
 * @return  void
 ******************************************************************************/
void EnableGamma(bool enable)
{
    g_bEnableGamma = enable;
}

/*******************************************************************************
 * @brief   Enable CCM(Color Correction Matrix)
 *
 * @param   enable      Disable/Enable
 *
 * @return  void
 ******************************************************************************/
void EnableCCM(bool enable)
{
    g_bEnableCCM = enable;
}

/*******************************************************************************
 * @brief   Enable contrast
 *
 * @param   enable      Disable/Enable
 * @param   value       Contrast value
 *
 * @return  void
 ******************************************************************************/
void EnableContrast(bool enable, int value)
{
    g_bEnableContrast = enable;
    g_contrastValue = value;
}

/*******************************************************************************
 * @brief   Display histogram
 *
 * @param   enable      Disable/Enable
 *
 * @return  void
 ******************************************************************************/
void DisplayHistogram(bool enable)
{
    g_bDispHistogram = enable;
}

/*******************************************************************************
 * @brief   Update gamma table
 *
 * @param   gamma       Gamma value
 *
 * @return  void
 ******************************************************************************/
void UpdateGammaTable(double gamma)
{
    int i, v;

    for (i = 0; i < GAMMA_TABLE_NUM; i++)
    {
        v = (int)(pow(i / (double)GAMMA_TABLE_NUM, gamma) * BIT16_MAX + 0.5);
        if (v > BIT16_MAX) v = BIT16_MAX;
        g_gammaTable[i] = v;
    }
}

/*******************************************************************************
 * @brief   Update sRGB gamma table
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void UpdateSRGBGammaTable(void)
{
    int i, v;
    double a = 0.055;
    double th = 0.0031308;

    for (i = 0; i < GAMMA_TABLE_NUM; i++)
    {
        double dValue = i / (double)GAMMA_TABLE_NUM;
        if (dValue < th)
        {
            v = (int)(dValue * 12.92 * BIT16_MAX + 0.5);
        }
        else
        {
            v = (int)(((1 + a) * pow(dValue, 1.0 / 2.4) - a) * BIT16_MAX + 0.5);
        }
        if (v > BIT16_MAX) v = BIT16_MAX;
        g_gammaTable[i] = v;
    }
}

/*******************************************************************************
 * @brief   Update CCM(Color Correction Matrix) table
 *
 * @param   ccm         Color Correction Matrix
 *
 * @return  void
 ******************************************************************************/
void UpdateCCMTable(double ccm[3][3])
{
    int x, y;

    for (y = 0; y < 3; y++)
    {
        for (x = 0; x < 3; x++)
        {
            nColorMatrix[y][x] = (int)(ccm[y][x] * 256);
        }
    }
}

/*******************************************************************************
 * @brief   Image processing for RAW10
 *
 * @param   SrcImage        Source image pointer
 * @param   DstImage        Destination image pointer
 *
 * @return  void
 ******************************************************************************/
static void ImageProcRAW10(uchar *SrcImage, ushort *DstImage)
{
    int w, h;
    int P0, P1, P2, P3, P4;
    uchar *pSrc;
    ushort *pDst;

    int DATA_LINE = RAW_HEIGHT - RGB_HEIGHT;

    for (h = 0; h < RGB_HEIGHT; h++)
    {
        pSrc = &SrcImage[(h + DATA_LINE) * RAW_WIDTH * 3];
        pDst = &DstImage[(RGB_HEIGHT - 1 - h) * RGB_WIDTH];

        for (w = 0; w < RGB_WIDTH; w += 4)
        {
            // 10bit RAW format
            //          bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
            // Byte 0:  P0[9]   P0[8]   P0[7]   P0[6]   P0[5]   P0[4]   P0[3]   P0[2]
            // Byte 1:  P1[9]   P1[8]   P1[7]   P1[6]   P1[5]   P1[4]   P1[3]   P1[2]
            // Byte 2:  P2[9]   P2[8]   P2[7]   P2[6]   P2[5]   P2[4]   P2[3]   P2[2]
            // Byte 3:  P3[9]   P3[8]   P3[7]   P3[6]   P3[5]   P3[4]   P3[3]   P3[2]
            // Byte 4:  P3[1]   P3[0]   P2[1]   P2[0]   P1[1]   P1[0]   P0[1]   P0[0]
            P4 =  pSrc[4];
            P3 = (pSrc[3] << 2) + ((P4 >> 6) & 0x3);
            P2 = (pSrc[2] << 2) + ((P4 >> 4) & 0x3);
            P1 = (pSrc[1] << 2) + ((P4 >> 2) & 0x3);
            P0 = (pSrc[0] << 2) + ((P4 >> 0) & 0x3);
            pSrc += 5;

            // Gamma correction
            if (g_bEnableGamma)
            {
                P0 = g_gammaTable[P0];
                P1 = g_gammaTable[P1];
                P2 = g_gammaTable[P2];
                P3 = g_gammaTable[P3];
            }
            else
            {
                // 10bit -> 16bit
                P0 <<= 6;
                P1 <<= 6;
                P2 <<= 6;
                P3 <<= 6;
            }

            // Store 16bit data
            pDst[0] = P0;
            pDst[1] = P1;
            pDst[2] = P2;
            pDst[3] = P3;
            pDst += 4;
        }
    }
}

/*******************************************************************************
 * @brief   Color Corretion Matrix
 *
 * @param   TargetImage     Target image pointer
 *
 * @return  void
 ******************************************************************************/
static void ColorCorrctionMatrix(ushort *TargetImage)
{
    int w, h;
    int R0, G0, B0;
    int R1, G1, B1;
    int max, min;
    ushort *pTarget;

    for (h = 0; h < RGB_HEIGHT; h++)
    {
        pTarget = &TargetImage[h * RGB_WIDTH * 3];

        for (w = 0; w < RGB_WIDTH; w++)
        {
            R0 = pTarget[0];
            G0 = pTarget[1];
            B0 = pTarget[2];

            R1 = (R0 * nColorMatrix[0][0] + G0 * nColorMatrix[0][1] + B0 * nColorMatrix[0][2]);
            G1 = (R0 * nColorMatrix[1][0] + G0 * nColorMatrix[1][1] + B0 * nColorMatrix[1][2]);
            B1 = (R0 * nColorMatrix[2][0] + G0 * nColorMatrix[2][1] + B0 * nColorMatrix[2][2]);
            if (R1 < 0) R1 = 0;
            if (G1 < 0) G1 = 0;
            if (B1 < 0) B1 = 0;
            R1 >>= 8;
            G1 >>= 8;
            B1 >>= 8;

            if (g_bEnableContrast)
            {
                max = 255 - g_contrastValue;
                min = g_contrastValue;
                R1 = (R1 - min * 256) * 255 / (max - min);
                G1 = (G1 - min * 256) * 255 / (max - min);
                B1 = (B1 - min * 256) * 255 / (max - min);
                if (R1 < 0) R1 = 0;
                if (G1 < 0) G1 = 0;
                if (B1 < 0) B1 = 0;
            }

            if (R1 > BIT16_MAX) R1 = BIT16_MAX;
            if (G1 > BIT16_MAX) G1 = BIT16_MAX;
            if (B1 > BIT16_MAX) B1 = BIT16_MAX;

            pTarget[0] = R1;
            pTarget[1] = G1;
            pTarget[2] = B1;
            pTarget += 3;
        }
    }
}

/*******************************************************************************
 * @brief   Contrast Correction
 *
 * @param   TargetImage     Target image pointer
 *
 * @return  void
 ******************************************************************************/
static void ContrastCorrection(ushort *TargetImage)
{
    int w, h;
    int R, G, B;
    int max, min;
    ushort *pTarget;

    for (h = 0; h < RGB_HEIGHT; h++)
    {
        pTarget = &TargetImage[h * RGB_WIDTH * 3];

        for (w = 0; w < RGB_WIDTH; w++)
        {
            R = pTarget[0];
            G = pTarget[1];
            B = pTarget[2];

            max = 255 - g_contrastValue;
            min = g_contrastValue;
            R = (R - min * 256) * 255 / (max - min);
            G = (G - min * 256) * 255 / (max - min);
            B = (B - min * 256) * 255 / (max - min);

            if (R < 0) R = 0;
            if (G < 0) G = 0;
            if (B < 0) B = 0;
            if (R > BIT16_MAX) R = BIT16_MAX;
            if (G > BIT16_MAX) G = BIT16_MAX;
            if (B > BIT16_MAX) B = BIT16_MAX;

            pTarget[0] = R;
            pTarget[1] = G;
            pTarget[2] = B;
            pTarget += 3;
        }
    }
}

/*******************************************************************************
 * @brief   Get time string
 *
 * @param   strtime         time string
 *
 * @return  void
 ******************************************************************************/
void GetTimeString(char* strtime)
{
    time_t timer;
    struct tm local;

    // Get current time
    time(&timer);

    // Convert local time
    localtime_s(&local, &timer);

    sprintf_s(strtime, _MAX_FNAME, "%04d%02d%02d_%02d%02d%02d",
        local.tm_year + 1900, local.tm_mon + 1, local.tm_mday,
        local.tm_hour, local.tm_min, local.tm_sec);
}

/*******************************************************************************
 * @brief   Save uncompressed RGB AVI file from RAW file
 *
 * @param   videoRawFilename    input RAW video filename
 * @param   videoRgbFilename    output RGB vodeo filename
 * @param   FPS                 framerate
 *
 * @return  void
 ******************************************************************************/
void SaveRGB_AVI(char* videoRawFilename, char* videoRgbFilename, int FPS)
{
    Mat frame, bayer_img, rgb_img, img_avi;
    VideoCapture capture(videoRawFilename);
    VideoWriter movie;

    bayer_img = Mat(RGB_HEIGHT, RGB_WIDTH, CV_16UC1);
    rgb_img = Mat(RGB_HEIGHT, RGB_WIDTH, CV_16UC3);

    // RGB uncompressed format
    int fourcc = CV_FOURCC('D', 'I', 'B', ' ');
    movie.open(videoRgbFilename, fourcc, FPS, Size(RGB_WIDTH, RGB_HEIGHT));

    g_iConvertCounter = 0;

    while (g_bMovieRecord)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            break;
        }
 
        // Image processing for RAW10
        ImageProcRAW10(frame.data, (ushort *)bayer_img.data);

        // Convert Bayer RGGB to RGB
        cvtColor(bayer_img, rgb_img, CV_BayerRG2RGB);

        // Color Corrction Matrix
        if (g_bEnableCCM)
        {
            ColorCorrctionMatrix((ushort *)rgb_img.data);
        }

        // Contrast correction
        if (g_bEnableContrast)
        {
            ContrastCorrection((ushort *)rgb_img.data);
        }

        rgb_img.convertTo(img_avi, CV_8UC3, 1/256.0);

        movie << img_avi;

        g_iConvertCounter++;
    }

    frame.release();
    bayer_img.release();
    rgb_img.release();
    img_avi.release();
    capture.release();
    movie.release();
}

/*******************************************************************************
 * @brief   Mouse event callback function
 *
 * @param   evnet           Mouse eveet
 * @param   x               Mouse X coordinate
 * @param   y               Mouse Y coordinate
 *
 * @return  void
 ******************************************************************************/
static void onMouse(int event, int x, int y, int, void*)
{
    switch (event)
    {
    case cv::EVENT_LBUTTONDOWN:
        g_mouse_x1 = x;
        g_mouse_y1 = y;
        g_mouse_x2 = x;
        g_mouse_y2 = y;
        g_bMouseDown = true;
        //cout << format("MouseDown(%d, %d)", x, y) << endl;
        break;
    case cv::EVENT_LBUTTONUP:
        if (x < 0)
        {
            g_mouse_x2 = 0;
        }
        else if (x > RGB_WIDTH - 1)
        {
            g_mouse_x2 = RGB_WIDTH - 1;
        }
        else
        {
            g_mouse_x2 = x;
        }
        if (y < 0)
        {
            g_mouse_y2 = 0;
        }
        else if (y > RGB_HEIGHT - 1)
        {
            g_mouse_y2 = RGB_HEIGHT - 1;
        }
        else
        {
            g_mouse_y2 = y;
        }
        g_bMouseDown = false;
        //cout << format("MouseUp(%d, %d)", x, y) << endl;
        break;
    case cv::EVENT_MOUSEMOVE:
        if (g_bMouseDown)
        {
            if (x < 0)
            {
                g_mouse_x2 = 0;
            }
            else if (x > RGB_WIDTH - 1)
            {
                g_mouse_x2 = RGB_WIDTH - 1;
            }
            else
            {
                g_mouse_x2 = x;
            }
            if (y < 0)
            {
                g_mouse_y2 = 0;
            }
            else if (y > RGB_HEIGHT - 1)
            {
                g_mouse_y2 = RGB_HEIGHT - 1;
            }
            else
            {
                g_mouse_y2 = y;
            }
            //cout << format("MouseMove(%d, %d)", x, y) << endl;
        }
        break;
    }
}

/*******************************************************************************
 * @brief   Draw Histogram
 *
 * @param   rgb_img         RGB Image
 *
 * @return  void
 *
 * https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
 ******************************************************************************/
void DrawHistogram(Mat rgb_img)
{
    int cut_x, cut_y, cut_w, cut_h;
    if (g_mouse_x1 <= g_mouse_x2)
    {
        cut_x = g_mouse_x1;
        cut_w = g_mouse_x2 - g_mouse_x1 + 1;
    }
    else
    {
        cut_x = g_mouse_x2;
        cut_w = g_mouse_x1 - g_mouse_x2 + 1;
    }

    if (g_mouse_y1 <= g_mouse_y2)
    {
        cut_y = g_mouse_y1;
        cut_h = g_mouse_y2 - g_mouse_y1 + 1;
    }
    else
    {
        cut_y = g_mouse_y2;
        cut_h = g_mouse_y1 - g_mouse_y2 + 1;
    }

    cv::Mat cut_img(rgb_img, cv::Rect(cut_x, cut_y, cut_w, cut_h));

    // Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split(cut_img, bgr_planes);

    // Establish the number of bins
    int histSize = 256;

    // Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 };
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat b_hist, g_hist, r_hist;

    // Compute the histograms:
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 200;
    int offset_x = 25, offset_y = 20;
    int bin_w = cvRound((double)hist_w / histSize);

    Mat histImage(hist_h + offset_y * 2, hist_w + offset_x * 2, CV_8UC3, Scalar(255, 255, 255));

    // Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows - offset_y * 2, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, histImage.rows - offset_y * 2, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, histImage.rows - offset_y * 2, NORM_MINMAX, -1, Mat());

    for (int x = 0; x <= 16; x++)
    {
        int line_w = x * hist_w / 16;
        line(histImage,
            Point(line_w + offset_x, offset_y),
            Point(line_w + offset_x, hist_h + offset_y),
            Scalar(0, 0, 0), 1, 8, 0);

        putText(histImage, format("%d", (x == 16) ? 255 : (x * 16)),
            Point(line_w + 18, hist_h + 33), FONT_HERSHEY_PLAIN, 0.6, Scalar(0, 0, 0), 1, CV_AA);
    }

    for (int y = 0; y <= 10; y++)
    {
        int line_h = y * hist_h / 10;
        line(histImage,
            Point(offset_x, line_h + offset_y),
            Point(hist_w + offset_x, line_h + offset_y),
            Scalar(0, 0, 0), 1, 8, 0);

        putText(histImage, format("%d", 100 - y * 10),
            Point(3, line_h + 23), FONT_HERSHEY_PLAIN, 0.6, Scalar(0, 0, 0), 1, CV_AA);
    }

    // Draw for each channel
    for (int i = 1; i < histSize; i++)
    {
        line(histImage,
            Point(bin_w*(i - 1) + offset_x, hist_h - cvRound(b_hist.at<float>(i - 1)) + offset_y),
            Point(bin_w*(i    ) + offset_x, hist_h - cvRound(b_hist.at<float>(i    )) + offset_y),
            Scalar(255, 0, 0), 2, 8, 0);
        line(histImage,
            Point(bin_w*(i - 1) + offset_x, hist_h - cvRound(g_hist.at<float>(i - 1)) + offset_y),
            Point(bin_w*(i    ) + offset_x, hist_h - cvRound(g_hist.at<float>(i    )) + offset_y),
            Scalar(0, 255, 0), 2, 8, 0);
        line(histImage,
            Point(bin_w*(i - 1) + offset_x, hist_h - cvRound(r_hist.at<float>(i - 1)) + offset_y),
            Point(bin_w*(i    ) + offset_x, hist_h - cvRound(r_hist.at<float>(i    )) + offset_y),
            Scalar(0, 0, 255), 2, 8, 0);
    }

    putText(histImage, format("R = %3.1lf", mean(bgr_planes[2])[0]),
        Point(100, 15), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 1, CV_AA);
    putText(histImage, format("G = %3.1lf", mean(bgr_planes[1])[0]),
        Point(200, 15), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 255, 0), 1, CV_AA);
    putText(histImage, format("B = %3.1lf", mean(bgr_planes[0])[0]),
        Point(300, 15), FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 0, 0), 1, CV_AA);

    // Display
    namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
    imshow("Histogram", histImage);
}

/*******************************************************************************
 * @brief   Capture thread
 *
 * @param   pArguments      Argument list pointer
 *
 * @return  0 Success, other Failure
 ******************************************************************************/
unsigned __stdcall capture_thrread(void* pArguments)
{
    Mat frame, bayer_img, rgb_img;
    VideoCapture capture;
    VideoWriter movie;

    g_bRunCapture = true;

    //
    // Select mode
    //
    //  mode0:(4056x3040@10)
    //  mode1:(2028x1520@30)
    //  mode2:(1012x760@60)
    //  mode3:(3840x2160@12)
    //  mode4:(1920x1080@48)
    //  mode5:(640x480@60)
    int mode = g_iSelectMode;

    //
    // Set Image size
    //
    RAW_WIDTH  = MODE_LIST[usbtype][mode].pack_width;
    RAW_HEIGHT = MODE_LIST[usbtype][mode].pack_height;
    RGB_WIDTH  = MODE_LIST[usbtype][mode].width;
    RGB_HEIGHT = MODE_LIST[usbtype][mode].height;
    bayer_img = Mat(RGB_HEIGHT, RGB_WIDTH, CV_16UC1);
    rgb_img   = Mat(RGB_HEIGHT, RGB_WIDTH, CV_16UC3);

    int DATA_LINE = RAW_HEIGHT - RGB_HEIGHT;

    String viewWindowTitle = "CA_ViewER";
#if RESIZE_WINDOW
    cvNamedWindow(viewWindowTitle, cv::WINDOW_NORMAL);
    cvMoveWindow(viewWindowTitle, 0, 0);
    cvResizeWindow(viewWindowTitle, 3840, 2160);
#else
    namedWindow(viewWindowTitle, cv::WINDOW_FULLSCREEN);
#endif

    // Mouse event setting
    setMouseCallback(viewWindowTitle, onMouse, 0);
    g_mouse_x1 = 0;
    g_mouse_y1 = 0;
    g_mouse_x2 = RGB_WIDTH - 1;
    g_mouse_y2 = RGB_HEIGHT - 1;

    //
    // Open camera
    //
    cout << "Opening camera..." << endl;

    for (int devID = 0; devID < 8; devID++)
    {
        // Open the device in DirectShow
        capture.open(devID, CAP_DSHOW);

        // Check camera device is opened
        if (!capture.isOpened())
        {
            //printf("Failed to open camera.\r\n");
            cerr << "ERROR: Can't initialize camera capture" << endl;
            return 1;
        }

        capture.set(CV_CAP_PROP_FRAME_WIDTH, RAW_WIDTH);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, RAW_HEIGHT);
        capture.set(CV_CAP_PROP_FPS, MODE_LIST[usbtype][mode].fps);
        printf("fps=%.1lf, %.lf x %.lf\r\n",
            capture.get(CV_CAP_PROP_FPS),
            capture.get(CV_CAP_PROP_FRAME_WIDTH),
            capture.get(CV_CAP_PROP_FRAME_HEIGHT));

        if (capture.get(CV_CAP_PROP_FRAME_WIDTH) == RAW_WIDTH &&
            capture.get(CV_CAP_PROP_FRAME_HEIGHT) == RAW_HEIGHT)
        {
            break;
        }

        capture.release();
    }

    cout << "Frame width: " << capture.get(CAP_PROP_FRAME_WIDTH) << endl;
    cout << "     height: " << capture.get(CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "Capturing FPS: " << capture.get(CAP_PROP_FPS) << endl;
    cout << endl << "Start Capturing..." << endl;

    //
    // Update gamma table
    //
    if (g_bSRGBGamma)
    {
        UpdateSRGBGammaTable();
    }
    else
    {
        UpdateGammaTable(g_gamma_value);
    }

    //
    // Start capture
    //
    int FPS = (int)MODE_LIST[usbtype][mode].fps;
    size_t nFrames = 0;
    int64 t0 = getTickCount();
    while (g_bRunCapture)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            cerr << "ERROR: Can't grab camera frame." << endl;
            break;
        }

        if (g_bMovieRecord)
        {
            char videoRawFilename[_MAX_FNAME];
            char videoRgbFilename[_MAX_FNAME];
            char strtime[_MAX_FNAME];

            if (g_iRecordCounter == 0)
            {
                GetTimeString(strtime);

                sprintf_s(videoRawFilename, "video_raw10_%s.avi", strtime);
                sprintf_s(videoRgbFilename, "video_rgb_%s.avi", strtime);

                // RGB uncompressed format
                int fourcc = CV_FOURCC('D', 'I', 'B', ' ');
                movie.open(videoRawFilename, fourcc, FPS, Size(RAW_WIDTH, RAW_HEIGHT));
                t0 = getTickCount();
            }

            movie << frame;

            g_iRecordCounter++;

            if (g_iRecordCounter >= g_iRecordNumber)
            {
                movie.release();

                int64 t1 = getTickCount();
                cout << "Record FPS: "
                     << format("%4.1f", (double)getTickFrequency() * (g_iRecordNumber) / (t1 - t0))
                     << endl;

                // Save uncompressed RGB AVI file from RAW file
                SaveRGB_AVI(videoRawFilename, videoRgbFilename, FPS);

                g_bMovieRecord = false;
            }
        }
        else
        {
            nFrames++;
            if (nFrames % FPS == 0)
            {
              int64 t1 = cv::getTickCount();
              cout << "Frames captured: " << format("%5lld", (long long int)nFrames)
                  << "  Average FPS: " << format("%4.1f", (double)getTickFrequency() * (FPS) / (t1 - t0))
                  << endl;
              g_nCapturedFrames = nFrames;
              g_dAverageFps = (double)getTickFrequency() * (FPS) / (t1 - t0);
              t0 = t1;
            }

            //
            // PDAF Control
            //
            PDAF_Control(frame.data, 3 * RAW_WIDTH * DATA_LINE, PDAF_WMAX, PDAF_HMAX);

            // Image processing for RAW10
            ImageProcRAW10(frame.data, (ushort *)bayer_img.data);

            // Convert Bayer RGGB to RGB
            cvtColor(bayer_img, rgb_img, CV_BayerRG2RGB);

            // Color Corrction Matrix
            if (g_bEnableCCM)
            {
                ColorCorrctionMatrix((ushort *)rgb_img.data);
            }

            // Contrast correction
            if (g_bEnableContrast)
            {
                ContrastCorrection((ushort *)rgb_img.data);
            }

            if (g_bStillCapture)
            {
                g_bStillCapture = false;

                char rawFilename[_MAX_FNAME];
                char rgbFilename[_MAX_FNAME];
                char strtime[_MAX_FNAME];

                GetTimeString(strtime);

                sprintf_s(rawFilename, "raw10_%s.bmp", strtime);    // *.bmp -> 24bit
                sprintf_s(rgbFilename, "rgb_%s.tif", strtime);      // *.tif/*.png -> 48bit

                // Save frame image
                imwrite(rawFilename, frame);
                imwrite(rgbFilename, rgb_img);

                cout << format("Save RAW10 image: %s", rawFilename) << endl
                    << format("Save RGB image: %s", rgbFilename) << endl;
            }

            if (g_bDispHistogram)
            {
                // Convert 16bit to 8bit
                rgb_img.convertTo(rgb_img, CV_8UC3, 255.0 / 65535.0);

                // Update histogram once per second
                if (nFrames % FPS == 0)
                {
                    DrawHistogram(rgb_img);
                }

                // Draw selection area
                rectangle(rgb_img,
                    Point(g_mouse_x1, g_mouse_y1),
                    Point(g_mouse_x2, g_mouse_y2),
                    Scalar(0, 0, 255), 2, 8, 0);
            }

            // Display image
            imshow(viewWindowTitle, rgb_img);

            // Wait for a pressed key
            waitKey(1);
        }
    }

    cout << "Number of captured frames: " << nFrames << endl;

    frame.release();
    bayer_img.release();
    rgb_img.release();
    capture.release();
    movie.release();    
    destroyAllWindows();
    _endthreadex(0);

    return 0;
}

/*******************************************************************************
 * @brief   Start capture
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void StartCapture(void)
{
    g_hCaptureThread = (HANDLE)_beginthreadex(
        NULL, 0, capture_thrread, NULL, 0, &g_uThreadID);
}

/*******************************************************************************
 * @brief   Stop capture
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void StopCapture(void)
{
    if (g_hCaptureThread)
    {
        g_bRunCapture = false;
        WaitForSingleObject(g_hCaptureThread, 5000);
        CloseHandle(g_hCaptureThread);
        g_hCaptureThread = NULL;
    }
}

/*******************************************************************************
 * @brief   Still capture
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void StillCapture(void)
{
    g_bStillCapture = true;
}

/*******************************************************************************
 * @brief   Movie Record
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MovieRecord(int frame)
{
    if (frame == 0)
    {
        g_bMovieRecord = false;
    }
    else
    {
        g_iRecordCounter = 0;
        g_iRecordNumber = frame;
        g_bMovieRecord = true;
    }
}

/*******************************************************************************
 * @brief   Get Max Record Frame
 *
 * @param   mode        Select mode
 *
 * @return  Max Record Frame
 ******************************************************************************/
int GetMaxRecordFrame(int mode)
{
    return 0xFFFFFFFF / (MODE_LIST[usbtype][mode].width * MODE_LIST[usbtype][mode].height * 3);
}

/*******************************************************************************
 * @brief   Get record progress
 *
 * @param   void
 *
 * @return  record counter
 ******************************************************************************/
int GetRecordProgress(void)
{
    return g_iRecordCounter;
}

/*******************************************************************************
 * @brief   Get convert progress
 *
 * @param   void
 *
 * @return  convert counter
 ******************************************************************************/
int GetConvertProgress(void)
{
    return g_iConvertCounter;
}

/*******************************************************************************
 * @brief   Get Average FPS
 *
 * @param   void
 *
 * @return  Average FPS
 ******************************************************************************/
double GetAverageFps(void)
{
    return g_dAverageFps;
}

/*******************************************************************************
 * @brief   Get Captured Frames
 *
 * @param   void
 *
 * @return  Captured Frames
 ******************************************************************************/
size_t GetCapturedFrames(void)
{
    return g_nCapturedFrames;
}
