
//CameraIN Headers
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include "D8MCapture.h"
#include "hps_0.h"
//------------------------------

//Server Headers
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <fstream>
#include <iostream>
#define PORT 8080 

//
//#pragma comment (lib, "Ws2_32.lib")
//---------------------------

using namespace cv;
using namespace std;

#ifndef CAPTURE_RAM_DEVICE
#define CAPTURE_RAM_DEVICE "/dev/f2h-dma-memory"
#endif /* ifndef CAPTURE_RAM_DEVICE */

int main()
{
//Camera In Initialization
    Mat src;
    D8MCapture *cap = new D8MCapture(TV_DECODER_TERASIC_STREAM_CAPTURE_BASE, CAPTURE_RAM_DEVICE);
    std::cout<<"attempting to open camera"<<endl;
	if (!cap->isOpened()) {
        return -1;
    }
	cout<<"open successful"<<endl;

    char str[100];
    static struct timeval last_time;
    struct timeval current_time;
    static float last_fps;
    float t;
    float fps;

    namedWindow("camera");
//---------------------
	int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; //buffer size
    //char *hello = "Hello from server"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; //perhaps i can just declare the ip on the qt side
    address.sin_port = htons( PORT ); //uses the port

    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
	cout<<"bound to port"<<PORT<<"and to IP"<<address.sin_addr.s_addr<<endl;
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
	cout<<"listened"<<endl;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
	cout<<"accepted connection"<<endl;
    
	
	int reading = 0; //flag

	ofstream file1;//file1 = open("image1.bmp", "wb") 	//getfile1
	file1.open("image1.bmp",ios::binary);

	int file1obtained = 0;	//flag

	int br = 0;
	int con = 0;

	int bytes_recieved = 0;	//counter to keep track of all of the data
	
	while (1)
	{
		memset(&buffer, 0, sizeof(buffer));//
		valread = read( new_socket , buffer, 1024);//data, addr = client_socket.recvfrom(BUFFER_SIZE)
		file1.write(buffer, 1024);//file1.write(data) //write to file
		bytes_recieved += sizeof(buffer);
		std::cout<<bytes_recieved<<std::endl;
		if (reading == 0)
			{
				std::cout<<"reading data"<<std::endl;
				reading = 1;
			}
		//cout<<buffer;
		if (strcmp(buffer, "3") == 0)
			{
				std::cout<<"end reached"<<std::endl;
				std::cout<<"original image obtained, press any key to continue"<<std::endl;
				file1obtained = 1;
					//grr = cv2.waitKey(0) used to continue
				file1.close();//file1.close(); //close the file
				break;
			}
	}	
	//std::cout<<"exited receiving loop"<<std::endl;
		
//------------------------------------	
	Mat overlay = imread("/root/oldopen/camera_intcptest/image1.bmp", -1);
	resize(overlay,overlay,Size(99,99));
	cvtColor(overlay, overlay, CV_GRAY2RGB);
	//Rect r=Rect(10,20,40,60);
	
	
	

	Mat gaming,dst;
//--CameraIN loop-----------------
    while (1) {
        if (!cap->read(src))
            return -1;
		memset(&buffer, 0, sizeof(buffer));//
        
		/*
        gettimeofday(&current_time, NULL);
        t = (current_time.tv_sec - last_time.tv_sec)
                + (current_time.tv_usec - last_time.tv_usec) / 1000000.;
        fps = 1. / t;
        fps = last_fps * 0.8 + fps * 0.2;
        last_fps = fps;
        last_time = current_time;
        sprintf(str, "%2.2f, %2.6f", fps, t);
        putText(src, str, Point(20, 40), FONT_HERSHEY_DUPLEX, 1,
                Scalar(0, 255, 0));*/

		valread = read( new_socket , buffer, 3);//data, addr = client_socket.recvfrom(3)
		

		br = int(buffer[0]);
		con =int(buffer[1]);
		cout<<"brightness="<<br-48<<"  con="<< con-48<<endl;
		//convertScaleAbs(src, gaming, br, con);

		src.convertTo(gaming, -1, con-48, (br-48)*10);
		imwrite("source.jpg", gaming);
		//cvtColor(gaming,gaming,COLOR_GRAY2BGR);
		Mat srcImg = imread("source.jpg",-1);

		if (overlay.data){
		overlay.copyTo(srcImg(cv::Rect(0, 0, overlay.cols, overlay.rows)));
		}
		

		//cout<<"stuck on image display"<<endl;
		//cin>>br;
        imshow("camera", srcImg);
		//imshow("newframe", dst);
		//cin>>br;

        //-- bail out if escape was pressed
        int c = waitKey(10);
        if ((char) c == 27) {
            con = 0;//break;
        }
    }
    delete cap;
    destroyAllWindows();
	//------------------------------------------
	
	
    return 0;
}
