#pragma once
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "NodePoint.h"

class Map
{
public:
    Map();

    void updateSource(int newIndex);

    void updateDestination(int newIndex);

    void addObstackle(int newIndex);

    void render();

    std::vector<NodePoint *> getNeighbors(int xIndex, int yIndex);

    std::vector<NodePoint *> getNeighbors(NodePoint *me);

    std::vector<NodePoint *> findPath();

private:
    cv::Mat m_image = cv::Mat::zeros(480, 640, CV_8UC3);
    std::vector<NodePoint> m_nodes;

    // index of source node in vector to update when new one is added
    int srcIndex{-1};
    int dstIndex{-1};
};