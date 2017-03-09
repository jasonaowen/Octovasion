//
//  GameState.cpp
//  GIP
//
//  Created by gwendolyn weston on 3/6/17.
//
//

#include "GameState.hpp"
#include <math.h>

GameState::GameState(int width, int height)
: worldWidth(width)
, worldHeight(height)
, rightTentacle({
    (int)(width * 0.69),
    (int)(height * 5 / 8)
  })
, leftTentacle({
    (int)(width * 0.31),
    (int)(height * 5 / 8)
  })
, capturedOctobabies(0)
, paddle({
    width/2,
    0,
    width / 8 > 0 ? width / 8 : 1,
    height / 16 > 0 ? height / 16 : 1
  })
, gameInProgress(true)
, bullets()
{}

bool collisionDetection(Point bullet, Rect paddle) {
    return bullet.x >= paddle.x && bullet.x < paddle.x + paddle.width
        && bullet.y >= paddle.y && bullet.y < paddle.y + paddle.height;
}

void GameState::handleAction(Action action) {
    if (gameInProgress == false && action != Action::NEW_GAME) {
        return;
    }
    switch (action) {
        case Action::NEW_GAME:
            capturedOctobabies = 0;
            gameInProgress = true;
            bullets = {};
            paddle.y = escapeBoundary();
            break;
        case Action::LEFT:
            paddle.x = leftTentacle.x;
            break;
        case Action::CENTER:
            paddle.x = worldWidth / 2;
            break;
        case Action::RIGHT:
            paddle.x = rightTentacle.x;
            break;
        case Action::FIRE_RIGHT_BULLET:
            fireBullet(rightTentacle);
            break;
        case Action::FIRE_LEFT_BULLET:
            fireBullet(leftTentacle);
            break;
        case Action::CHECK_PADDLE_COLLISION:
        {
            auto i = bullets.begin();
            while (i != bullets.end()) {
                bool isCollided = collisionDetection(*i, paddle);
                if (isCollided) {
                    i = bullets.erase(i);
                    capturedOctobabies++;
                    updatePaddleForEscapeBoundaryChange();
                } else {
                    i++;
                }
            }
            break;
        }
        case Action::ESCAPE_BULLET:
        {
            auto i = bullets.begin();
            while (i != bullets.end()) {
                if (i->y <= escapeBoundary()) {
                    i = bullets.erase(i);
                    if (capturedOctobabies > 0) {
                        capturedOctobabies--;
                        updatePaddleForEscapeBoundaryChange();
                    } else {
                        gameInProgress = false;
                    }
                } else {
                    i++;
                }
            }
            break;
        }
        case Action::MOVE_BULLET:
            for (Point &bullet : bullets) {
                bullet.y -= 1;
            }
            break;
        default:
            break;
    }
}

void GameState::fireBullet(Point origin) {
    bullets.push_back(origin);
}

int GameState::escapeBoundary() {
    return ceil((float)capturedOctobabies/worldWidth);
}

void GameState::updatePaddleForEscapeBoundaryChange() {
  paddle.y = escapeBoundary();
}

bool operator==(const Point& lhs, const Point& rhs) {
    return (lhs.x == rhs.x && lhs.y == rhs.y);
}

bool operator==(const Rect& lhs, const Rect& rhs) {
    return (lhs.x == rhs.x && lhs.y == rhs.y &&
            lhs.width == rhs.width && lhs.height && rhs.height);
}
