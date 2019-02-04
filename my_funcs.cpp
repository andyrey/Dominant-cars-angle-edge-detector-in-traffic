#include "my_funcs.h"

int whichLineDisturbedMost2(const int values[], const int size_of_values_array, float&
	ratio)
{//size_of_values_array==13
 //values[] - array of responses in this frame
 //return most often in stack id most_probable_id
 //ratio is ratio of value[most_probable_id] in this frame / maximal in this frame 
 //	-not alwais == 1, may be less
	static int stable_most_probable_id(6);
	static std::deque<int> m_deq_id(STACKDEPTH, -1);//deque for median extracting id 
													//(== 0, 1, 2, 3, 4) aanonHmm BHaHane "-1"
	std::deque<int>::iterator it_deq_id;
	int maxid = 0;//id of object with maxval
	int maxval = 0;//maximal value for one of objects in this frame 
	int max_ocurr = 0;//maximal amount of some object in stack
	int count;
	int most_probable_id = -1;//id of object which has max_ocurr 
	int id = 1;
	//if(identity>maxid) maxid=identity;
	//if(maxid>100) maxid=100;

	for (int i = 0; i < size_of_values_array; ++i)
	{//refill the stack
		if (values[i] > maxval)
		{
			maxval = values[i];
			maxid = i;
		}
	}

	if (maxval > 5000)//or do not chan
	{
		m_deq_id.push_front(maxid);
		m_deq_id.pop_back();
	}
	//for(int j=maxid;j>=-1;--j)//ioiaaa Ii anal n(506h65 id
	for (int j = size_of_values_array; j >= -1; --j)
	{//cont all possible id
		count = 0;

		for (it_deq_id = m_deq_id.begin(); it_deq_id != m_deq_id.end(); ++it_deq_id)//run 
																					//all deq elemets 1e516j65 11 anai yealaini hiehea 10
		{
			if (*it_deq_id == j)
			{
				++count;
				id = *it_deq_id;
			}

		}
		if (count > max_ocurr)
		{
			max_ocurr = count;
			most_probable_id = id;//-(--it_deq_id);
		}
	}
	//stable_most_probable_id=(int)(stable_most_probable_id*0.8+ most_probable_id*0.2); 
	cout << "current values" << values[0] << '\t' << values[1] << '\t' << values[2] << '\t' << values[3] << '\t'
		<< values[4] << '\t' << values[5] << '\t' << values[6] << endl;
	cout << "maxval " << maxval << " id= " << maxid << endl;

	cout << "most often id so far ";
	for (int tt = 1; tt <= id; ++tt)
		cout << '\t';
	cout << most_probable_id << " have stored " << max_ocurr << " times" << endl;
	cout << "maximal valued id's in stack: " << endl;
	for (it_deq_id = m_deq_id.begin(); it_deq_id != m_deq_id.end(); ++it_deq_id)
		cout << *it_deq_id << " ";
	cout << endl;
	//getchar();
	//double probability= (double)max_ocurr*100./deq_size;//hoa6 
	//Wiyoiihou a ioloaioa6
	ratio = (maxval > 0) ? ((float)values[most_probable_id] / maxval) : 0.f;
	return most_probable_id;
}

void drawValues(Mat &plot, const int values[], const int number, const int active_id)
{//draw every frame in image mat "plot", bars with height "values", in number of bars 
 //	"number", selecting most active line with red
	cv::Scalar clrs[] = { Scalar(100,200,20), Scalar(150,5,200), Scalar(200,250,30),
		Scalar(100,5,180), Scalar(200,50,230),

		Scalar(220,250,20),Scalar(255,10,250),Scalar(100,200,20),Scalar(150,5,200),Scalar(200,250
			,30),Scalar(100,5,180),

		Scalar(200,50,230),Scalar(220,250,20),Scalar(100,200,20),Scalar(150,5,200) };
	static int frame_count_1(0);
	int imax, maxval = 0;
	int* length = new int[number];

	for (int i = 0; i < number; ++i)
	{
		length[i] = values[i] / 100;//((float)maxval)*100;

		cv::line(plot, Point(20 + 40 * i, HIGHT_OSC_PLOT + 10 - length[i]), Point(20 +
			40 * i, HIGHT_OSC_PLOT + 10), clrs[i], 7);//oscill bars for each mode
		if (i == active_id)//&& frame_count_1>STACKDEPTH
			cv::line(plot, Point(20 + 40 * i, HIGHT_OSC_PLOT + 10 - length[i]), Point(20 +
				40 * i, HIGHT_OSC_PLOT + 10), Scalar(0, 0, 255), 14);//most frequent mode
	}
	imshow("Bars", plot);
	delete[] length;
	++frame_count_1;
}


int scanSpecialLine(const Mat& mat, const int m) //,vector<int> &vvals
{//forms special stright line with 1 pixel thin, the number of pixels= mat.rows for any 
 //angle from 0 till 45 degree
 //for the first just draw these lines
 //m=1 --45 degree,     m=5 -- 11.3 degree;    m=H --0.0 degree
 //m      alpha, degree
 // 1        45
 // 2        26.5
 // 3        18.4
 // 4        14.04
 // 5        11.3
 // 7        8.13
 //10        5.7
 //11        5.2
 //12        4.76
 ///....
 //H        0.0

	int W = mat.cols;
	int H = mat.rows;
	int d = ceil((float)H / abs(m));//number of stairs-vertical span of the line
	int h0 = (H - d) / 2;//upper Y- position of the line//+1
	int j = 1;//h0;//row count from here
			  //int j=0;
	int sumvalue = 0;
	uchar *p;
	int shift;
	int ms = mat.step;
	int mes = mat.elemSize();
	int sizeInBytes = mat.step[0] * mat.rows;
	//int sib=sizeof(mat.data);==4
	int w1 = (W - H) / 2;// left (and right) margine
	int in_line_shift;
	int x, y; //coordinates for check only 
	unsigned long long count = 0;
	do
	{
		if (m > 0)//right-top to left-bottom lines
			in_line_shift = W - w1 - j*m;//HaHano cTyneHbkm Ha cTpoke 
		else//left-top to right-bottom lines
			in_line_shift = w1 - j*m;

		shift = (j + h0)*mat.step + (in_line_shift)*mat.elemSize();//bytes h0-1 
		if (shift >= sizeInBytes - sizeof(mat.data)) break;
		//shift=(j+h0)*mat.step + (line_shift)*mat.elemSize();//bytes
		for (int i = 0; i < abs(m); ++i)//oTpvicoeKa m TOHeK B oilHo0 cTpoKe 143o6pax(eHms
		{
			p = mat.data + shift + i*mat.elemSize();
			uchar val = *p;

			//3Aecb 3amepilTb OTKJH'lK 0 cymmopoBaTb
			if (mat.channels() == 1)
			{
				sumvalue += static_cast<int>(val);
				*p = 255;//to draw-see the built line, but it distort the gauge	indication!!
			}
			/*else if(mat.channels()==3)
			*(p++)=0;//blue
			*(p++)=0;//green
			*p=255;
			}*/
			++count;
		}
		++j;
	} while (count < H);

	return sumvalue;

}
void scanLine(const Mat& mat, vector<int> &vvals, int row, int leftcol, int rightcol, int
	thickness)
{//from image mat collect values ovals on the row from leftcol till rightcol with (line 
 // is horizontal)
	int value, i, j;
	vvals.resize(rightcol - leftcol, 0);
	vector<int> vvals2(rightcol - leftcol, 0);
	//calcs shift from data begginning on original image to move pointer into position 
	// of given coord point
	if (leftcol < 0) leftcol = 0;
	if (rightcol > mat.cols - 1) rightcol = mat.cols - 1;
	if (row > mat.rows - 1)      row = mat.rows - 1;
	//int elsz=mat.elemSize(),//3 for bgr i for gray
	//    matstep=mat.step;//2880 for bgr 960 for gray 
	for (int j = row; j < row + thickness; ++j)
	{
		for (i = leftcol; i < rightcol; ++i)
		{
			int shift = i*mat.elemSize() + j*mat.step;//shift from data begin
													  //measured in bytes
			int ch = mat.channels();
			//Point3_<uchar> *rgbtriple;
			uchar *val, b, g, r;
			val = ((uchar*)mat.data + shift);//
			if (ch == 1)
			{//value= static_cast<int>(mat.at<uchar>(row,i));
				value = static_cast<int>(*val);
			}
			else
				if (ch == 3)
				{
					b = *val;
					val++;
					g = *val; val++;
					r = *val;
					value = static_cast<int>((r + g + b) / 3.);
				}
			//vvals.push_back(value);
			vvals2[i - leftcol] = (int)((vvals2[i - leftcol] * (j -
				row) + value) / (j - row + 1.0f));
		}//for
	}//for j

	for (int i = 0; i < vvals.size(); ++i)
		vvals[i] = (vvals2[i] < 50) ? 0 : vvals2[i];
	//vvals[i]=vvals2[i];
	return;
}