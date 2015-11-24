#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace cv;

Mat src,srcgray,dst;
Mat cannyout;
const int max_threshold = 255;
int thresh;
float radio = 3.0;
char *title1 = "Eige";
RNG rng(1234); 

//time;
double frequency = getTickFrequency();

void onthresh(int,void*)
{
	int64 tos2 = getTickCount();

	Canny(srcgray,cannyout,thresh,thresh*radio,3);

	Mat Morphelement = getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(cannyout,cannyout,MORPH_CLOSE,Morphelement);

	

	vector<vector<Point> > contours;
	vector<Vec4i> heirarchy;
	findContours(cannyout,contours,heirarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,Point(0,0));

	//connects the bounderPoints,because the edges of some objects are disconnected ; 

	for (unsigned int i = 0; i < contours.size(); i++ )
	{
		vector<Point> bounderPoints;
		for (unsigned int j = 0; j < contours[i].size(); j++ )
		{
			if (contours[i][j].x == 1 || contours[i][j].y == 1 || contours[i][j].x ==cannyout.cols-2  || contours[i][j].y == cannyout.rows-2 )
			{
				bounderPoints.push_back(contours[i][j]) ;
			}
		}
		unsigned int len = bounderPoints.size();
		for (unsigned k = 0; k < len ; k++)
		{
			line( cannyout, bounderPoints[k], bounderPoints[(k+1)%len], Scalar(255,255,255), 1, 8 );
		}
	}
	
	findContours(cannyout,contours,heirarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,Point(0,0));

	////connects the bounderPoints

	//for (unsigned int i = 0; i < contours.size(); i++ )
	//{
	//	vector<Point> bounderPoints;
	//	for (unsigned int j = 0; j < contours[i].size(); j++ )
	//	{
	//		if (contours[i][j].x == 0 || contours[i][j].y == 0)
	//		{
	//			bounderPoints.push_back(contours[i][j]) ;
	//		}
	//	}
	//	unsigned int len = bounderPoints.size();
	//	for (unsigned k = 0; k < len ; k++)
	//	{
	//		line( srccop, bounderPoints[k], bounderPoints[(k+1)%len], color, 1, 8 );
	//	}
	//}


	Mat srccop;
	srccop.create(srcgray.size(),srcgray.type());
	srccop = Scalar::all(0);
	vector<RotatedRect> minRect( contours.size() );
	vector<Point2f> vcenters;
	for( int i = 0; i < contours.size(); i++ )
	{
		minRect[i] = minAreaRect( Mat(contours[i]) );
		vcenters.push_back(minRect[i].center) ;
	}

	int64 toe2 = getTickCount();
	std::cout<<"ms time2:" << (toe2 - tos2 )/frequency*1000 <<std::endl;

	std::cout<<"countours number:"<<contours.size()<<std::endl;

	for( int i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar( 255, 255, 255 );
		drawContours( srccop, contours, i, color, 1 , 8, heirarchy, 0, Point() );

		Point2f rect_points[4]; 
		minRect[i].points( rect_points );

		circle(srccop,minRect[i].center,3,color,-1);
		for( int j = 0; j < 4; j++ )
			line( srccop, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
	}

	////connects the bounderPoints

	//for (unsigned int i = 0; i < contours.size(); i++ )
	//{
	//	vector<Point> bounderPoints;
	//	for (unsigned int j = 0; j < contours[i].size(); j++ )
	//	{
	//		if (contours[i][j].x == 1 || contours[i][j].y == 1)
	//		{
	//			bounderPoints.push_back(contours[i][j]) ;
	//		}
	//	}
	//	unsigned int len = bounderPoints.size();
	//	for (unsigned k = 0; k < len ; k++)
	//	{
	//		line( srccop, bounderPoints[k], bounderPoints[(k+1)%len], Scalar(255,255,255), 1, 8 );
	//	}
	//}

	/// Show in a window
	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", srccop );

	////试图用已经测定的巨型轮廓来得到封闭的轮廓，由于目标与图像边缘重合方法不适用;
	//vector<vector<Point> > contoursHandled;
	//vector<Vec4i> heirarchyHandled;
	//findContours(srccop,contoursHandled,heirarchyHandled,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,Point(0,0));

	//Mat srccopHandled;
	//srccopHandled.create(srcgray.size(),srcgray.type());
	//srccopHandled = Scalar::all(0);

	//for( int i = 0; i< contoursHandled.size(); i++ )
	//{
	//	Scalar color = Scalar( 255, 255, 255 );
	//	drawContours( srccopHandled, contoursHandled, i, color, 1 , 8, heirarchyHandled, 0, Point() );

	//	Point2f rect_points[4]; minRect[i].points( rect_points );

	//	circle(srccop,minRect[i].center,2,color,-1);
	//}

	///// Show in a window
	//namedWindow( "ContoursHandled", CV_WINDOW_AUTOSIZE );
	//imshow( "ContoursHandled", srccopHandled );
}

int main()
{
	//input the image,mediablur it,;
	int64 tos1 = getTickCount();


	src = imread("2s.jpg");
	if (!src.data)
	{
		std::cout<<"load image fail!"<<std::endl;
		exit(0);
	}
	Mat srcBlur;
	medianBlur(src,srcBlur,3);
	Mat Morphelement = getStructuringElement(MORPH_RECT,Size(7,7));
	Mat srcMroph;
	morphologyEx(src,srcMroph,MORPH_CLOSE,Morphelement);

	/*int bordersize = 2;
	Mat borderimg;
	Scalar value = Scalar(srcMroph.at<Vec3b>(60,1)[0],srcMroph.at<Vec3b>(60,1)[1],srcMroph.at<Vec3b>(60,1)[2]);
	copyMakeBorder( srcMroph, borderimg, bordersize, bordersize, bordersize, bordersize, BORDER_CONSTANT, value );
	namedWindow("borderimg",1);
	imshow("borderimg",borderimg);
	waitKey(0);*/

	//get the gray image;
	cvtColor(srcMroph,srcgray,CV_BGR2GRAY);
	//cvtColor(borderimg,srcgray,CV_BGR2GRAY);
	namedWindow("grayimg",CV_WINDOW_AUTOSIZE);
	imshow("grayimg",srcgray);

	//gray hist;
	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 } ;
	const float* histRange =  { range } ;
	//const int channel = 0;

	bool uniform = true; bool accumulate = false;
	Mat Hist;
	calcHist(&srcgray,1,0,Mat(),Hist,1,&histSize,&histRange,uniform,accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(Hist, Hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(Hist.at<float>(i-1)) ) ,
			Point( bin_w*(i), hist_h - cvRound(Hist.at<float>(i)) ),
			Scalar( 255, 0, 0), 2, 8, 0  );
	}

	/// Display
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
	imshow("calcHist Demo", histImage );

	waitKey(0);

	/*int bordersize = 2;
	Mat borderimg;
	Scalar value = Scalar::all(srcgray.at<int>(60,1));
	 copyMakeBorder( srcgray, borderimg, bordersize, bordersize, bordersize, bordersize, BORDER_CONSTANT, value );
*/

	int64 toe1 = getTickCount();
	std::cout<< "ms time1:" << (toe1-tos1)/frequency*1000 << std::endl;
	//create window
	namedWindow(title1,CV_WINDOW_AUTOSIZE);
	imshow(title1,src);

	//create trackbar;
	thresh = 43;
	createTrackbar("threshold",title1,&thresh,max_threshold,onthresh);
	onthresh( 0 , 0 );
	waitKey(0);


	return 0;
}
