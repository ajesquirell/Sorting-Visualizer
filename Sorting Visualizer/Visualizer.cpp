#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>
using namespace std;


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
	int nValueAmt; // Number of random numbers to sort
	int* pValueArray; //Array to hold random values

	Point sliderHandle;
	Point sliderBar;

	float fPrevMouseX = NULL;
	bool bMovingSlider = false;


public:
	bool OnUserCreate() override
	{
		srand(time(0)); // Seed the rng with time
		nValueAmt = 100; //For now this NEEDS to match max value of values we want to be able to have
		pValueArray = new int[nValueAmt];
		CreateNewArray();

		sliderBar.h = 5;
		sliderBar.w = 500;
		sliderBar.x = ScreenWidth() / 2 - sliderBar.w / 2;
		sliderBar.y = ScreenHeight() / 8;
		sliderHandle.x = sliderBar.x + sliderBar.w / 2;
		sliderHandle.y = sliderBar.y + sliderBar.h / 2;
		sliderHandle.r = 10;

		return true;
	}

	bool OnUserDestroy() override
	{
		delete[] pValueArray;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//Set background color
		Clear(olc::DARK_CYAN);




		//Input
		if (IsFocused())
		{
			if (GetMouseX() >= 10 && GetMouseX() <= 160 && GetMouseY() >= 10 && GetMouseY() <= 60)
			{
				FillRect(11, 11, 149, 49, olc::DARK_BLUE);
				if (GetMouse(0).bReleased)
				{
					CreateNewArray(); //Later pass in value for selected array size
				}
			}



			// Pythagorian Theorem to find if mouse is within slider handle radius
			if (sqrt(((GetMouseX() - sliderHandle.x) * (GetMouseX() - sliderHandle.x)) + ((GetMouseY() - sliderHandle.y) * (GetMouseY() - sliderHandle.y))) <= sliderHandle.r)
			{
				if (GetMouse(0).bHeld)
					bMovingSlider = true;
			}

			if (!GetMouse(0).bHeld) // Stop tracking mouse movement to move slider handle
				bMovingSlider = false;

			if (bMovingSlider) // This is here so that mouse does not have to stay within slider handle circle to move it
			{
				sliderHandle.x += GetMouseX() - fPrevMouseX;
				//sliderHandle.x = GetMouseX();

				// Clamp slider handle to the bounds of the slider bar
				if (sliderHandle.x < sliderBar.x)
					sliderHandle.x = sliderBar.x;
				if (sliderHandle.x > (sliderBar.x + sliderBar.w))
					sliderHandle.x = sliderBar.x + sliderBar.w;
			}
		}

		// Adjust nValueAmt based on slider
		nValueAmt = ((sliderHandle.x - sliderBar.x) / sliderBar.w) * 100; //100 is max number of values we are allowing

		// Prevent nValueAmt from being 0
		if (nValueAmt < 1) nValueAmt = 1;

		/*===== Draw to screen =====*/

		// New array Button
		DrawRect(10, 10, 150, 50, olc::BLUE);
		DrawString(20, 30, "Create New Array");

		// Draw Bars
		float widthPerBar = (ScreenWidth() * 0.8f) / nValueAmt; // Use 80% of screen width

		for (int x = 0; x < nValueAmt; x++)
		{
			// Math to center the bars on screen, no matter the number of bars or screen size
			FillRect((ScreenWidth() * 0.1f) + (widthPerBar * (1.0f / 6)) + (x * widthPerBar), 200, widthPerBar * (2.0f / 3), (pValueArray[x] / 1000.0f) * 200.0f);
		}

		// Draw Slider Bar and Handle
		FillRect(sliderBar.x, sliderBar.y, sliderBar.w, sliderBar.h, olc::RED);
		FillCircle(sliderHandle.x, sliderHandle.y, sliderHandle.r);

		//TESTING - Draw nValueAmt
		DrawString(sliderHandle.x - 7, sliderHandle.y - sliderHandle.r - 10, to_string(nValueAmt));
		
		fPrevMouseX = GetMouseX(); //For tracking last frame's mouse movement

		return true;
	} //OnUserUpdate

	void CreateNewArray()
	{
		for (int x = 0; x < nValueAmt; x++)
		{
			//nValues[x] = 5 + rand() / (RAND_MAX / (1000 - 5 + 1) + 1);
			pValueArray[x] = rand() % (995 + 1) + 5;
			cout << pValueArray[x] << endl;
		}
	}
};

int main()
{
	SortingVisualizer sv;
	if (sv.Construct(1280, 720, 1, 1))
		sv.Start();

	return 0;
}