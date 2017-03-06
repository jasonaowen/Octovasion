//
//  GameState.cpp
//  GIP
//
//  Created by gwendolyn weston on 3/6/17.
//
//

#include "GameState.hpp"

GameState::GameState(int width, int height)
: worldWidth(width)
, worldHeight(height)
, rightTentacle({width * 3 / 4, height * 3 / 4})
, leftTentacle({width / 4, height * 3 / 4})
, capturedOctobabies(0)
, paddle({width/2, 0})
, bullets({})
{}

void GameState::handleAction(Action action) {
    switch (action) {
        case Action::LEFT:
            if (paddle.x > 0) {
                paddle.x--;
            }
            break;
        case Action::RIGHT:
            if (paddle.x < worldWidth) {
                paddle.x++;
            }
            break;
        case Action::FIRE_RIGHT_BULLET:
            fireBullet(rightTentacle);
            break;
        case Action::FIRE_LEFT_BULLET:
            fireBullet(leftTentacle);
            break;
        default:
            break;
    }
}

void GameState::fireBullet(Point origin) {
    bullets.push_back(origin);
}

bool operator==(const Point& lhs, const Point& rhs) {
    return (lhs.x == rhs.x && lhs.y == rhs.y);
}
