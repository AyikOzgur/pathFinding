#pragma once
#include <opencv2/opencv.hpp>

enum class Type
{
    START,
    DESTINATION,
    PATH,
    OBSTACKLA,
    WAY
};

class NodePoint
{

public:
    NodePoint(cv::Point location)
    {
        m_location = location;
    }

    cv::Point getPoint()
    {
        return m_location;
    }

    int getX()
    {
        return (m_location.x - 5) / 10;
    }

    int getY()
    {
        return (m_location.y - 5) / 10;
    }

    cv::Scalar getColor()
    {
        return m_color;
    }

    Type getType()
    {
        return m_type;
    }

    void setLocation(cv::Point location)
    {
        m_location = location;
    }

    void setType(Type type)
    {
        switch (type)
        {
        case Type::PATH:
            m_color = cv::Scalar(0, 255, 0);
            break;
        case Type::OBSTACKLA:
            if (m_type == Type::OBSTACKLA)
            {
                m_color = cv::Scalar(0, 255, 0);
                m_type = Type::PATH;
                return;
            }
            else
                m_color = cv::Scalar(0, 0, 0);
            break;
        case Type::START:
            m_color = cv::Scalar(255, 0, 0);
            break;
        case Type::DESTINATION:
            m_color = cv::Scalar(0, 0, 255);
            break;
        case Type::WAY:
            m_color = cv::Scalar(0, 255, 255);
            break;
        }

        m_type = type;
    }

    float f, g, h;
    bool m_isChecked{false};
    NodePoint *m_parent{nullptr};

private:
    cv::Scalar m_color{cv::Scalar(0, 255, 0)};
    cv::Point m_location;
    Type m_type{Type::PATH};
};
