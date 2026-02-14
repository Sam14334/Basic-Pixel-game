#pragma once 
#include "SFML/Graphics.hpp"

namespace MathHelpers { 
    float Length(const sf::Vector2f& rVector) {
        float fLength = sqrt(rVector.x * rVector.x + rVector.y * rVector.y);
        return fLength;
    }

    sf::Vector2f Normalize(const sf::Vector2f& rVector) {

        float fLength = Length(rVector);
        sf::Vector2f vNormalizedVector = { rVector.x / fLength , rVector.y / fLength };
        return vNormalizedVector;
    }
};