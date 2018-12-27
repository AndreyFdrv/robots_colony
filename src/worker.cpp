#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <ros/ros.h>
#include <gazebo_msgs/ModelState.h>
#include <tf/transform_listener.h>
#include <robots_colony/Commands.h>
#include <robots_colony/Coordinates.h>
using namespace ros;
double MinCoordinate = -10;
double MaxCoordinate = 10;
double Step = 0.1;
double QueenX=0;
double QueenY=0;
bool HaveTask=false;
Publisher GazeboPublisher;
Publisher QueenPublisher;
gazebo_msgs::ModelState Msg;
std::vector<robots_colony::Coordinates> FoodSources;
double Distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x1-x2, 2)+pow(y1-y2, 2));
}
void Explore(const robots_colony::Commands &command)
{
	if(HaveTask)
		return;
	HaveTask=true;
	Rate rate(50);
	gazebo_msgs::ModelState msg;
	msg=Msg;
	if(command.command.compare("explore")==0)
	{
    		double x = command.x;
		double y = command.y;
		double distanceToGoal = Distance(x, y, msg.pose.position.x, msg.pose.position.y);
		int StepsCount = (int)(distanceToGoal/Step)+1;
		for(int i=0; i<StepsCount; i++)
		{
			double angle = atan2(y-msg.pose.position.y, x-msg.pose.position.x);
			msg.pose.position.x += Step*cos(angle);
    			msg.pose.position.y += Step*sin(angle);
    			msg.pose.orientation.z = sin(angle/2);
    			msg.pose.orientation.w = cos(angle/2);
			GazeboPublisher.publish(msg);
			for(int j=0; j<FoodSources.size(); j++)
			{
				double distanceToFoodSource = Distance(FoodSources.at(j).x, FoodSources.at(j).y, 
					msg.pose.position.x, msg.pose.position.y);
				if(distanceToFoodSource<5)
					QueenPublisher.publish(FoodSources.at(j));
			}
			rate.sleep();
		}
		Msg=msg;
		HaveTask=false;
	}
	else if(command.command.compare("getFood")==0)
	{
		double x = command.x;
		double y = command.y;
		double distance = Distance(x, y, msg.pose.position.x, msg.pose.position.y)-1;
		int StepsCount = (int)(distance/Step)+1;
		for(int i=0; i<StepsCount; i++)
		{
			double angle = atan2(y-msg.pose.position.y, x-msg.pose.position.x);
			msg.pose.position.x += Step*cos(angle);
    			msg.pose.position.y += Step*sin(angle);
    			msg.pose.orientation.z = sin(angle/2);
    			msg.pose.orientation.w = cos(angle/2);
			GazeboPublisher.publish(msg);
			rate.sleep();
		}
		x = QueenX;
		y = QueenY;
		distance = Distance(x, y, msg.pose.position.x, msg.pose.position.y)-2;
		StepsCount = (int)(distance/Step)+1;
		for(int i=0; i<StepsCount; i++)
		{
			double angle = atan2(y-msg.pose.position.y, x-msg.pose.position.x);
			msg.pose.position.x += Step*cos(angle);
    			msg.pose.position.y += Step*sin(angle);
    			msg.pose.orientation.z = sin(angle/2);
    			msg.pose.orientation.w = cos(angle/2);
			GazeboPublisher.publish(msg);
			rate.sleep();
		}
		Msg=msg;
		HaveTask=false;
	}
}
void FillFoodSources()
{
	robots_colony::Coordinates foodSource;
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
	init(argc, argv, "worker");
	FillFoodSources();
    	NodeHandle n;
    	GazeboPublisher = n.advertise<gazebo_msgs::ModelState>("gazebo/set_model_state", 10);
	QueenPublisher = n.advertise<robots_colony::Coordinates>("exploring_queen", 1000);
    	gazebo_msgs::ModelState msg;
    	msg.model_name = argv[1];
    	msg.pose.position.x = atof(argv[2]);
    	msg.pose.position.y = atof(argv[3]);
    	msg.pose.orientation.z = sin(0);
    	msg.pose.orientation.w = cos(0);
	Msg=msg;
	Subscriber sub=n.subscribe(("commands_"+std::string(argv[1])).c_str(), 1000, Explore);
	spin();
}
