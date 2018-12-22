#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <ros/ros.h>
#include <robots_colony/Commands.h>
using namespace ros;
int MinCoordinate = -10;
int MaxCoordinate = 10;
int WorkersCount=3;
struct Coordinate
{
	double x;
	double y;
};
std::vector<Coordinate> FoodSources;
double GenerateCoordinate()
{
	return (double)(MinCoordinate + rand() % (MaxCoordinate - MinCoordinate + 1));
}
int GenerateFoodSourceIndex()
{
	return rand()%(FoodSources.size());	
}
void FillFoodSources()
{
	Coordinate foodSource;
	foodSource.x=-4.0926;
	foodSource.y=-6.37064;
	FoodSources.push_back(foodSource);
	foodSource.x=2.27097;
	foodSource.y=-4.90781;
	FoodSources.push_back(foodSource);
	foodSource.x=7.75741;
	foodSource.y=11.097;
	FoodSources.push_back(foodSource);
	foodSource.x=-5.49876;
	foodSource.y=6.85922;
	FoodSources.push_back(foodSource);
	foodSource.x=-8.18779;
	foodSource.y=-1.68541;
	FoodSources.push_back(foodSource);
	foodSource.x=9.3142;
	foodSource.y=-5.25374;
	FoodSources.push_back(foodSource);
}
int main(int argc, char **argv)
{
	FillFoodSources();//temporary function, later this information will be got from workers
	init(argc, argv, "queen");
	NodeHandle n;
	robots_colony::Commands command;
	srand(time(NULL));
	while(true)
	{
		for(int i=1; i<=WorkersCount; i++)
		{
			Publisher pub = n.advertise<robots_colony::Commands>(("exploring_worker"+std::to_string(i)).c_str(), 1000);
			if((rand()%2==0)||(FoodSources.size()==0))
			{
				command.command = "explore";
				command.x = GenerateCoordinate();
				command.y = GenerateCoordinate();
			}
			else
			{
				command.command = "getFood";
				int foodSourceIndex = GenerateFoodSourceIndex();
				command.x = FoodSources[foodSourceIndex].x;
				command.y = FoodSources[foodSourceIndex].y;
			}
			sleep(1);
			pub.publish(command);
			sleep(1);
		}
		sleep(10);
	}
}
