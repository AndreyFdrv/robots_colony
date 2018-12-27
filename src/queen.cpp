#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <ros/ros.h>
#include <robots_colony/Commands.h>
#include <robots_colony/Coordinates.h>
using namespace ros;
int MinCoordinate = -10;
int MaxCoordinate = 10;
int WorkersCount=3;
std::vector<robots_colony::Coordinates> FoodSources;
double GenerateCoordinate()
{
	return (double)(MinCoordinate + rand() % (MaxCoordinate - MinCoordinate + 1));
}
int GenerateFoodSourceIndex()
{
	return rand()%(FoodSources.size());	
}
void AddNewFoodSource(const robots_colony::Coordinates &coordinate)
{
	ROS_INFO("New food source was found");
	for(int i=0; i<FoodSources.size(); i++)
	{
		if((FoodSources.at(i).x==coordinate.x) && (FoodSources.at(i).y==coordinate.y))
			return;
	}
	FoodSources.push_back(coordinate);
}
int main(int argc, char **argv)
{
	init(argc, argv, "queen");
	NodeHandle n;
	robots_colony::Commands command;
	srand(time(NULL));
	Subscriber sub=n.subscribe("exploring_queen", 1000, AddNewFoodSource);
	while(true)
	{
		spinOnce();
		for(int i=1; i<=WorkersCount; i++)
		{
			Publisher pub = n.advertise<robots_colony::Commands>(("commands_worker"+std::to_string(i)).c_str(), 1000);
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
