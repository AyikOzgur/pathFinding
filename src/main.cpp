#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include "NodePoint.h"
#include "Map.h"

using namespace cv;
using namespace std;

void CallBackFunc(int event, int x, int y, int flags, void *userdata)
{

    Map *myMap = (Map *)userdata;

    if (event == EVENT_LBUTTONDOWN)
    {

        // find clicked circle, x and y in pixes unit
        // each circle has 10 pixels so index = pixel / 10
        int xOffset = x / 10;
        int yOffset = y / 10;

        int key = waitKey(0);

        if ((char)key == 's')
        {
            myMap->updateSource(yOffset * 64 + xOffset);
        }
        else if ((char)key == 'd')
        {
            myMap->updateDestination(yOffset * 64 + xOffset);
        }
        else if ((char)key == 'a')
        {
            myMap->addObstackle(yOffset * 64 + xOffset);
        }
    }

    else if (event == EVENT_RBUTTONDOWN)
    {

        std::vector<NodePoint *> foundPath;

        foundPath = myMap->findPath();

        for (auto &node : foundPath)
        {
            node->setType(Type::WAY);
        }

        // keep source color 
        (foundPath.back())->setType(Type::START);

        myMap->render();
    }
}

int main()
{

    Map myMap;
    myMap.render();
    setMouseCallback("Test Window", CallBackFunc, &myMap);

    while (true)
    {
        // Wait for a key press
        cv::waitKey(0);
    }

    // Clean up and close the window
    cv::destroyAllWindows();

    return 0;
}