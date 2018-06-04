#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"

std::ostringstream ttext;
cv::Mat src,dst,roi_g;
int src_gray,m,radius=0;
cv::Point lk;
std::vector<cv::Vec3f> roi;
using namespace cv;
using namespace std;
RNG rng(12345);

//Function for detecting centre and radius of circle
  int countcir(cv::Mat roi)
{
  cvtColor(roi, roi_g, CV_RGB2GRAY);
  cv::threshold(roi_g, roi_g, 200, 255, cv::THRESH_BINARY | CV_THRESH_OTSU );
  vector<Vec3f> cir;
//Detecting circle in image
  HoughCircles( roi_g, cir, CV_HOUGH_GRADIENT,1 , 5,100*5, 16,9, 100);
  {
      Point center(cvRound(cir[1][0]), cvRound(cir[1][1]));
      lk=center;
      radius = cvRound(cir[1][2]);
      circle( roi, center, radius, Scalar(0,255,0), 3, 8, 0 );
   }
  return 0;
}

int main()
{
 std::vector<std::vector<cv::Point> > conto;
 std::vector<cv::Vec4i> hirer;
 int peri,ve=0;
 cv::Mat approx,ca;
 cv::Size vert;
 string shape;
 double area;
 std::ostringstream shapetext;

//Reading the image shapes.jpg(modify according to requirement)
 cv::Mat image=cv::imread("/home/bxv7657/NASCENT/diced/shaped/shapes.jpg");
 cv::imshow("original", image);
//Detecting the canny edges for isolating edges
 cv::Canny( image, ca, 100, 100*5, 3 );
 Mat drawing = Mat::zeros( ca.size(), CV_8UC3 );

 int erosion_size = 20;  
 Mat element = getStructuringElement(cv::MORPH_RECT,cv::Size(2 * erosion_size + 1, 2 *erosion_size + 1),cv::Point(erosion_size, erosion_size));
 
// Perform erosion or dilation to improve segmentation: To avoid effects of occulusion in real life  
dilate(ca,dst,element);        
erode(dst,ca,element); 
int tot=0;
int rad=0;
Point cent; 
// Finding Contours helps us isolate each shape
cv::findContours( ca,conto, hirer,CV_RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
for(int i=0; i<conto.size(); i++)
{
// Calculate contour area for all detected contour/shapes
  double contArea = cv::contourArea(conto[i]);

//Get bounding rectangle co-ordinates
    cv::Rect Bounco = cv::boundingRect(cv::Mat(conto[i]));
// Crop individual shapes
     peri=cv::arcLength(conto[i],true);
//Applying Douglas-Peucker algorithm for approximating # of  Vertices
     cv::approxPolyDP(conto[i],approx, (0.04 * peri), true);
     vert=(approx.size());
     ve=vert.height;

   if(ve==3)
{

     shape="triangle";

}
 if(ve==4)
{

     shape="Rectangle";

}
 if(ve>4)
{

     shape="Circle";
     rad=countcir(image);
}    peri=cv::arcLength(conto[i],true);
     area=cv::contourArea(approx);
          if(shape=="Circle")
{
          shapetext << ""<<shape<<" Area:"<<area<<" Peri:"<<peri<<""<<"Center "<<lk<<" radius: "<<radius;
}
if(shape!="Circle")
{
shapetext << ""<<shape<<" Area:"<<area<<" and perimeter:"<<peri <<" with Vertices: "<<approx;
}
// Draw the value by each shape on image
        cv::putText(image, shapetext.str(),
        cv::Point(Bounco.x,Bounco.y),
        cv::FONT_HERSHEY_COMPLEX_SMALL,0.6,cv::Scalar::all(0),1,8);
        rectangle(image,Bounco.tl(), Bounco.br(),Scalar(0,255,0),2,8,0);
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, conto, i, color, 2, 8, hirer, 0, Point() );
//For printing in command line
if(shape!="Circle")
{
     cout<<"The area of "<<shape<<" is "<<area<<" \n perimeter:"<<peri <<" with \n Vertices:\n "<<approx<<endl;
     }
if(shape=="Circle")
{
     cout<<"The area of circle is "<<area<<" and \n perimeter:"<<peri <<" with Center:\n "<<lk<<" and \n radius "<<radius<<endl;
     }
//Saving the result image
   cv::imwrite( "/home/bxv7657/NASCENT/diced/shaped/Result.png", image);
  cv::drawContours(image, approx, -1, (255,0,0), 3);
}
  cv::imshow("Original Image", image);
cv::waitKey(0);

}
