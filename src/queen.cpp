#include <cstdlib>
#include <ctime>
#include <string>
#include <ros/ros.h>
#include <robots_colony/Commands.h>
using namespace ros;
int MinCoordinate = -10;
int MaxCoordinate = 10;
int WorkersCount=3;
double GenerateCoordinate()
{
	return (double)(MinCoordinate + rand() % (MaxCoordinate - MinCoordinate + 1));
}
int main(int argc, char **argv)
{
	init(argc, argv, "queen");
	NodeHandle n;
	robots_colony::Commands command;
	srand(time(NULL));
	while(true)
	{
		for(int i=1; i<=WorkersCount; i++)
		{
			Publisher pub = n.advertise<robots_colony::Commands>(("exploring_worker"+std::to_string(i)).c_str(), 1000);
			command.command = "explore";
			command.x = GenerateCoordinate();
			command.y = GenerateCoordinate();
			sleep(1);
			pub.publish(command);
			sleep(1);
		}
		sleep(10);
	}
}
