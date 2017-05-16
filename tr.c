#include<stdio.h>  
#include<iostream>  
  
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include<opencv2/imgproc/imgproc.hpp>  
  
#include<opencv2/core/core_c.h>  
#include<opencv2/imgproc/imgproc_c.h>  
  
using namespace std;  
using namespace cv;  
  
  
int main()  
{  
    char strFilename[100];                           //【1】定义一个字符数组保存----图片的存储路径  
    char strWindowname[100];                         //【2】定义一个字符数组保存----用于动态更新窗口的窗口名  
    char strFilenameAft[100];                        //【3】定义一个字符数组保存----用于给处理之后的图片动态的命名  
    int  j = 1;  
    for (int i = 720; i <=799; i++)  
    {                                                //【4】将图片的路径名动态的写入到strFilename这个地址的内存空间  
        sprintf_s(strFilename,    "D:\\mydata\\train\\%d.jpg", i);          
        sprintf_s(strFilenameAft, "D:\\mydata\\aft_train\\horse%d.jpeg", j);  
        sprintf_s(strWindowname,  "srcImg%d", j);  
  
        //===========================================================================================  
        //【模块1】将图片读入内存,并显示  
        //===========================================================================================  
        IplImage* pImg = cvLoadImage(strFilename);        //【1】从指定的路径,将图片加载到内存中  
        cvNamedWindow(strWindowname, CV_WINDOW_AUTOSIZE); //【2】创建一个显示图片的窗口  
        cvMoveWindow(strWindowname, 200, 200);            //【3】将显示窗口固定在(200,200)这个位置显示都进来的图片  
        cvShowImage(strWindowname, pImg);                 //【4】显示图片  
        //===========================================================================================  
        //【模块2】高斯金字塔下采样  
        //         1----将图片size调节到32*32左右,并显示  
        //         2----384*256:经过连续三次下采样,变为48*32  
        //============================================================================================  
        IplImage* pPyrDownImg  = cvCreateImage(cvSize(pImg->width / 2, pImg->height / 2), pImg->depth, pImg->nChannels);  
        IplImage* pPyrDownImg1 = cvCreateImage(cvSize(pPyrDownImg->width / 2, pPyrDownImg->height / 2), pPyrDownImg->depth, pPyrDownImg->nChannels);  
        IplImage* pPyrDownImg2 = cvCreateImage(cvSize(pPyrDownImg1->width / 2, pPyrDownImg1->height / 2), pPyrDownImg1->depth, pPyrDownImg1->nChannels);  
        cvPyrDown(pImg, pPyrDownImg, CV_GAUSSIAN_5x5);        //【1】384*256--->192*128  
        cvPyrDown(pPyrDownImg, pPyrDownImg1, CV_GAUSSIAN_5x5);//【2】192*128--->96*64  
        cvPyrDown(pPyrDownImg1,pPyrDownImg2, CV_GAUSSIAN_5x5);//【3】96*64----->48*32  
        cvNamedWindow("【PyrDown】", CV_WINDOW_AUTOSIZE);  
        cvMoveWindow ("【PyrDown】", 500, 200);  
        cvShowImage("【PyrDown】", pPyrDownImg2);  
        //============================================================================================  
        //【模块3】设置ROI区域裁剪图片----利用ROI区域将图片裁剪为:32*32  
        //============================================================================================  
        cvSetImageROI(pPyrDownImg2, cvRect(8, 0, 32, 32));//【1】设置一个32*32的ROI区域  
        cvNamedWindow("【ROI_Img】");  
        cvMoveWindow ("【ROI_Img】", 700, 200);  
        cvShowImage  ("【ROI_Img】", pPyrDownImg2);  
        cvSaveImage  (strFilenameAft, pPyrDownImg2);      //【2】将修改图片size之后的图片保存在指定的文件夹下  
        ++j;  
        cv::waitKey(10);  
        //============================================================================================  
        //【模块4】释放内存空间  
        //============================================================================================  
        cvReleaseImage(&pPyrDownImg);  
        cvReleaseImage(&pImg);                            //【1】释放掉存储图片的内存  
        cvDestroyWindow(strWindowname);                   //【2】销毁窗口的内存  
        cvDestroyWindow("【ROI_Img】");  
        cvDestroyWindow("【PyrDown】");  
    }  
  
    return 0;  
  
}  
