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

private:
	int nValueAmt; // Number of random numbers to sort
	//int valueArray[50]; //Array to hold random values
	int* valueArray; //Array to hold random values

	//DO I WANT TO USE DYNAMIC ARRAY OR JUST USE NORMAL AT 100 AND ONLY USE FIRST 4 OR HOW EVER MANY WE SELECT, THEN WE COULD HAVE A SLIDER THAT ADJUSTS ON THE FLY AND ALREADY HAS NUMBERS POPULATED IN THE ARRAY????

public:
	bool OnUserCreate() override
	{
		nValueAmt = 50;
		valueArray = new int[nValueAmt];
		cout << valueArray << endl << valueArray[0] << "sizeof(vA): " << sizeof(&valueArray) << endl << "sizeof(valueArray[0]): " << sizeof(valueArray[0]) << endl;
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
				DrawString(400, 400, "Does it work?");
				if (GetMouse(0).bReleased)
				{
					CreateNewArray(); //Later pass in value for selected array size
				}
			}
		}


		/*===== Draw to screen =====*/

		//New array Button
		DrawRect(10, 10, 150, 50, olc::BLUE);
		DrawString(20, 30, "Create New Array");

		//Length Bars
		int widthPerBar = ((ScreenWidth() * 0.8f) / 100.0f); //Where 100 is currently hard coded to number of numbers to sort

		for (int x = 0; x < nValueAmt; x++)
		{
			FillRect(ScreenWidth() * 0.1f + (x * widthPerBar), 200, widthPerBar * (2.0f / 3), (valueArray[x] / 1000.0f) * 200.0f);
		}
		

		return true;
	} //OnUserUpdate

	void CreateNewArray()
	{
		srand(time(0));
		for (int x = 0; x < sizeof(valueArray) / sizeof(valueArray[0]); x++)
		{
			//nValues[x] = 5 + rand() / (RAND_MAX / (1000 - 5 + 1) + 1);
			valueArray[x] = rand() % (995 + 1) + 5;
			cout << valueArray[x] << endl;
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