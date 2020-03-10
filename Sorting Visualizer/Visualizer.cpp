#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>
#include <thread>
#include <future>
using namespace std;

static bool s_SortFinished = false;
static int s_SortTickTime = 1; // Will be in ms

void QuickSortInitial(int arr[], int low, int high, olc::Pixel colorArray[]);

class SortingVisualizer : public olc::PixelGameEngine
{
public:
	SortingVisualizer()
	{
		sAppName = "Austin Esquirell - Sorting Visualizer";
	}

public:
	struct Point
	{
		Point() {};
		Point(int x, int y) {
			x = this->x;
			y = this->y;
		}

		float x = 0;
		float y = 0;

		//Optional extras
		float w = 0;
		float h = 0;
		float r = 0;
	};

private:
	int nMaxValueAmt = 200;

	int nValueAmt; // Number of random numbers to sort
	int* pValueArray; //Array to hold random values
	olc::Pixel* pColorArray; //Array to hold color of value bars to visualize sorting process

	int nPanAmt = 0;

	Point sliderHandle;
	Point sliderBar;
	bool bMovingSlider = false;
	Point sliderHandle_Time;
	Point sliderBar_Time;
	bool bMovingSlider_Time = false;
	Point sliderHandle_Pan;
	Point sliderBar_Pan;
	bool bMovingSlider_Pan = false;

	float fPrevMouseX = NULL;

	std::thread workerThread;

public:
	bool OnUserCreate() override
	{
		srand(time(0)); // Seed the rng with time
		nValueAmt = nMaxValueAmt; //For now this NEEDS to match max value of values we want to be able to have
		pValueArray = new int[nValueAmt];
		pColorArray = new olc::Pixel[nValueAmt];
		CreateNewArray();

		sliderBar.h = 5;
		sliderBar.w = 500;
		sliderBar.x = ScreenWidth() / 2 - sliderBar.w / 2;
		sliderBar.y = ScreenHeight() / 8;
		sliderHandle.x = sliderBar.x + sliderBar.w;
		sliderHandle.y = sliderBar.y + sliderBar.h / 2;
		sliderHandle.r = 10;

		sliderBar_Time.h = 5;
		sliderBar_Time.w = 500;
		sliderBar_Time.x = ScreenWidth() / 2 - sliderBar_Time.w / 2;
		sliderBar_Time.y = ScreenHeight() / 8 + 40;
		sliderHandle_Time.x = sliderBar_Time.x + sliderBar_Time.w / 2;
		sliderHandle_Time.y = sliderBar_Time.y + sliderBar_Time.h / 2;
		sliderHandle_Time.r = 10;

		sliderBar_Pan.h = 5;
		sliderBar_Pan.w = 500;
		sliderBar_Pan.x = ScreenWidth() / 2 - sliderBar_Pan.w / 2;
		sliderBar_Pan.y = ScreenHeight() - 100;
		sliderHandle_Pan.x = sliderBar_Pan.x;
		sliderHandle_Pan.y = sliderBar_Pan.y + sliderBar_Pan.h / 2;
		sliderHandle_Pan.r = 10;

		return true;
	}

	bool OnUserDestroy() override
	{
		delete[] pValueArray;
		delete[] pColorArray;
		return true;
	}


	bool OnUserUpdate(float fElapsedTime) override
	{
		//Set background color
		Clear(olc::DARK_CYAN);

		//Input
		if (IsFocused())
		{
			/*============== RANDOMIZE BUTTON===================*/
			if (GetMouseX() >= 10 && GetMouseX() <= 160 && GetMouseY() >= 10 && GetMouseY() <= 60)
			{
				FillRect(11, 11, 149, 49, olc::DARK_BLUE);
				if (GetMouse(0).bReleased)
				{
					CreateNewArray(); //Later pass in value for selected array size
				}
			}
			/*==================================================*/

			/*============== SORT BUTTON ===================*/
			if (GetMouseX() >= 200 && GetMouseX() <= 390 && GetMouseY() >= 10 && GetMouseY() <= 60)
			{
				FillRect(201, 11, 189, 49, olc::DARK_GREEN);
				if (GetMouse(0).bReleased)
				{
					workerThread = std::thread(QuickSortInitial, pValueArray, 0, 200 - 1, pColorArray);
					//std::async(std::launch::async, &QuickSortInitial, pValueArray, 0, 200 - 1, pColorArray);
					//QuickSort(pValueArray, 0, 200 - 1, pColorArray);
				}
			}
			/*==============================================*/
			// Pythagorian Theorem to find if mouse is within slider handle radius
			if (sqrt(((GetMouseX() - sliderHandle.x) * (GetMouseX() - sliderHandle.x)) + ((GetMouseY() - sliderHandle.y) * (GetMouseY() - sliderHandle.y))) <= sliderHandle.r)
			{
				if (GetMouse(0).bHeld)
					bMovingSlider = true;
			}

			if (!GetMouse(0).bHeld) // Stop tracking mouse movement to move slider handle
				bMovingSlider = false;

			/*if (GetMouseX() > sliderBar.x + sliderBar.w + sliderHandle.r || GetMouseX() < sliderBar.x - sliderHandle.r)
				bMovingSlider = false;*/

			if (bMovingSlider) // This is here so that mouse does not have to stay within slider handle circle to move it
			{
				//sliderHandle.x += GetMouseX() - fPrevMouseX;
				sliderHandle.x = GetMouseX();

				// Clamp slider handle to the bounds of the slider bar
				if (sliderHandle.x < sliderBar.x)
					sliderHandle.x = sliderBar.x;
				if (sliderHandle.x > (sliderBar.x + sliderBar.w))
					sliderHandle.x = sliderBar.x + sliderBar.w;
			}
			/*=====================================Time Slider==========================================================*/
			// Pythagorian Theorem to find if mouse is within slider handle radius
			if (sqrt(((GetMouseX() - sliderHandle_Time.x) * (GetMouseX() - sliderHandle_Time.x)) + ((GetMouseY() - sliderHandle_Time.y) * (GetMouseY() - sliderHandle_Time.y))) <= sliderHandle_Time.r)
			{
				if (GetMouse(0).bHeld)
					bMovingSlider_Time = true;
			}

			if (!GetMouse(0).bHeld) // Stop tracking mouse movement to move slider handle
				bMovingSlider_Time = false;

			/*if (GetMouseX() > sliderBar.x + sliderBar.w + sliderHandle.r || GetMouseX() < sliderBar.x - sliderHandle.r)
				bMovingSlider = false;*/

			if (bMovingSlider_Time) // This is here so that mouse does not have to stay within slider handle circle to move it
			{
				//sliderHandle.x += GetMouseX() - fPrevMouseX;
				sliderHandle_Time.x = GetMouseX();

				// Clamp slider handle to the bounds of the slider bar
				if (sliderHandle_Time.x < sliderBar_Time.x)
					sliderHandle_Time.x = sliderBar_Time.x;
				if (sliderHandle_Time.x > (sliderBar_Time.x + sliderBar_Time.w))
					sliderHandle_Time.x = sliderBar_Time.x + sliderBar_Time.w;
			}
			/*=================================================Time Slider end=====================================================*/
						/*=====================================Pan Slider==========================================================*/
			// Pythagorian Theorem to find if mouse is within slider handle radius
			if (sqrt(((GetMouseX() - sliderHandle_Pan.x) * (GetMouseX() - sliderHandle_Pan.x)) + ((GetMouseY() - sliderHandle_Pan.y) * (GetMouseY() - sliderHandle_Pan.y))) <= sliderHandle_Pan.r)
			{
				if (GetMouse(0).bHeld)
					bMovingSlider_Pan = true;
			}

			if (!GetMouse(0).bHeld) // Stop tracking mouse movement to move slider handle
				bMovingSlider_Pan = false;

			/*if (GetMouseX() > sliderBar.x + sliderBar.w + sliderHandle.r || GetMouseX() < sliderBar.x - sliderHandle.r)
				bMovingSlider = false;*/

			if (bMovingSlider_Pan) // This is here so that mouse does not have to stay within slider handle circle to move it
			{
				//sliderHandle.x += GetMouseX() - fPrevMouseX;
				sliderHandle_Pan.x = GetMouseX();

				// Clamp slider handle to the bounds of the slider bar
				if (sliderHandle_Pan.x < sliderBar_Pan.x)
					sliderHandle_Pan.x = sliderBar_Pan.x;
				if (sliderHandle_Pan.x > (sliderBar_Pan.x + sliderBar_Pan.w))
					sliderHandle_Pan.x = sliderBar_Pan.x + sliderBar_Pan.w;
			}
			/*=============================================Pan Slider end=====================================================*/
		}

		// Adjust nValueAmt based on slider
		nValueAmt = ((sliderHandle.x - sliderBar.x) / sliderBar.w) * nMaxValueAmt;

		// Adjust sort tick time based on slider2
		s_SortTickTime = ((sliderHandle_Time.x - sliderBar_Time.x) / sliderBar_Time.w) * 50;

		//Adjust pan based on pan slider
		nPanAmt = ((sliderHandle_Pan.x - sliderBar_Pan.x) / sliderBar_Pan.w) * (nMaxValueAmt - nValueAmt);

		// Prevent nValueAmt from being 0
		if (nValueAmt < 1) nValueAmt = 1;

		/*===== Draw to screen =====*/

		// Randomize Values Button
		DrawRect(10, 10, 150, 50, olc::BLUE);
		DrawString(20, 30, "Randomize Values");

		// Run Sorting Algorithm button
		DrawRect(200, 10, 190, 50, olc::GREEN);
		DrawString(210, 30, "Run Sorting Algorithm");

		//Draw Value Bars
		float widthPerBar = (ScreenWidth() * 0.8f) / nValueAmt; // Use 80% of screen width

		for (int x = 0; x < nValueAmt; x++)
		{
			if (nValueAmt + nPanAmt > nMaxValueAmt) nPanAmt = nMaxValueAmt - nValueAmt;

			// Math to center the bars on screen, no matter the number of bars or screen size
			FillRect((ScreenWidth() * 0.1f) + (widthPerBar * (1.0f / 6)) + (x * widthPerBar), 200, widthPerBar * (2.0f / 3), (pValueArray[x + nPanAmt] / 1000.0f) * 200.0f, pColorArray[x + nPanAmt]);
		}

		//// Draw Slider Bars and Handles
		FillRect(sliderBar.x, sliderBar.y, sliderBar.w, sliderBar.h, olc::RED);
		FillCircle(sliderHandle.x, sliderHandle.y, sliderHandle.r);

		FillRect(sliderBar_Time.x, sliderBar_Time.y, sliderBar_Time.w, sliderBar_Time.h, olc::MAGENTA);
		FillCircle(sliderHandle_Time.x, sliderHandle_Time.y, sliderHandle_Time.r);

		FillRect(sliderBar_Pan.x, sliderBar_Pan.y, sliderBar_Pan.w, sliderBar_Pan.h, olc::DARK_GREEN);
		FillCircle(sliderHandle_Pan.x, sliderHandle_Pan.y, sliderHandle_Pan.r);

		////Draw nValueAmt
		DrawString(sliderHandle.x - 7, sliderHandle.y - sliderHandle.r - 10, to_string(nValueAmt));

		//Draw s_SortTickTime
		DrawString(sliderHandle_Time.x - 7, sliderHandle_Time.y - sliderHandle_Time.r - 10, to_string(s_SortTickTime) + "ms");

		//Draw <--  --> for pan slider
		DrawString(sliderHandle_Pan.x - 31, sliderHandle_Pan.y + sliderHandle_Time.r + 10, "<------>");
		DrawString(sliderHandle_Pan.x - 7, sliderHandle_Pan.y - sliderHandle_Pan.r - 10, to_string(nPanAmt));

		
		fPrevMouseX = GetMouseX(); //For tracking last frame's mouse movement

		// Check if sort has finished, and join the threads if ready
		if (s_SortFinished)
		{
			if (workerThread.joinable())
			{
				cout << "\nThread is joinable!!!!\n";
				workerThread.join();
				s_SortFinished = false;
			}
			else
			{
				cout << "\nThread NOT Joinable!!\n";
				s_SortFinished = false;
			}
		}

		return true;
	} //OnUserUpdate

	void CreateNewArray()
	{
		for (int x = 0; x < nMaxValueAmt; x++) //Used to be nValueAmt to only randomize a certain number of values in the array, for sorting smaller data sets.
		{
			//nValues[x] = 5 + rand() / (RAND_MAX / (1000 - 5 + 1) + 1);
			pValueArray[x] = rand() % (995 + 1) + 5;
			cout << pValueArray[x] << endl;

			pColorArray[x] = olc::WHITE;
		}
	}


	void swap(int& a, int& b)
	{
		int t = a;
		a = b;
		b = t;
	}

};

/*==================== QUICK SORT ====================*/
int Partition(int arr[], int low, int high, olc::Pixel cArr[]) // We could just reference pColorArray directly, but put it as function argument for clarity of its dependencies.
{
	// Using last array element as pivot
	int pivot = arr[high];

	// Initialize smaller element index (i) to -1
	int i = low - 1;

	for (int j = low; j < high; j++)
	{
		using namespace std::literals::chrono_literals;

		// Make value we're comparing at a different color
		cArr[j] = olc::MAGENTA;

		// Optional brief pause to give user more time to see what is happening
		std::this_thread::sleep_for(std::chrono::milliseconds(s_SortTickTime));

		if (arr[j] < pivot)
		{
			i++; // Increment smaller element index
			cout << "\nSwap " << arr[i] << " and " << arr[j] << endl;
			swap(arr[i], arr[j]);
		}
		cArr[j] = olc::WHITE;
	}

	cout << "\nSwap " << arr[i + 1] << " and " << arr[high] << "   (Pivot)" << endl;
	swap(arr[i + 1], arr[high]); // Put pivot element in its place
	cArr[i + 1] = olc::GREEN; //Pivot is in correct place, make it green

	return (i + 1); // Return index of pivot position
}

void QuickSort(int arr[], int low, int high, olc::Pixel colorArray[])
{
	if (low < high) // Base Case check
	{
		// Start with entire array
		int partitionIndex = Partition(arr, low, high, colorArray);

		// First pivot now in its place in array, continue with rest of elements on either side of pivot
		QuickSort(arr, low, partitionIndex - 1, colorArray);
		QuickSort(arr, partitionIndex + 1, high, colorArray);
	}
	else // Base case reached, meaning there are less than 2 values to sort in this set (0 or 1)
	{	// In the case that there is one value, set it to green because it will already be in its place. If there are no values then this will already be green, so setting again won't matter
		colorArray[low] = olc::GREEN;
	}
}

void QuickSortInitial(int arr[], int low, int high, olc::Pixel colorArray[])
{
	if (low < high) // Base Case check
	{
		// Start with entire array
		int partitionIndex = Partition(arr, low, high, colorArray);

		// First pivot now in its place in array, continue with rest of elements on either side of pivot
		QuickSort(arr, low, partitionIndex - 1, colorArray);
		QuickSort(arr, partitionIndex + 1, high, colorArray);
	}
	s_SortFinished = true;
}
/*============================================================================================================*/


int main()
{
	SortingVisualizer sv;
	if (sv.Construct(1280, 720, 1, 1))
		sv.Start();

	return 0;
}