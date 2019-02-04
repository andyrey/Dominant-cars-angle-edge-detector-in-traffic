/*******.******=***.**.***********************************1****
By Zakharoff Andrey
This program accumulates traffic info and find statistically dominating
angle of front cars' edges. It can be used for normalizing front view.
Restriction:
1) Accuracy is not high;
2) Only stright line traffic can be measured
3) Angles from -45 to +45 degrees.
4) Some time (or cars passed) should be waited to get statistically approved result
********************************************************************/
#include "my_funcs.h" 
using namespace std;
using namespace cv;

int frame_count;

int main(int argc, char** argv)
{
	char* filename;
	VideoCapture vicapture;
	if (argc >= 2)
		filename = argv[1];
	else
	{
		cout << "Please, put the target videofile!" << endl;
		return -1;
	}
	bool is_opened = vicapture.open(filename);
	if (!is_opened)
	{
		cout << filename << " isn't opened!!" << endl;
		return -2;
	}
	frame_count = 0;
	Mat frame1, frame2, frame_diff;
	float alpha[] = { 45.f, 26.f, 18.4f, 14.f, 11.3f,8.13f, 5.2f, 0.0f, -5.2f, -8.13f, -
		11.3f, -14.f, -18.4f, -26.f,-45.f };
	float angle_current = 0.f, angle_mean = -1.f;

	int valu[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int valu_prev[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int valu_diff[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	long int frame_count = 0;

	vicapture >> frame1;
	frame2 = frame1;
	Size smaller_size;//=frame2.size(); 
	smaller_size.width = frame1.cols*0.4;
	smaller_size.height = frame1.rows*0.4;

	const int KEY_SPACE = 32;
	const int KEY_ESC = 27;
	int Y_line = 300;
	int key = 0;

	vector<int> vals;
	vals.reserve(smaller_size.width);
	int vals_spike[LINESET];//responses measured framely for 7 different angles

	do
	{
		vicapture >> frame1;
		if (frame1.empty() || frame2.empty())
			break;
		resize(frame1, frame1, smaller_size);
		resize(frame2, frame2, smaller_size);
		cvtColor(frame1, frame1, CV_BGR2GRAY);
		if (!frame_count)//bcs frame2=framel (gray) at the end of this loop
			cvtColor(frame2, frame2, CV_BGR2GRAY);
		///frame_diff=framel;
		absdiff(frame1, frame2, frame_diff);
		//int erosion_size=3;
		threshold(frame_diff, frame_diff, 50, 255,
			CV_THRESH_BINARY);//CV_THRESH_OTSU+ 
		//scanLine(frame_diff, vals, Y_line, 0, frame_diff.cols, 30);
		Mat	frame_diff2 = Mat::zeros(frame_diff.rows, frame_diff.cols, frame_diff.type());
		//draw lines to see when car intersect it 
		line(frame2, Point(0, Y_line), Point(frame2.cols, Y_line), Scalar(255, 255, 0), 1);

		//right-top to left-bottom lines valu[id]
		valu[0] = scanSpecialLine(frame_diff, 1);//45degree 
		valu[1] = scanSpecialLine(frame_diff, 2);//26degree 
		valu[2] = scanSpecialLine(frame_diff, 3);//18.4degree 
		valu[3] = scanSpecialLine(frame_diff, 4);//14 
		valu[4] = scanSpecialLine(frame_diff, 5);//11.3
		valu[5] = scanSpecialLine(frame_diff, 7);//8.13 
		valu[6] = scanSpecialLine(frame_diff, 11);//5.2
		valu[7] = scanSpecialLine(frame_diff, frame_diff.rows);//0.0 
	  //left-top to right-bottom lines
		valu[8] = scanSpecialLine(frame_diff, -11);//-5.2degree
		valu[9] = scanSpecialLine(frame_diff, -7);//-8.13 
		valu[10] = scanSpecialLine(frame_diff, -5);//-11.3degree 
		valu[11] = scanSpecialLine(frame_diff, -4);//-14degree 
		valu[12] = scanSpecialLine(frame_diff, -3);//-18.4
		valu[13] = scanSpecialLine(frame_diff, -2);//-26 
		valu[14] = scanSpecialLine(frame_diff, -1);//-45

		for (int i = 0; i < LINESET; ++i)
			valu_diff[i] = abs(valu[i] - valu_prev[i]);

		Mat plot = Mat::zeros(HIGHT_OSC_PLOT, 700, CV_8UC3);

		float ratio_of_active_mode;
		int mostActive_id = whichLineDisturbedMost2(valu_diff,
			LINESET, ratio_of_active_mode);//keeps static stack inside 
		drawValues(plot, valu, LINESET, mostActive_id);
		//showIntGraph("Line graph", &vals[0], vals.size(), 1);//,0 
		if (mostActive_id > -1)
		{
			if (angle_mean == -1)//activation of mean value 
				angle_mean = alpha[mostActive_id];

			angle_current = alpha[mostActive_id];
			// angle_mean = ((1.f -ratio_of_active_mode)*angle_mean*frame_count + ratio_of_active_mode*angle_current) / (1.f + frame_count);
			//angle_mean=(angle_mean*frame_count+angle_current)/(1.f+frame_count); 
			angle_mean = 0.99*angle_mean + 0.01*angle_current;
			std::cout << std::fixed;
			std::cout << std::setprecision(2);
			cout << "\n\n\n" << "in frame " << frame_count << '\n' << "angle mean =" << angle_mean << "\n\n";
		}

		vals.clear();

		imshow("frame_diff", frame_diff);
		imshow("road", frame2);
		key = cvWaitKey(10);
		if (key == KEY_SPACE)
			key = cvWaitKey(0);

		if (key == KEY_ESC || key == 'q' || key == 'Q')
			break;
		frame2 = frame1;
		++frame_count;
		for (int i = 0; i < LINESET; ++i)
			valu_prev[i] = valu[i];
	} while (1);

	return 0;
}
