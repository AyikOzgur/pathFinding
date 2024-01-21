#include "Map.h"

float heuristic(NodePoint *src, NodePoint *dst)
{
    float xdiff = (src->getX() - dst->getX()) * (src->getX() - dst->getX());
    float ydiff = (src->getY() - dst->getY()) * (src->getY() - dst->getY());
    return sqrt(xdiff + ydiff);
}

float distanceBetween(NodePoint *src, NodePoint *dst)
{
    float xdiff = (src->getX() - dst->getX()) * (src->getX() - dst->getX());
    float ydiff = (src->getY() - dst->getY()) * (src->getY() - dst->getY());
    return sqrt(xdiff + ydiff);
}

// Function to find the NodePoint with the lowest f value
NodePoint *findLowestF(const std::vector<NodePoint *> &nodes)
{
    if (nodes.empty())
    {
        return nullptr; // Return nullptr if the vector is empty
    }

    NodePoint *lowestFNode = nodes[0];
    int lowestFValue = std::numeric_limits<int>::max();

    for (NodePoint *node : nodes)
    {
        if (node != nullptr && node->f < lowestFValue)
        {
            lowestFNode = node;
            lowestFValue = node->f;
        }
    }

    return lowestFNode;
}

void sortNodePointsByF(std::vector<NodePoint *> &nodePoints)
{
    std::sort(nodePoints.begin(), nodePoints.end(),
              [](const NodePoint *a, const NodePoint *b)
              {
                  return a->f < b->f;
              });
}

bool isInList(std::vector<NodePoint *> &list, NodePoint *element)
{
    // Search for the pointer in the vector
    auto it = std::find(list.begin(), list.end(), element);

    if (it != list.end())
    {
        // Found test in list
        return true;
    }
    else
    {
        // test not found in list
        return false;
    }
}

void removeFromList(std::vector<NodePoint *> &list, NodePoint *element)
{
    // Search for the pointer in the vector
    auto it = std::find(list.begin(), list.end(), element);

    if (it != list.end())
    {
        // Found test in list
        list.erase(it);
    }
    else
    {
        // test not found in list
        return;
    }
}

Map::Map()
{
    cv::namedWindow("Test Window", cv::WINDOW_AUTOSIZE);

    // create all points as green
    for (int j = 0; j < 48; ++j)
    {
        for (int i = 0; i < 64; ++i)
        {
            m_nodes.emplace_back(cv::Point(i * 10 + 5, j * 10 + 5));
        }
    }
}

void Map::updateSource(int newIndex)
{
    // clear previous source if exist
    if (srcIndex != -1)
    {
        m_nodes.at(srcIndex).setType(Type::PATH);
    }

    // update new src color
    m_nodes.at(newIndex).setType(Type::START);

    srcIndex = newIndex;
    render();
}

void Map::updateDestination(int newIndex)
{
    // clear previous destination if exist
    if (dstIndex != -1)
    {
        m_nodes.at(dstIndex).setType(Type::PATH);
    }

    // update new src color
    m_nodes.at(newIndex).setType(Type::DESTINATION);

    dstIndex = newIndex;
    render();
}

void Map::addObstackle(int newIndex)
{
    // update new src color
    m_nodes.at(newIndex).setType(Type::OBSTACKLA);

    render();
}

void Map::render()
{
    for (auto &node : m_nodes)
    {
        cv::circle(m_image, node.getPoint(), 5, node.getColor(), -1);
    }

    // Show the image
    cv::imshow("Test Window", m_image);
}

std::vector<NodePoint *> Map::getNeighbors(int xIndex, int yIndex)
{
    // neighbor indices, right left up down.
    int rIndex = yIndex * 64 + xIndex + 1;
    int lIndex = yIndex * 64 + xIndex - 1;
    int uIndex = (yIndex - 1) * 64 + xIndex;
    int dIndex = (yIndex + 1) * 64 + xIndex;

    NodePoint *me = &m_nodes.at(yIndex * 64 + xIndex);
    NodePoint *rn = &m_nodes.at(rIndex);
    NodePoint *ln = &m_nodes.at(lIndex);
    NodePoint *un = &m_nodes.at(uIndex);
    NodePoint *dn = &m_nodes.at(dIndex);

    std::vector<NodePoint *> neigbors;

    return neigbors;
}

std::vector<NodePoint *> Map::getNeighbors(NodePoint *me)
{
    std::vector<NodePoint *> neigbors;

    // coordinates of me
    int xIndex = (me->getPoint().x - 5) / 10;
    int yIndex = (me->getPoint().y - 5) / 10;

    // neighbor indices, right left up down.
    int rIndex = yIndex * 64 + xIndex + 1;
    int lIndex = yIndex * 64 + xIndex - 1;
    int uIndex = (yIndex - 1) * 64 + xIndex;
    int dIndex = (yIndex + 1) * 64 + xIndex;

    // right neighbor
    if (rIndex >= 0 && rIndex <= m_nodes.size())
    {
        NodePoint *rn = &m_nodes.at(rIndex);
        if (rn->getType() != Type::OBSTACKLA && !rn->m_isChecked)
            neigbors.push_back(rn);
    }
    // left
    if (lIndex >= 0 && lIndex <= m_nodes.size())
    {
        NodePoint *ln = &m_nodes.at(lIndex);
        if (ln->getType() != Type::OBSTACKLA && !ln->m_isChecked)
            neigbors.push_back(ln);
    }
    // up
    if (uIndex >= 0 && uIndex <= m_nodes.size())
    {
        NodePoint *un = &m_nodes.at(uIndex);
        if (un->getType() != Type::OBSTACKLA && !un->m_isChecked)
            neigbors.push_back(un);
    }
    // down
    if (dIndex >= 0 && dIndex <= m_nodes.size())
    {
        NodePoint *dn = &m_nodes.at(dIndex);
        if (dn->getType() != Type::OBSTACKLA && !dn->m_isChecked)
            neigbors.push_back(dn);
    }
    // left-up
    if (uIndex - 1 >= 0 && uIndex - 1 <= m_nodes.size())
    {
        NodePoint *lun = &m_nodes.at(uIndex - 1);
        if (lun->getType() != Type::OBSTACKLA && !lun->m_isChecked)
            neigbors.push_back(lun);
    }
    // right-up
    if (uIndex + 1 >= 0 && uIndex + 1 <= m_nodes.size())
    {
        NodePoint *run = &m_nodes.at(uIndex + 1);
        if (run->getType() != Type::OBSTACKLA && !run->m_isChecked)
            neigbors.push_back(run);
    }
    // left-down
    if (dIndex - 1 >= 0 && dIndex - 1 <= m_nodes.size())
    {
        NodePoint *ldn = &m_nodes.at(dIndex - 1);
        if (ldn->getType() != Type::OBSTACKLA && !ldn->m_isChecked)
            neigbors.push_back(ldn);
    }
    // rigth-down
    if (dIndex + 1 >= 0 && dIndex + 1 <= m_nodes.size())
    {
        NodePoint *rdn = &m_nodes.at(dIndex + 1);
        if (rdn->getType() != Type::OBSTACKLA && !rdn->m_isChecked)
            neigbors.push_back(rdn);
    }

    return neigbors;
}

std::vector<NodePoint *> Map::findPath()
{
    std::vector<NodePoint *> path;
    std::vector<NodePoint *> openList;
    std::vector<NodePoint *> closedList;

    // start and destination nodes
    NodePoint *startNode = &m_nodes.at(srcIndex);
    NodePoint *endNode = &m_nodes.at(dstIndex);

    // put start code to openlist
    openList.push_back(startNode);

    NodePoint *currentNode;

    while (!openList.empty())
    {
        // sort openList by f
        sortNodePointsByF(openList);

        // get node with lowest f from openlist
        currentNode = openList.front();
        openList.erase(openList.begin()); // Remove the first element

        // put current node to closed list
        closedList.push_back(currentNode);

        // check if destination found
        if (currentNode == endNode)
        {
            break;
        }

        // be sure this node will be checked once
        currentNode->m_isChecked = true;

        // Update neeighboors
        std::vector<NodePoint *> neighbors = getNeighbors(currentNode);
        for (auto &neighbor : neighbors)
        {
            float newCost = currentNode->g + distanceBetween(currentNode, neighbor);

            if (isInList(openList, neighbor) && newCost < neighbor->g)
            {
                removeFromList(openList, neighbor); // because we found better path.
            }

            if (isInList(closedList, neighbor) && newCost < neighbor->g)
            {
                removeFromList(closedList, neighbor);
            }

            if (!isInList(closedList, neighbor) && !isInList(openList, neighbor))
            {
                neighbor->g = newCost;
                openList.push_back(neighbor);
                neighbor->f = neighbor->g + heuristic(neighbor, endNode);
                neighbor->m_parent = currentNode;
            }
        }
    }

    // Prepare path.
    NodePoint *node = endNode->m_parent;
    while (node != nullptr)
    {
        path.push_back(node);
        node = node->m_parent;
    }

    return path;
}