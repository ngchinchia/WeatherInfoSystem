#include <iomanip>
#include <string>
#include <fstream>
#include <stdlib.h> 
#include <iostream>

using namespace std;

void mainMenu();
void readConfigFile(string filename, int & xIdex, int & yIdex, string AccessFiles[3], int**& idGrid, string**& cityGrid, int** & cloudGrid, int** & pressureGrid);
void readCityMapFile(string filename, int**& idGrid,string**& cityGrid, int yIdex, int xIdex);
void readCloudsPressureFile(string filename, int **& grid, int yIdex, int xIdex);
void displayGrid(int** display, int cols , int rows);
void displayCloudsPressureIndex(int** display, int cols, int rows);
void displayGrid(string ** display, int cols, int rows);
void displayCloudsPressureLmh(int** display, int cols, int rows);
double calculateAverage(string ** cityGrid, int ** numGrid, int cols, int rows,string val);
void generateReport(string ** cityGrid, int ** cloudGrid, int ** pressureGrid, int ** idsGrid, int cols, int rows);

//Functions to check if line is empty
bool isBlankLine(char const* line)
{
    for ( char const* empty = line; *empty; ++empty )
    {
        if ( !isspace(*empty) ) return false;
    }
    return true;
}

bool isBlankLine(string const& line)
{
   return isBlankLine(line.c_str());
}

//Function to read CONFIG FILE
void readConfigFile(string filename, int & xIdex, int & yIdex, string AccessFiles[3], int**& idGrid, string**& cityGrid, int** & cloudGrid, int** & pressureGrid)
{
	//variables declaration
	ifstream file(filename.c_str());
	string line;
	int counter = 1;
	int arrayIndex = 0;
	int count = 0;

	
	//If file is open do the following conditions
	if (file.is_open())	
	{
		
		
		while (!file.eof()) //If it is not the end of file do the following conditions
		{
			
			getline(file, line); //Getting line by line of text file and store it in a string variable

			if ((!isBlankLine(line)) && (line[0] != '/') )//Checks for 2 conditions: 1) Not an empty line && 2) Not a commented line
			{
				
				/***Getting GridXIdxMax value***/
				if (counter == 1)
				{        
            		int rangeX1=atoi(line.substr(line.find('=')+1).c_str());
            		int rangeX2=atoi(line.substr(line.find('-')+1).c_str());
            		xIdex= atoi(line.substr(line.find('-')+1).c_str())+1;
            		counter++;
            		cout<<"\n"<<"Reading in GridX_idxRange: " <<rangeX1<<"-"<<rangeX2<<" ... done!"<<endl;
				}
				/***Getting GridYIdxMax value***/
				else if (counter == 2)
				{
					 int rangeY1=atoi(line.substr(line.find('=')+1).c_str());
           			 int rangeY2=atoi(line.substr(line.find('-')+1).c_str());
           			 yIdex= atoi(line.substr(line.find('-')+1).c_str())+1;
           			 counter++;
                     cout<<"Reading in GridY_idxRange: " <<rangeY1<<"-"<<rangeY2<<" ... done!"<<"\n"<<endl;
                     cout<<"Storing data from input file : " <<endl;
         	         arrayIndex = 0;
				}
				/***Getting filenames***/
				else
				{
					line = line.substr(0,line.size());
					AccessFiles[arrayIndex] = line;
					arrayIndex++;
				}
				
			}
		}
		file.close();
	}
	else
	{
		cout << "Error: File could not be opened " <<" " <<filename<< endl;
	}
	readCityMapFile(AccessFiles[0], idGrid, cityGrid, yIdex, xIdex);
	cout << AccessFiles[0] << "... done!" << endl;
	readCloudsPressureFile(AccessFiles[1], cloudGrid, yIdex, xIdex);
	cout << AccessFiles[1] << "... done!" << endl;
	readCloudsPressureFile(AccessFiles[2], pressureGrid, yIdex, xIdex);
	cout << AccessFiles[2] << "... done!" << endl;
	cout << endl << "All records successfully stored. Going back to main menu ..." << endl << endl;
}

//Function to read CITYLOCATION FILE
void readCityMapFile(string filename, int**& idGrid,string**& cityGrid, int yIdex, int xIdex)
{
	//Declare variables
	ifstream file(filename.c_str());
	string line;
	int x=0,y=0;
	
	//Dynamic allocation of grids
	idGrid = new int*[yIdex];
	cityGrid = new string*[yIdex];
	for (int i = 0; i < yIdex; i++)
	{
		idGrid[i] = new int[xIdex];
		cityGrid[i] = new string[xIdex];

		/*initializing the grid*/
		for (int j = 0; j < xIdex; j++)
		{
			idGrid[i][j] = -1;
			cityGrid[i][j] = "";
		}
	}

	if (file.is_open())
	{
		while(!file.eof())
		{
			
			getline(file, line);
			

			if (!isBlankLine(line))
			{
				
				x = atoi(line.substr(1, line.find(',') - 1).c_str());

				
				line = line.substr(line.find(',') + 2);

				
				y = atoi(line.substr(0, line.find(']')).c_str());

				
				line = line.substr(line.find(']') + 2);

				
				idGrid[y][x] = atoi(line.substr(0, line.find('-')).c_str());

				
				line = line.substr(line.find('-') + 1);

				
				cityGrid[y][x] = line;

				line = "";
			}
		}
		file.close();
	}
	else
	{
		cout << "Error:File could not be opened:" <<" "<< filename << endl;
	}
}

//Function to read cloud pressure file
void readCloudsPressureFile(string filename, int **& grid, int yIdex, int xIdex)
{
	//variable declaration
	ifstream file(filename.c_str());
	string line;
	int x = 0, y = 0;
	
	grid = new int*[yIdex];
	for (int i = 0; i < yIdex; i++)
	{
		grid[i] = new int[xIdex];

		//init grid
		for (int j = 0; j < xIdex; j++)
		{
			grid[i][j] = -1;
		}
	}

	if (file.is_open())
	{
		
		while (!file.eof()) //While it isn't the end of file
		{
			
			getline(file, line);

			if (line != "")
			{
				//Extract x coordinate
				x = atoi(line.substr(1, line.find(',') - 1).c_str());

				
				line = line.substr(line.find(',') + 2);

				//Extract y coordinate
				y = atoi(line.substr(0, line.find(']')).c_str());

				
				line = line.substr(line.find(']') + 2);

				//Extract pressure/cloud cover
				grid[y][x] = atoi(line.c_str());

				line = "";
			}
		}
		file.close();
	}
	else
	{
		cout << "Could not open file " << filename << endl;
	}
}


void displayGrid(int** display, int cols , int rows)
{
	//Displaying first row '#' border line 
	cout << setw(6) << left << " ";
	for (int i = 0; i < cols+2; i++)
	{
		cout << setw(6) << left << "#";
	}
	cout << endl << endl;
	

	for (int i = rows - 1; i >= 0; i--)
	{
		//displays 'y-axis' number and left '#' border
		cout << setw(6) << left << i << setw(6) << left << "#";
		//displays grid content
		for (int j = 0; j < cols; j++)
		{
			if(display[i][j] != -1)
				cout << setw(6) << left << display[i][j];
			else
				cout << setw(6) << left << " ";
		}
		//displays right '#' border
		cout << setw(6) << left << "#" << endl << endl;
	}

	//Displaying last row '#' border line
	cout << setw(6) << left << " ";
	for (int i = 0; i < cols+2; i++)
	{
		cout << setw(6) << left << "#";
	}
	cout << endl << endl;
	
	//displays x-axis numbers
	cout << setw(6) << left << " " << setw(6) << left << " ";
	for (int i = 0; i < cols; i++)
	{
		cout << setw(6) << left << i;
	}
	cout << endl;
}

void displayGrid(string ** display, int cols, int rows)
{

	cout << setw(6) << left << " ";
	for (int i = 0; i < cols + 2; i++)
	{
		cout << setw(6) << left << "#";
	}
	cout << endl << endl;


	for (int i = rows - 1; i >= 0; i--)
	{
		cout << setw(6) << left << i << setw(6) << left << "#";
		for (int j = 0; j < cols; j++)
		{
			if (display[i][j] != "")
				cout << setw(6) << left << display[i][j];
			else
				cout << setw(6) << left << " ";
		}
		cout << setw(6) << left << "#" << endl << endl;
	}


	cout << setw(6) << left << " ";
	for (int i = 0; i < cols + 2; i++)
	{
		cout << setw(6) << left << "#";
	}
	cout << endl << endl;


	cout << setw(6) << left << " " << setw(6) << left << " ";
	for (int i = 0; i < cols; i++)
	{
		cout << setw(6) << left << i;
	}
	cout << endl;
}

//Function to display single digits
void displayCloudsPressureIndex(int** display, int cols, int rows)
{


	//Init a temporary grid containing single digits
	int** temp = new int*[rows];
	for (int i = 0; i < rows; i++)
	{
		temp[i] = new int[cols];
		for (int j = 0; j < cols; j++)
		{
			temp[i][j] = display[i][j] / 10;
		}
	}

	displayGrid(temp, cols, rows);

	//deallocating temporary grid 
	for (int i = 0; i < rows; i++)
		delete[] temp[i];
		delete[] temp;
}

//Function to display LMH symbols
void displayCloudsPressureLmh(int ** display, int cols, int rows)
{


	string** letterArray = new string*[rows];
	for (int i = 0; i < rows; i++)
	{
		letterArray[i] = new string[cols];
		for (int j = 0; j < cols; j++)
		{
			
			if (display[i][j] >= 0 && display[i][j]< 35)
			{
				letterArray[i][j] = "L";
			}
			else if (display[i][j] >= 35 && display[i][j] < 65)
			{
				letterArray[i][j] = "M";
			}
			else
			{
				letterArray[i][j] = "H";
			}
		}
	}

	displayGrid(letterArray, cols, rows);
}

string displayLMH(double value)
{

	if (value >= 0 && value< 35)
	{
		return "L";
	}
	else if (value >= 35 && value < 65)
	{
		return "M";
	}
	else
	{
		return "H";
	}
}

//ASCII Graphics Display
string displayPattern(double value)
{
	if (value == 90)
	{
		return "~~~~\n~~~~~\n\\\\\\\\\\";
	}
	else if (value == 80)
	{
		return "~~~~\n~~~~~\n \\\\\\\\";
	}
	else if (value == 70)
	{
		return "~~~~\n~~~~~\n   \\\\\\";
	}
	else if (value == 60)
	{
		return "~~~~\n~~~~~\n   \\\\";
	}
	else if (value == 50)
	{
		return "~~~~\n~~~~~\n    \\";
	}
	else if (value == 40)
	{
		return "~~~~\n~~~~~\n";
	}
	else if (value == 30)
	{
		return "~~~\n~~~~\n";
	}
	else if (value == 20)
	{
		return "~~\n~~~\n";
	}
	else if (value == 10)
	{
		return "~\n~~\n";
	}
	else
	{
		return "NIL";
	}
}

bool namePresent(string* arr, string find, int size)
{

	for (int i = 0; i < size; i++)
	{
		if (arr[i] == find)
			return true;
		else if (arr[i] == "")
			return false;
	}
	return false;
}

//Checks for probability of rain
double probability(string ACC, string AP)
{
	if (AP == "L" && ACC == "H")
		return 90.00;
	else if (AP == "L" && ACC == "M")
		return 80.00;
	else if (AP == "L" && ACC == "L")
		return 70.00;
	else if (AP == "M" && ACC == "H")
		return 60.00;
	else if (AP == "M" && ACC == "M")
		return 50.00;
	else if (AP == "M" && ACC == "L")
		return 40.00;
	else if (AP == "H" && ACC == "H")
		return 30.00;
	else if (AP == "H" && ACC == "M")
		return 20.00;
	else if (AP == "H" && ACC == "L")
		return 10.00;
	else
		return 0;
}

double calculateAverage(string ** cityGrid, int ** numGrid, int cols, int rows,string val)
{
	double sum = 0;
	double count = 0.0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (cityGrid[i][j] == val)
			{
				sum += numGrid[i][j];
				count++;

				bool BtmEmpty = false;
				bool LftEmpty = false;
				bool REmpty = false;
				bool TopEmpty = false;

				//Checks for validity of index and also checks if anything is present above, below left and right of the cell
				if ((i - 1 > -1 && cityGrid[i - 1][j] != val))
					BtmEmpty = true;
				if (j - 1 > -1 && cityGrid[i][j-1] != val)
					LftEmpty = true;
				if (j + 1 < cols && cityGrid[i][j + 1] != val)
					REmpty = true;
				if (i + 1 < rows && cityGrid[i + 1][j] != val)
					TopEmpty = true;

				if (BtmEmpty == true)
				{
					sum += numGrid[i - 1][j];
					count++;
					if (LftEmpty == true)
					{
						sum += numGrid[i - 1][j - 1];	
						count++;
					}
					if (REmpty == true)	
					{
						sum += numGrid[i - 1][j + 1];
						count++;
					}
				}
				if (TopEmpty == true)
				{
					sum += numGrid[i + 1][j];
					count++;
					{
						sum += numGrid[i + 1][j - 1];
						count++;
					}
					if (REmpty == true)	
					{
						sum += numGrid[i + 1][j + 1];
						count++;
					}
				}
				if (LftEmpty == true)	
				{
					sum += numGrid[i][j-1];
					count++;
				}
				if (REmpty == true)	
				{
					sum += numGrid[i][j+1];
					count++;
				}

			}
		}
	}
	return sum/count;
}


//Function to generate report
void generateReport(string ** cityGrid, int ** cloudGrid, int ** pressureGrid, int ** idsGrid, int cols, int rows)
{
	int index = 0;
	string cityName = "";

	//Initialisation of temp array
	string* nameArray = new string[rows*cols];
	for (int i = 0; i < rows*cols; i++)
	{
		nameArray[i] = "";
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++) 
		{
			if (cityGrid[i][j] != "")
			{
				//Display details
				if (namePresent(nameArray, cityGrid[i][j],rows*cols) == false)
				{
					cityName = cityGrid[i][j];
					nameArray[index] = cityName;
					double avgCC = calculateAverage(cityGrid, cloudGrid, cols, rows, cityName);
					double avgP = calculateAverage(cityGrid, pressureGrid, cols, rows, cityName);
					string avgCcSymbol = displayLMH(avgCC);
					string avgPSymbol = displayLMH(avgP);
					cout << "City Name : " << cityName << endl;
					cout << "City ID   : " << idsGrid[i][j] << endl;
					//To specify min precision,fixed meaning a fixed number of decimal digits after decimal point
					cout << "Ave. Cloud Cover (ACC)	: " << setprecision(2) << fixed << avgCC << "  (" << avgCcSymbol << ")" << endl;
					cout << "Ave. Pressure    (AP)	: " << setprecision(2) << fixed << avgP << "  (" << avgPSymbol << ")" << endl;
					cout << "Probability of Rain (%) : " << setprecision(2) << fixed << probability(avgCcSymbol, avgPSymbol) << endl; 
					cout << displayPattern(probability(avgCcSymbol, avgPSymbol)) << endl;
					cout << endl << endl << endl;
					index++;
				}
				
			}
		}
	}
	//Deallocating temp array
	delete[] nameArray;
}

//This is the main menu
void mainMenu()
{
	cout << "\nStudent ID: 7058901" << endl;
	cout << "Student Name: Ng Chin Chia" << endl;
	//This function prints all the choices the user can choose
	cout << "---------------------------" << endl;
	cout << "Welcome to Weather Information Processing System!" << endl;
	cout << endl;
	cout << "1)	 Read in and process a configuration file" << endl;
	cout << "2)	 Display city map" << endl;
	cout << "3)	 Display cloud coverage map (cloudiness index)" << endl;
	cout << "4)	 Display cloud coverage map (LMH symbols)" << endl;
	cout << "5)	 Display atmospheric pressure map (pressure index)" << endl;
	cout << "6)	 Display atmospheric pressure map (LMH symbols)" << endl;
	cout << "7)	 Show weather forecast summary report" << endl;
	cout << "8)	 Quit" << endl;
	cout << endl;
	cout << "Please enter your choice: ";
}

int main()
{
	
	//Declaring variables
	char option = ' ';
	int xIdexMax = -1;		
	int yIdexMax = -1;	
	string AccessFileNames[3];

	int** idGrid = NULL;
	string** cityGrid = NULL;
	int** cloudGrid = NULL;
	int** pressureGrid= NULL;

  do
	{
		mainMenu();
		cin>>option;
		cout<<endl;

		if (option == '1')
		{
			string filename;
			cout<<"[Read in and process a configuration file]" << endl;
			try
			{			
				cout<<"Please enter filename: ";
				cin>>filename;
				readConfigFile(filename, xIdexMax, yIdexMax, AccessFileNames, idGrid, cityGrid, cloudGrid, pressureGrid);
			
			}
			catch(exception)
			{
				cout<<"Wrong format, please try again."<<endl;
			}
		}
		else if (option == '2')
		{
			if (cityGrid != NULL)
				displayGrid(idGrid, xIdexMax, yIdexMax);
			else
				cout << "Please select choice 1 first." << endl;
		}
		else if (option == '3')
		{
			if (cityGrid != NULL)
				displayCloudsPressureIndex(cloudGrid, xIdexMax, yIdexMax);
			else
				cout << "Please select choice 1 first."<< endl;
		}
		else if (option == '4')
		{
			if (cityGrid != NULL)
				displayCloudsPressureLmh(cloudGrid, xIdexMax, yIdexMax);
			else
				cout << "Please select choice 1 first." << endl;
		}
		else if (option == '5')
		{
			if (cityGrid != NULL)
				displayCloudsPressureIndex(pressureGrid, xIdexMax, yIdexMax);
			else
				cout << "Please select choice 1 first." << endl;
		}
		else if (option == '6')
		{
			if (cityGrid != NULL)
				displayCloudsPressureLmh(pressureGrid, xIdexMax, yIdexMax);
			else
				cout << "Please select choice 1 first." << endl;
		}
		else if (option == '7')
		{
			if (cityGrid != NULL)
				generateReport(cityGrid, cloudGrid, pressureGrid, idGrid, xIdexMax, yIdexMax);
			else
				cout << "Please select choice 1 first." << endl;
		}
		else if(option =='8') 
		{
			//Deallocation before exiting main menu
			if (cityGrid != NULL)
			{
				for (int i = 0; i < yIdexMax; i++) 
				{
					delete[] cityGrid[i];
					delete[] cloudGrid[i];
					delete[] pressureGrid[i];
					delete[] idGrid[i];
				}
				delete[] cityGrid;
				delete[] cloudGrid;
				delete[] pressureGrid;
				delete[] idGrid;
			}
			break;
		}
		else
		{
			cout<<"Please select choices 1-8."<<endl;
		}
  	} 
	while(option!=8);	
    return 0; 
    
}
			

